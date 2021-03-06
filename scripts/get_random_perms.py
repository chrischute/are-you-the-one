#!/usr/bin/python
# get_random_perms.py
# Christopher Chute
#
# Generate random permutations (to stdout) for reading by ayto.cpp.

from random import shuffle
from sys import argv

def main():
	if (len(argv) != 2):
		print 'Usage: ./pool_generator.py NUM_TO_GENERATE'
		return
	num_to_generate = int(argv[1])
	pool = {}
	digits = '0123456789'
	while (len(pool) < num_to_generate):
		pool[scramble(digits)] = None

	for digits in pool.keys():
		print digits

def scramble(s):
	s_list = list(s)
	shuffle(s_list)
	return ''.join(s_list)

if __name__ == '__main__':
	main()
