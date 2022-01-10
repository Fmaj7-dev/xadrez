#include "tree.h"
#include "util/ettune.h"

Node::Node()
{

}

float Node::minimax( bool maximizing )
{MEASURE
    if ( children_.empty() )
        return variation_.chessboard_.evaluation();

    if ( maximizing )
    {
        float value = -10000;
        for ( auto& node : children_ )
        {
            float newVal = node.minimax();

            if ( newVal > value )
            {
                value = newVal;
            }
        }
        return value;
    }
    else
    {
        float value = 10000;
        for ( auto& node : children_ )
        {
            float newVal = node.minimax();

            if ( newVal < value )
            {
                value = newVal;
            }
        }
        return value;
    }
}