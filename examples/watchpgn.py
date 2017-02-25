#!/usr/bin/env python

# uses python-chess to animate a game with engine analysis on the canvas

import chess
import chess.uci
import chess.pgn

import time

import random
from subprocess import Popen, PIPE, STDOUT

width = 8*64
height = 128+8*64+128

p = Popen(['acanvas'], stdin=PIPE)
p.stdin.write('resize 0 0 %d %d\n' % (width, height))

def getBestMove(board):
	eng = chess.uci.popen_engine("/usr/local/bin/stockfish")
	eng.uci()
	eng.ucinewgame()
	eng.position(board)
	result = eng.go(movetime=2000)
	return result[0]

f = open('example.pgn')
gameNode = chess.pgn.read_game(f)

#
# print player labels
# 
whiteLabel = ''
if 'White' in gameNode.headers:
	whiteLabel = gameNode.headers['White']
if 'WhiteElo' in gameNode.headers:
	whiteLabel += ' (%d)' % int(gameNode.headers['WhiteElo'])

blackLabel = ''
if 'Black' in gameNode.headers:
	blackLabel = gameNode.headers['Black']
if 'BlackElo' in gameNode.headers:
	blackLabel += ' (%d)' % int(gameNode.headers['BlackElo'])

p.stdin.write('font 0 40\n')
p.stdin.write('color FFFFFF\n')
p.stdin.write('write 20 40 %s\n' % whiteLabel)
p.stdin.write('write 20 %s %s\n' % (128+8*64+40, whiteLabel))

i = 0;
while 1:
	move = ''
	# if game node has parent, what was the move that got us here?
	if gameNode.parent:
		move = gameNode.san()

	board = gameNode.board()

	#moveBestUci = getBestMove(board)
	#moveBestSan = board.san(moveBestUci)

	#print "state %d: %s (from %s, best was %s)" % (i, board.fen(), move, moveBestSan)
	p.stdin.write('chess 0 128 %s\n' % board.fen())
	time.sleep(1)

	if gameNode.is_end():
		break;

	gameNode = gameNode.variation(0)

print 'press ctrl+c to quit'
try:	
	while 1:
		pass
except KeyboardInterrupt:
	pass

p.kill()

