#!/usr/bin/env python3
import os
import sys
import system
import getopt

def read_file():
    dic = {}

    dic['1024'] = {}
    dic['4096'] = {}
    dic['8000'] = {}

    
    dic['1024']['tot'] = 0
    dic['1024']['cnt'] = 0
    dic['1024']['average'] = 0

    
    dic['4096']['tot'] = 0
    dic['4096']['cnt'] = 0
    dic['4096']['average'] = 0

    dic['8000']['tot'] = 0
    dic['8000']['cnt'] = 0
    dic["8000"]['average'] = 0

    l = ""
    print(dic.items())

    print("reading file now...")
    with open('bc_out.log','r') as f:
	for i, line in enumerate(f):
	    if line.startswith('1024'):
		#print(line)
  	        l = line.split(';')  
		dic["1024"]["tot"] = dic["1024"]["tot"] + float(l[3].rstrip())
		dic["1024"]["cnt"] = dic["1024"]["cnt"] + 1
	    if line.startswith('4096'):
		#print(line)
	        l = line.split(';')
	        dic["4096"]["tot"] = dic["4096"]["tot"] + float(l[3].rstrip())
	        dic["4096"]["cnt"] = dic["4096"]["cnt"] + 1
	    if line.startswith('8000'):
		#print(line)
	        l = line.split(';')
	        dic["8000"]["tot"] = dic["8000"]["tot"] + float(l[3].rstrip())
	        dic["8000"]["cnt"] = dic["8000"]["cnt"] + 1



    dic["1024"]["average"] = (dic["1024"]["tot"] / dic["1024"]["cnt"])
    dic["4096"]["average"] = (dic["4096"]["tot"] / dic["4096"]["cnt"])
    dic["8000"]["average"] = (dic["8000"]["tot"] / dic["8000"]["cnt"])
    print(dic.items())
    print "\n==============================="
    print "\ttotal = {0}\taverage = {1}".format(cnt,average)


def main(argv):
    try:
        opts, unused = getopt.getopt(argv, 'i:r:p:t:d:', ['input_file_name=', 'reps=', 'port=', 'debug='])
    except getopt.GetoptError:
        print('Could not parse inputs')
        sys.exit(-1)

    for opt, arg in opts:
        if opt in ('-i', '--input_file_name'):
            arg_file = arg
        elif opt in ('-r', '--reps'):
            arg_reps = int(arg)
        elif opt in ('-t', '--timeout'):
            arg_timeout = int(arg)
    try: 
	with open('bc_out.log','r'):
	    print("it exists!\n")
	read_file()
    except:
	print("\nERROR: No file found...are you sure bc_out.log exists? (I can't find it)\n\n")
	exit(0)


if __name__ == "__main__":
    main(sys.argv[1:])



