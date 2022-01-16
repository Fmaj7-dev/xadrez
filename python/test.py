import chess
import sys

""""
count = 0
# level 1
legal1 = board.legal_moves
for m1 in legal1:
    board.push(m1)
    #print(board.fen())
    # level 2
    legal2 = board.legal_moves
    for m2 in legal2:
        board.push(m2)
        #print(board.fen())
        # level 3
        legal3 = board.legal_moves
        for m3 in legal3:
            board.push(m3)
            #print(board.fen())
            count += 1
            board.pop()        
        board.pop()
    board.pop()
"""
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