#!/usr/local/cs/bin/python3

import random, sys
from optparse import OptionParser

class shuffle:
    def __init__(self, filename):
        if filename is not sys.stdin:
            f = open(filename, 'r')
        else:
            f = filename
        self.lines = f.readlines()
        f.close()
        random.shuffle(self.lines)

    def getLines(self):
        return self.lines

def main():
    version_msg = "%prog ?"
    usage_msg = """\n%prog -e [OPTION]... [ARG]...\n%prog [OPTION]... [FILE]"""
    parser = OptionParser(version=version_msg,
                          usage=usage_msg)

    parser.add_option("-e", "--echo",
                      action="store_true", dest="echo", default=False,
                      help="treat each ARG as an input line")

    parser.add_option("-n", "--head-count",
                      action="store", dest="headcount", type=int,
                      help="output at most COUNT lines (default ALL)")

    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat", default=False,
                      help="allow repetitions of lines")

    options, args = parser.parse_args(sys.argv[1:])            
   
    if len(args) == 0 or args[0] == '-':
        input_file = sys.stdin
    else:
        input_file = args[0]
    if not options.echo:
        try:
            shuffled = shuffle(input_file).getLines()
        except IOError as e:
            parser.error("I/O error: {0}".format(e))
    else:
        random.shuffle(args)
        shuffled = args

    
    if options.headcount is None:
        headcount = len(shuffled)
    else:
        headcount = min(int(options.headcount), len(shuffled))
    
    if int(headcount) < 0:
        parser.error("negative headcount: {0})".format(headcount))
    
    try:
        for index in range(headcount):
            if options.repeat and type(options.headcount) is not int:
                while True:
                    try:
                        print(random.choice(shuffled).rstrip('\n'))
                    except:
                        sys.exit()
            elif options.repeat:
                print(random.choice(shuffled).rstrip('\n'))
            else:
                print(shuffled[index].rstrip('\n'))
    except IOError as e:
        parser.error("I/O error: {0}".format(e))

if __name__ == "__main__":
    main()
