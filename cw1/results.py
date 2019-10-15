#!/usr/bin/env python3
import sys

cnt = 0
average = 0
tot = 0
l = ""

with open('bc_out.log','r') as f:
	for i, line in enumerate(f):
		if line.startswith('1024'):
			l = line.split(';')
			tot = tot + float(l[3].rstrip())
			cnt = cnt + 1

average = tot / cnt
print "\n==============================="
print "\ttotal = {0}\taverage = {1}".format(cnt,average)

