# move generator using chess library to compare with our own results

import chess
import sys

# global
total_moves = 0

# perf test
def perf(board, depth ):
    global total_moves
    legal = board.legal_moves

    if (depth == 1):
        # output the leaves of the tree
        for m in legal:
            board.push(m)
            print(board.fen())
            board.pop()
            
        total_moves += len(list(legal))
        return len(list(legal))

    for m in legal:
        board.push(m)
        perf(board, depth - 1 )
        board.pop()

# invoke perf
board = chess.Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1")
perf(board, int(sys.argv[1]))
print(total_moves)