#include "stdafx.h"

#include "CColumn.h"

namespace BasicOCR {

	CColumn::CColumn() : CDocObject() { }

	//CColumn::~CColumn() { }
	CColumn::CColumn(rectangle obj_box) : CDocObject(obj_box) {
		Paragraphs = gcnew List<CParagraph ^>();
	}
	CColumn::CColumn(float documentHeight, rectangle obj_box) : CDocObject() {
		X = obj_box.x0;
		Y = documentHeight - obj_box.y0 - obj_box.height();
		Width = obj_box.width();
		Height = obj_box.height();
		Paragraphs = gcnew List<CParagraph ^>();
	}

	bool CColumn::Contains(CParagraph ^paragraphObj) {
		if (paragraphObj->X >= X && paragraphObj->Width <= Width &&
			paragraphObj->Y >= Y && paragraphObj->Height <= Height)
			return true;
		return false;
	}

}