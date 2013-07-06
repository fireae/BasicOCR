
#include "ocropus.h"
#include <cctype>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "colib/colib.h"
#ifdef _WIN32
#include "imglib/iulib.h"
#include <direct.h>
#else#
#include "iulib/iulib.h"
#include <glob.h>
#endif

#include "glinerec.h"