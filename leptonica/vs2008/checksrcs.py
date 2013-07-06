#!/usr/bin/env python

"""
checksrcs.py -- Check that all leptonlib\src\*.c & *.h files are in srcs.csv
	       and leptonica.vcproj

$RCSfile: checksrcs.py,v $ $Revision: 1.2 $ $Date: 2009/12/08 21:14:05 $
"""

r"""
Requires:

 python 2.5 or greater: activestate.com
  http://activestate.com/Products/Download/Download.plex?id=ActivePython

General Notes:
--------------

Usage:

    python25 checksrcs.py srcDir slnDir srcs.csv

Format for a .vcproj file entry:

			<File
				RelativePath="..\src\allheaders.h"
				>
			</File>

"""

# imports of python standard library modules
# See Python Documentation | Library Reference for details
import csv
import glob
import optparse
import os
import re

# ====================================================================

VCPROJ_NAME = "leptonlib.vcproj"

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

def checkSrcs(srcDir, vcprojFilename, srcsFilename):
    cPattern = os.path.join(srcDir, "*.c")
    cFileList = glob.glob(cPattern)
    cFiles = set()
    for cFile in cFileList:
        head, tail = os.path.split(cFile)
        tail = tail.lower()
        cFiles.add(tail)

    hPattern = os.path.join(srcDir, "*.h")
    hFileList = glob.glob(hPattern)
    hFiles = set()
    for hFile in hFileList:
        head, tail = os.path.split(hFile)
        tail = tail.lower()
        hFiles.add(tail)

    documentedCFiles = set()
    documentedHFiles = set()
    categoryDict = {}
    byCategory = []

    f = open(srcsFilename, "rb")
    reader = csv.reader(f, dialect="excel")
    reader.next()	#skip header row
    for row in reader:
        filename, category, description = row
        filename = filename.lower()
        root, ext = os.path.splitext(filename)
        if ext == ".c":
            documentedCFiles.add(filename)
        else:
            documentedHFiles.add(filename)

        count = categoryDict.setdefault(category, 0)
        count += 1
        categoryDict[category] = count

        byCategory.append( (category, filename) )
    f.close()

    vanishedCFiles = documentedCFiles - cFiles
    undocumentedCFiles = cFiles - documentedCFiles

    print "%4d Total c files in %s" % (len(cFiles), srcDir)
    print "%4d Total documented c files in %s" % (len(documentedCFiles), srcsFilename)
    print

    print "%4d Vanished c files (in %s but not in %s)" % (len(vanishedCFiles), srcsFilename, srcDir)
    sortedList = list(vanishedCFiles)
    sortedList.sort()
    for filename in sortedList:
        print "       %s" % filename
    print

    print "%4d Undocumented c files (in %s but not in %s)" % (len(undocumentedCFiles), srcDir, srcsFilename)
    sortedList = list(undocumentedCFiles)
    sortedList.sort()
    for filename in sortedList:
        print "       %s" % filename
    print

    vanishedHFiles = documentedHFiles - hFiles
    undocumentedHFiles = hFiles - documentedHFiles

    print "%4d Total h files in %s" % (len(hFiles), srcDir)
    print "%4d Total documented h files in %s" % (len(documentedHFiles), srcsFilename)
    print

    print "%4d Vanished h files (in %s but not in %s)" % (len(vanishedHFiles), srcsFilename, srcDir)
    sortedList = list(vanishedHFiles)
    sortedList.sort()
    for filename in sortedList:
        print "       %s" % filename
    print

    print "%4d Undocumented h files (in %s but not in %s)" % (len(undocumentedHFiles), srcDir, srcsFilename)
    sortedList = list(undocumentedHFiles)
    sortedList.sort()
    for filename in sortedList:
        print "       %s" % filename
    print

    dumpCountDict("Categories", categoryDict)

    print "%s\n" % ("="*72,)

    #now check the files in the VS2008 project
    f = open(vcprojFilename, "r")
    data = f.read()
    f.close()

    projectFiles = re.findall(r'(?i)RelativePath="\.\.\\src\\([^"]+)"', data)
    projectCFiles = set()
    projectHFiles = set()
    for projectFile in projectFiles:
        projectFile = projectFile.lower()
        root, ext = os.path.splitext(projectFile)
        if ext == ".c":
            projectCFiles.add(projectFile)
        else:
            projectHFiles.add(projectFile)

    documentedProjectCFiles = documentedCFiles & projectCFiles
    vanishedProjectCFiles = documentedCFiles - projectCFiles
    undocumentedProjectCFiles = projectCFiles - documentedCFiles
    missingProjectCFiles = cFiles - projectCFiles

    print "%4d Total c project files in %s" % (len(projectCFiles), vcprojFilename)
    print "%4d Total documented c project files in %s" % (len(documentedProjectCFiles), vcprojFilename)
    print

    print "%4d Vanished c project files (in %s but not in %s)" % (len(vanishedProjectCFiles), srcsFilename, vcprojFilename)
    sortedList = list(vanishedProjectCFiles)
    sortedList.sort()
    for filename in sortedList:
        print "       %s" % filename
    print

    print "%4d Undocumented c project files (in %s but not in %s)" % (len(undocumentedProjectCFiles), vcprojFilename, srcsFilename)
    sortedList = list(undocumentedProjectCFiles)
    sortedList.sort()
    for filename in sortedList:
        print "       %s" % filename
    print

    print "%4d Missing c project files (in %s but not in %s)" % (len(missingProjectCFiles), srcDir, vcprojFilename)
    sortedList = list(missingProjectCFiles)
    sortedList.sort()
    for filename in sortedList:
        print "       %s" % filename
    print
    print "%s\n" % ("-"*72,)

    documentedProjectHFiles = documentedHFiles & projectHFiles
    vanishedProjectHFiles = documentedHFiles - projectHFiles
    undocumentedProjectHFiles = projectHFiles - documentedHFiles
    missingProjectHFiles = hFiles - projectHFiles

    print "%4d Total .h project files in %s" % (len(projectHFiles), vcprojFilename)
    print "%4d Total documented .h project files in %s" % (len(documentedProjectHFiles), vcprojFilename)
    print

    print "%4d Vanished .h project files (in %s but not in %s)" % (len(vanishedProjectHFiles), srcsFilename, vcprojFilename)
    sortedList = list(vanishedProjectHFiles)
    sortedList.sort()
    for filename in sortedList:
        print "       %s" % filename
    print

    print "%4d Undocumented .h project files (in %s but not in %s)" % (len(undocumentedProjectHFiles), vcprojFilename, srcsFilename)
    sortedList = list(undocumentedProjectHFiles)
    sortedList.sort()
    for filename in sortedList:
        print "       %s" % filename
    print

    print "%4d Missing .h project files (in %s but not in %s)" % (len(missingProjectHFiles), srcDir, vcprojFilename)
    sortedList = list(missingProjectHFiles)
    sortedList.sort()
    for filename in sortedList:
        print "       %s" % filename

# ====================================================================

def main ():
    parser = optparse.OptionParser(usage="%prog [options] srcDir slnDir srcs.csv",
                                   version="%prog $Revision: 1.2 $")

    parser.set_defaults(debugging=0)
    parser.add_option("-d", "--debug", action="store_const", const=1,
                      dest="debugging",
                      help="basic debugging messages", )
    parser.add_option("-D", "--Debug", action="store_const", const=2,
                      dest="debugging",
                      help="Extended debugging messages", )
    options, args = parser.parse_args()

    if len(args) != 3:
        parser.error("incorrect number of arguments")

    srcDir = args[0]
    if not os.path.exists(srcDir):
        parser.error("%s doesn't exist" % srcDir)

    slnDir = args[1]
    vcprojFilename = os.path.join(slnDir, VCPROJ_NAME)
    if not os.path.exists(vcprojFilename):
        parser.error("%s doesn't exist" % vcprojFilename)

    srcsFilename = args[2]
    if not os.path.exists(srcsFilename):
        parser.error("%s doesn't exist" % srcsFilename)

    checkSrcs(srcDir, vcprojFilename, srcsFilename)

if __name__ == '__main__' :
    main()
