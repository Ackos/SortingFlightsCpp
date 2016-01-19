#include "LabelsAndButtons.h"

//--------------------------------------------------------------
// ListLabel class definition
//--------------------------------------------------------------

/*
	Constructor, sets all graphical details up, so that it just show text.

	@param x -> (int) x coordinate.
	@param y -> (int) y coordinate.
	@param title -> (const char *) text to be shown.
*/
ListLabel::ListLabel(int x, int y, const char* title)
	: Fl_Box(x, y, 100, 25, title)
{
	box(FL_NO_BOX);
	labelsize(18);
	labelfont(FL_BOLD_ITALIC);
	labelcolor(FL_WHITE);
	align(FL_ALIGN_CENTER);
}

/*
	Destructor, empty.
*/
ListLabel::~ListLabel()
{
}

//--------------------------------------------------------------
// END OF ListLabel class definition
//--------------------------------------------------------------

//--------------------------------------------------------------
// CustomButton class definition
//--------------------------------------------------------------


/*
	Constructor, sets all graphical details up so that button is showed.

	@param x -> (int) x coordinate.
	@param y -> (int) y coordinate.
	@param w -> (int) width of a button.
	@param h -> (int) height of a button.
	@param title -> (const char *) button text.
*/
CustomButton::CustomButton(int x, int y, int w, int h, const char * title)
	: Fl_Button(x, y, w, h, title)
{
	box(FL_ROUND_UP_BOX);
	down_box(FL_ROUND_DOWN_BOX);
	labelcolor(FL_WHITE);
	color(Fl_Color(0x45454500));
	labelsize(16);
	labelfont(FL_BOLD_ITALIC);
	align(FL_ALIGN_CENTER);
}

/*
	Destructor, empty
*/
CustomButton::~CustomButton()
{
}

//--------------------------------------------------------------
// END OF CustomButton class definition
//--------------------------------------------------------------
