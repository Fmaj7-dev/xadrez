#include <chrono>
#include <string>
#include <map>
#include <stack>
#include <iostream>
#include <utility>
#include <vector>

// Represents stack frame execution
class Context
{
public:
    std::string name;
    uint32_t num_times_called;
    uint32_t concurrent_executions;
    std::chrono::duration<double> num_ms;
};

// Node of the tree of Contexts
class ContextNode
{
public:
    Context context;
    std::map<std::string, ContextNode> children;
    ContextNode* parent;
};

// Holds the tree of Contexts
class EtTune
{
private:
    // Disallow instantiation outside of the class.
    EtTune()
    {
        root_.context.num_times_called = 0;
    }; 
public:
    ~EtTune(){printMeasures();}; 


    EtTune( const EtTune& ) = delete;
    EtTune& operator=( const EtTune & ) = delete;
    EtTune( EtTune && ) = delete;
    EtTune& operator=( EtTune && ) = delete;

    void openContext(const std::string& name)
    {
        // if first context, main.
        if (root_.context.num_times_called == 0)
        {
            root_.context.num_times_called = 1;
            root_.context.concurrent_executions = 1;
            root_.context.name = name;

            currentContextNode_ = &root_;
        }
        else
        {
            
            auto search = currentContextNode_->children.find(name);

            // if recursive call
            if (currentContextNode_->context.name == name)
            {
                currentContextNode_->context.concurrent_executions++;
                currentContextNode_->context.num_times_called++;
            }
            // if context found
            else if (search != currentContextNode_->children.end())
            {
                currentContextNode_ = &search->second;
                currentContextNode_->context.num_times_called++;
                currentContextNode_->context.concurrent_executions++;
            }
            // if not found, create it
            else
            {
                ContextNode new_cn;
                new_cn.context.num_times_called = 1;
                new_cn.context.concurrent_executions = 1;
                new_cn.context.name = name;
                new_cn.parent = currentContextNode_;
                currentContextNode_->children[name] = new_cn;
                currentContextNode_ = &currentContextNode_->children[name];
            }
        }
    }

    void closeContext( const std::string& name, const std::chrono::duration<double> ms )
    {
        currentContextNode_->context.concurrent_executions--;

        if (currentContextNode_->context.concurrent_executions == 0)
        {
            if (currentContextNode_->context.num_times_called == 1)
                currentContextNode_->context.num_ms = ms;
            else
                currentContextNode_->context.num_ms += ms;

            currentContextNode_ = currentContextNode_->parent;

            auto search = children.find(name);
            if (search != children.end())
            {
                search->second.context.num_times_called++;
                search->second.context.num_ms += ms;
            }
            else
            {
                ContextNode new_cn;
                new_cn.context.num_times_called = 1;
                new_cn.context.num_ms = ms;
                new_cn.context.name = name;
                children[name] = new_cn;
            }
        }
    }

    void recursivePrint(ContextNode* cn, int depth)
    {
        for (int i = 0; i < depth; i++)
            std::cout << "    ";

        float ratio = root_.context.num_ms.count() / cn->context.num_ms.count();
        float percentage = 100 / ratio;

        std::cout << cn->context.name << ": " << cn->context.num_times_called << " times, " << cn->context.num_ms.count() << " ms, " << percentage<<"%"<<std::endl;

        for (auto& child : cn->children)
        {
            recursivePrint(&child.second, depth + 1);
        }
    }

    static bool cmp(std::pair<std::string, ContextNode>& a,
         std::pair<std::string, ContextNode>& b)
    {
        return a.second.context.num_ms > b.second.context.num_ms;
    }

    void printLinear()
    {
        std::vector<std::pair<std::string, ContextNode> > A;
  
        // Copy key-value pair from Map
        // to vector of pairs
        for (auto& it : children) {
            A.push_back(it);
        }
    
        // Sort using comparator function
        std::sort(A.begin(), A.end(), cmp);
    
        // Print the sorted value
        for (auto& child : A)
        {
            std::cout<<child.second.context.name<<": "<<child.second.context.num_times_called<<" times, "<<child.second.context.num_ms.count()<<" ms " <<std::endl;
        }
    }

    void printMeasures()
    {
        //recursivePrint(&root_, 0);

        std::cout<<"-----------------"<<std::endl;
        printLinear();
        
    }

    static auto& instance()
    {
        static EtTune tune;
        return tune;
    }

    // for tree printing
    ContextNode root_;
    ContextNode* currentContextNode_;

    //
    std::map<std::string, ContextNode> children;
};

// The only class the user should use (through the macro MEASURE)
class Chrono
{
public:
    Chrono(std::string name)
    : name_(name)
    {
        EtTune::instance().openContext(name_);
        start_ = std::chrono::high_resolution_clock::now();
    }

    ~Chrono()
    {
        
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = now - start_;

        EtTune::instance().closeContext(name_, duration);
    }

private:
    std::chrono::steady_clock::time_point start_;
    std::string name_;
};

//#define MEASURE Chrono chrono(__FUNCTION__);
#define MEASURE 