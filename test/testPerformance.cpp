#include "catch.hpp"
#include "chessboard.h"
#include "variation.h"

#include <iostream>
#include <chrono>

TEST_CASE("Test Performance")
{
    auto start = std::chrono::steady_clock::now();

    Chessboard cb;
    cb.initDefault();

    Variations v;
    cb.findVariations(v);

    unsigned int numVariations = 0;

    for( size_t i = 0; i < v.size(); ++i)
    {
        //numVariations++;
        float neval = v[i].chessboard_.evaluation();

        // second level variations
        {
            Variations v2;
            v[i].chessboard_.findVariations(v2);

            for ( size_t j = 0; j < v2.size(); ++j )
            {
                //numVariations++;
                // third level variations
                {
                    Variations v3;
                    v2[j].chessboard_.findVariations(v3);

                    for ( size_t k = 0; k < v3.size(); ++k )
                    {
                        //numVariations++;
                        // fourth level
                        {
                            Variations v4;
                            v3[k].chessboard_.findVariations(v4);

                            for ( size_t l = 0; l < v4.size(); ++l )
                            {
                                //numVariations++;
                                // fifth level
                                {
                                    Variations v5;
                                    v4[l].chessboard_.findVariations(v5);

                                    for ( size_t m = 0; m < v5.size(); ++m )
                                    {
                                        numVariations++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << std::endl;

    std::cout << "Number of variations: " << numVariations << std::endl;
}