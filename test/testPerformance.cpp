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

    //kiwipete
    //cb.importFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

    //
    //cb.importFen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

    //
    //cb.importFen("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ");

    Variations v;
    cb.findVariations(v);

    unsigned int numVariations1 = 0;
    unsigned int numVariations2 = 0;
    unsigned int numVariations3 = 0;
    unsigned int numVariations4 = 0;
    unsigned int numVariations5 = 0;
    unsigned int numVariations6 = 0;
    unsigned int numVariations7 = 0;

    //printVariations(v);

    for( size_t i = 0; i < v.size(); ++i)
    {
        numVariations1++;
        //float neval = v[i].chessboard_.evaluation();

        // second level variations
        {
            Variations v2;
            v[i].chessboard_.findVariations(v2);

            for ( size_t j = 0; j < v2.size(); ++j )
            {
                numVariations2++;
                //std::cout<<v2[j].chessboard_.exportFen()<<std::endl;
                
                // third level variations
                {
                    Variations v3;
                    v2[j].chessboard_.findVariations(v3);

                    for ( size_t k = 0; k < v3.size(); ++k )
                    {
                        numVariations3++;
                        // fourth level
                        {
                            Variations v4;
                            v3[k].chessboard_.findVariations(v4);

                            for ( size_t l = 0; l < v4.size(); ++l )
                            {
                                numVariations4++;
                                // fifth level
                                {
                                    Variations v5;
                                    v4[l].chessboard_.findVariations(v5);

                                    if (v5.empty())
                                    {
                                        std::cout<<"checkmate level 5"<<std::endl;
                                        std::cout<<v4[l].chessboard_.exportFen()<<std::endl;
                                    }

                                    for ( size_t m = 0; m < v5.size(); ++m )
                                    {
                                        numVariations5++;
                                        // sixth level
                                        {
                                            Variations v6;
                                            v5[m].chessboard_.findVariations(v6);

                                            if (v6.empty())
                                            {
                                                std::cout<<"checkmate level 6"<<std::endl;
                                                std::cout<<v5[m].chessboard_.exportFen()<<std::endl;
                                            }

                                            for ( size_t n = 0; n < v6.size(); ++n )
                                            {
                                                numVariations6++;
                                            }
                                        }
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

    std::cout << "Number of variations 1: " << numVariations1 << std::endl;
    std::cout << "Number of variations 2: " << numVariations2 << std::endl;
    std::cout << "Number of variations 3: " << numVariations3 << std::endl;
    std::cout << "Number of variations 4: " << numVariations4 << std::endl;
    std::cout << "Number of variations 5: " << numVariations5 << std::endl;
    std::cout << "Number of variations 6: " << numVariations6 << std::endl;
}
