#!/usr.bin/python

import sys, locale, string, re
from optparse import OptionParser

class comm:
    def __init__(self,file1, file2):
        try:
            if file1 == "-":
                f1 = sys.stdin
                f2 = open(file2, 'r')
            elif file2 == "-":
                f2 = sys.stdin
                f1 = open(file1, 'r')
            elif file1 == "-" and file2 == "-":
                print("Can only read one file from stdin")
                exit()
            else:
                f1 = open(file1, 'r')
                f2 = open(file2, 'r')
            self.l1 = f1.read().splitlines()
            self.l2 = f2.read().splitlines()
            self.col1 = []
            self.col2 = []
            self.col3 = []
            f1.close()
        except IOError as e:
            errno = e.errno
            strerror = e.strerror
            parser.error("I/O error({0}): {1}".format(errno, strerror))

    def unsorted_comparison(self):
        for i in range(len(self.l1)):
            for j in range(len(self.l2)):
                if self.l1[i] == self.l2[j]:
                    self.col3.append(self.l1[i])
                    self.col2.append("\t")
                    self.col1.append("\t")
                    del self.l2[j] #delete so that we can add full column later
                    found = True
                    break
                else:
                    found = False
            if found == False:
                self.col1.append(self.l1[i])
                self.col2.append("")
                self.col3.append("")
        for k in range(len(self.l2)):
            self.col1.append("\t")
            self.col2.append(self.l2[k])
            self.col3.append("")

    def is_Sorted(self):
        sorted1 = sorted(self.l1)
        sorted2 = sorted(self.l2)
        if sorted1 != self.l1:
            sys.stderr.write("File 1 is not sorted\n")
            return False
        if sorted2 != self.l2:
            sys.stderr.write("File 2 is not sorted\n")
            return False
        return True

    def sorted_comparison(self):
        if self.is_Sorted():
            i = j = 0
            while i < len(self.l1) and j < len(self.l2):
                if self.l1[i] == self.l2[j]:
                    self.col3.append(self.l1[i])
                    self.col1.append("\t")
                    self.col2.append("\t")
                    self.l1[i] = ''
                    self.l2[j] = ''
                    i += 1
                    j += 1
                elif self.l1[i] > self.l2[j]:
                    self.col2.append(self.l2[j])
                    self.col1.append("\t")
                    self.col3.append("")
                    self.l2[j] = ''
                    j += 1
                elif self.l1[i] < self.l2[j]:
                    self.col1.append(self.l1[i])
                    self.col2.append("")
                    self.col3.append("")
                    self.l1[i] = ''
                    i +=1
            if i > j:
                if len(self.l1) == len(self.l2):
                    self.col1.append("\t")
                    self.col2.append(self.l2[j])
                    self.col3.append("")
                else:
                    diff = i - j
                    for m in range(diff):
                        self.col1.append(self.l1[i+m])
                        self.col2.append("")
                        self.col3.append("")
            elif i < j:
                if len(self.l1) == len(self.l2):
                    self.col1.append(self.l1[i])
                    self.col2.append("") #append empty character only if 
                    self.col3.append("")
                else:
                    diff = i - j
                    for n in range(diff):
                        self.col1.append("\t")

    def printColumns(self, o1, o2, o3):
        if o1 == True:
            self.col1 = [''] * len(self.col1)
        if o2 == True:
            self.col2 = [''] * len(self.col2)
        if o3 == True:
            self.col3 = [''] * len(self.col3)
        result = []
        for i in range(len(self.col1)):
            result.append(self.col1[i] + self.col2[i] + self.col3[i])
        for j in range(len(result)):
            if not re.match(r'^\s*$', result[j]): #if the line is not empty (whitespace)
                print(result[j])

def main():
    locale.setlocale(locale.LC_COLLATE, 'C')
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE1 FILE2
    Output the differences and similarities between two files."""

    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-1", action="store_true", dest="c1", default = False, help="suppress printing of column 1")
    parser.add_option("-2", action="store_true", dest="c2", default = False, help="suppress printing of column 2")
    parser.add_option("-3", action="store_true", dest="c3", default = False, help="suppress printing of column 3")
    parser.add_option("-u", action="store_true", dest="unsorted", default = False, help="compare unsorted files")
    options, args = parser.parse_args(sys.argv[1:])


    try:
        c1 = bool(options.c1)
    except:
        parser.error("invalid option for -1: {0}".format(options.c1))
    try:
        c2 = bool(options.c2)
    except:
        parser.error("invalid option for -2: {0}".format(options.c2))
    try:
        c3 = bool(options.c3)
    except:
        parser.error("invalid option for -3: {0}".format(options.c3))
    try:
        unsorted = bool(options.unsorted)
    except:
        parser.error("invalid option -u: {0}".format(options.unsorted))

    if len(args) != 2:
        parser.error("incorrect number of arguments")
    input_file1 = args[0]
    input_file2 = args[1]
    try:
        compare = comm(input_file1, input_file2)
        if unsorted:
            compare.unsorted_comparison()
        else:
            compare.sorted_comparison()
        compare.printColumns(c1, c2, c3)
    except IOError as e:
        errno = e.errno
        strerror = e.strerror
        parser.error("I/O error({0}): {1}".format(errno, strerror))

if __name__ == "__main__":
    main()
