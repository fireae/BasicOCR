// -*- C++ -*-

// Copyright 2006-2008 Deutsches Forschungszentrum fuer Kuenstliche Intelligenz
// or its licensors, as applicable.
//
// You may not use this file except under the terms of the accompanying license.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may
// obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Project: OCRopus
// File: main-ocr-binarize-otsu.cc
// Purpose: An implementation of Otsu's global thresholding method
// Responsible: Faisal Shafait (faisal.shafait@dfki.de)
// Reviewer:
// Primary Repository:
// Web Sites: www.iupr.org, www.dfki.de

#include "colib/colib.h"
#include "ocropus.h"

using namespace ocropus;
using namespace colib;
using namespace iulib;

int main(int argc,char **argv) {
    try {
        if(argc != 3) {
            fprintf(stderr,"usage: ... input output\n");
            exit(1);
        }
        bytearray image;
        read_png(image,stdio(argv[1],"r"),true);
        autodel<IBinarize> binarize(make_BinarizeByOtsu());
        floatarray in;
        copy(in,image);
        bytearray out;
        makelike(out,in);
        binarize->binarize(out,in);
        write_png(stdio(argv[2],"w"),out);
    }
    catch(const char *oops) {
        fprintf(stderr,"oops: %s\n",oops);
    }
}

