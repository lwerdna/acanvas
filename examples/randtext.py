#!/usr/bin/env python

import random
from subprocess import Popen, PIPE, STDOUT

width = 1280
height = 1024

p = Popen(['acanvas'], stdin=PIPE)
p.stdin.write('resize 0 0 %d %d\n' % (width, height))

for i in range(50):
	x = random.randint(20, width-20)
	y = random.randint(20, height-20)
	[r,g,b] = map(lambda x: random.randint(0,255), [0,0,0])
	fontSize = random.randint(10,50)
	fontId = random.choice([0,4,8,12])
	p.stdin.write('color %02X%02X%02X\n' % (r,g,b))
	p.stdin.write('font %d %d\n' % (fontId, fontSize))
	p.stdin.write('write %d %d ID=%d SIZE=%d\n' % (x,y,fontId,fontSize))

print 'press ctrl+c to quit'
try:	
	while 1:
		pass
except KeyboardInterrupt:
	pass

p.kill()



