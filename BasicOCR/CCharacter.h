// CCharacter.h

#pragma once

#include "colib.h"

#include "CDocObject.h"

//using namespace System;
//using namespace colib;

namespace BasicOCR {

	[XmlRootAttribute("Character")]
	public ref class CCharacter : public CDocObject
	{
		public:
			CCharacter();
			CCharacter(rectangle obj_box);
			CCharacter(float documentHeight, rectangle obj_box, float char_cost, int char_ascii);
			CCharacter(rectangle obj_box, float char_cost, int char_ascii);
			CCharacter(rectangle obj_box, Double confidence, Char character, int fontindex, int pointsize, int formatting);			
			//~CCharacter();

			Char^ Value;//Gary - Redundant variables
			int FontIndex, PointSize, Formatting;
			int Left, Right, Top, Bottom;
			Double Confidence;
			Object^ Tag;

			//~CCharacter();
			//[Category("Contents")]
			property float Cost;
			//property Char ^Letter;
			//[Category("Contents")]
			property String ^Letter;
		private:

	};
}