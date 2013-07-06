// -*- C++ -*-

// Copyright 2006 Deutsches Forschungszentrum fuer Kuenstliche Intelligenz 
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
// Project: iulib -- image understanding library
// File: io_png.h
// Purpose: interface to io_png.cc
// Responsible: tmb
// Reviewer: 
// Primary Repository: 
// Web Sites: www.iupr.org, www.dfki.de

#ifndef h_png__
#define h_png__

#include "colib/colib.h"

namespace iulib {
    void read_png(colib::bytearray &image, FILE *stream, bool gray=false);
    void write_png(FILE *stream, colib::bytearray &image);
    void read_png_packed(colib::intarray &image, FILE *stream, bool gray=false);
    void write_png_packed(FILE *stream, colib::intarray &image);
}

#endif
