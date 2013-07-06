// This is the main DLL file.

#include "stdafx.h"

#include "BasicOCR.h"

namespace BasicOCR {

	BasicOCR::BasicOCR() {
		init_ocropus_components();
		init_glclass();
		init_glfmaps();
		init_linerec();	
	};
}
