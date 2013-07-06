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
// File: line-info.h
// Purpose: interface to line-info.cc
// Responsible: mezhirov
// Reviewer:
// Primary Repository:
// Web Sites: www.iupr.org, www.dfki.de


#ifndef h_line_info__
#define h_line_info__

#include "ocropus.h"

namespace ocropus {
    using namespace colib;

    /// Get line information by character segmentation.
    /// Having a true character segmentation is best, but CCs should also work.
    ///
    /// @param intercept Y coordinate of intersection of the baseline with Oy axis.
    /// @param slope Slope of the baseline (dy/dx, i.e. tangent)
    /// @param xheight Height of letters.
    /// @param descender_sink Distance between descender line and baseline (positive)
    /// @param ascender_rise Distance between ascender line and baseline plus x-height
    ///
    /// WARNING: this doesn't work reliably

    bool get_extended_line_info(float &intercept, float &slope,
                                float &xheight, float &descender_sink,
                                float &ascender_rise, intarray &seg) DEPRECATED;

    /// Get line information of a black-and-white line.
    ///
    /// WARNING: this doesn't work reliably

    bool get_extended_line_info_using_ccs(float &intercept, float &slope,
                                          float &xheight, float &descender_sink,
                                          float &ascender_rise, bytearray &img) DEPRECATED;

    bool get_rast_info(float &intercept, float &slope,bytearray &img);

};

#endif
