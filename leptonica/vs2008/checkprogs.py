#!/usr/bin/env python

"""
checkprogs.py -- Check that all leptonlib\prog\*.c files are in progs.csv
		 

$RCSfile: checkprogs.py,v $ $Revision: 1.4 $ $Date: 2009/12/06 04:54:40 $
"""

r"""
Requires:

 python 2.5 or greater: activestate.com
  http://activestate.com/Products/Download/Download.plex?id=ActivePython

General Notes:
--------------

Usage:

    python25 checkprogs.py progDir progs.csv

Also generates partialsln.txt. This file can be used to manually edit
leptonlib.sln to make the subfolders in the prog_file Solution Folder
contain the correct lists of files.

Format for a .sln file entry:

	ProjectSection(SolutionItems) = preProject
		..\prog\xformbox_reg.c = ..\prog\xformbox_reg.c
	EndProjectSection

"""

# imports of python standard library modules
# See Python Documentation | Library Reference for details
import csv
import glob
import optparse
import os
import re

# ====================================================================

def dumpCountDict(title, countDict):
    keys = countDict.keys()
    keys.sort()
    total = 0

    print "%s (%d):" % (title, len(countDict))
    print
    for key in keys:
        count = countDict[key]
        print " %3d %s" % (count, key)
        total += count
    print " ---"
    print " %3d" % total
    print

def checkProgs(progDir, progsFilename):
    progPattern = os.path.join(progDir, "*.c")
    progFiles = glob.glob(progPattern)
    progs = set()
    for prog in progFiles:
        head, tail = os.path.split(prog)
        tail = tail.lower()
        progs.add(tail)

    documentedProgs = set()
    typeDict = {}
    categoryDict = {}
    byType = []
    byCategory = []

    f = open(progsFilename, "rb")
    reader = csv.reader(f, dialect="excel")
    reader.next()	#skip header row
    for row in reader:
        filename, progType, category, description, samplefile, needsArgs = row
        filename = filename.lower()
        documentedProgs.add(filename)

        count = typeDict.setdefault(progType, 0)
        count += 1
        typeDict[progType] = count

        count = categoryDict.setdefault(category, 0)
        count += 1
        categoryDict[category] = count

        byType.append( (progType, filename) )
        byCategory.append( (category, filename) )
    f.close()

    vanishedProgs = documentedProgs - progs
    undocumentedProgs = progs - documentedProgs

    print "%4d Total progs in %s" % (len(progs), progDir)
    print "%4d Total documented progs in %s" % (len(documentedProgs), progsFilename)
    print

    print "%4d Vanished progs (in %s but not in %s)" % (len(vanishedProgs), progsFilename, progDir)
    sortedList = list(vanishedProgs)
    sortedList.sort()
    for filename in sortedList:
        print "       %s" % filename
    print

    print "%4d Undocumented progs (in %s but not in %s)" % (len(undocumentedProgs), progDir, progsFilename)
    sortedList = list(undocumentedProgs)
    sortedList.sort()
    for filename in sortedList:
        print "       %s" % filename
    print

    dumpCountDict("Prog Types", typeDict)
    dumpCountDict("Categories", categoryDict)

    #use following generated file to edit contents of leptonlib.sln
    partialSlnFilename = "partialsln.txt"
    print "Generating %s" % partialSlnFilename
    byType.sort()
    byCategory.sort()

    f = open(partialSlnFilename, "w")

    prevType = None
    for progType, filename in byType:
        if progType != prevType:
            if prevType is not None:
                f.write("\tEndProjectSection\n")
            f.write("\n%s:\n\n" % progType)
            f.write("\tProjectSection(SolutionItems) = preProject\n")
            prevType = progType
        f.write("\t\t..\\prog\\%s = ..\\prog\\%s\n" % (filename, filename))
    f.write("\tEndProjectSection\n")
    f.write("\n")
    f.write("%s\n" % ("="*72,))

    prevCategory = None
    for category, filename in byCategory:
        if category != prevCategory:
            if prevCategory is not None:
                f.write("\tEndProjectSection\n")
            f.write("\n%s:\n\n" % category)
            f.write("\tProjectSection(SolutionItems) = preProject\n")
            prevCategory = category
        f.write("\t\t..\\prog\\%s = ..\\prog\\%s\n" % (filename, filename))
    f.write("\tEndProjectSection\n")
    f.close()

# ====================================================================

def main ():
    parser = optparse.OptionParser(usage="%prog [options] progDir progs.csv",
                                   version="%prog $Revision: 1.4 $")

    parser.set_defaults(debugging=0)
    parser.add_option("-d", "--debug", action="store_const", const=1,
                      dest="debugging",
                      help="basic debugging messages", )
    parser.add_option("-D", "--Debug", action="store_const", const=2,
                      dest="debugging",
                      help="Extended debugging messages", )
    options, args = parser.parse_args()

    if len(args) != 2:
        parser.error("incorrect number of arguments")
    progDir = args[0]
    if not os.path.exists(progDir):
        parser.error("%s doesn't exist" % progDir)
    progsFilename = args[1]
    if not os.path.exists(progsFilename):
        parser.error("%s doesn't exist" % progsFilename)

    checkProgs(progDir, progsFilename)

if __name__ == '__main__' :
    main()
