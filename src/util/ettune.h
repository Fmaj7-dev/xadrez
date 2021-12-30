#include <chrono>
#include <string>

class EtTune
{
private:
    EtTune(); // Disallow instantiation outside of the class.
public:
    EtTune( const EtTune& ) = delete;
    EtTune& operator=( const EtTune & ) = delete;
    EtTune( EtTune && ) = delete;
    EtTune& operator=( EtTune && ) = delete;

    void addMeasure( const std::string& name, const std::chrono::time_point time )
    {

    }

    void printMeasures()
    {}

    static auto& instance()
    {
        static EtTune tune;
        return tune;
    }
};

class Chrono
{
public:
    Chrono(std::string name)
    : name_(name)
    {
        start_ = std::chrono::high_resolution_clock::now();
    }

    ~Chrono()
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = now - start_;

        EtTune::instance().addMeasure(name_, duration);
    }

private:
    std::chrono::time_point start_;
    std::string name_;
};

#define MEASURE Chrono chrono(__FUNCTION__);