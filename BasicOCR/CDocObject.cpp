#include "stdafx.h"

#include "CDocObject.h"

namespace BasicOCR {

	CDocObject::CDocObject() {
		X = 0;
		Y = 0;
		Width = 0;
		Height = 0;
	};

	CDocObject::CDocObject(rectangle obj_box) {
		X = obj_box.x0;
		Y = obj_box.y0;
		Width = obj_box.width();
		Height = obj_box.height();
	}


}