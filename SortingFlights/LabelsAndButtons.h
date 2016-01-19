#ifndef _LABELS_AND_BUTTONS_INCLUDED_
#define _LABELS_AND_BUTTONS_INCLUDED_

#include <FL\Fl_Button.H>
#include <FL\Fl_Box.H>

//--------------------------------------------------------------
// ListLabel class declaration
//--------------------------------------------------------------

/*
	Class ListLabel

	Class designed to represent text label, it inherits Fl_Box,
	so it is acctually a Box widget but, it only purpose is to show label.

	@author Acko.
*/
class ListLabel : public Fl_Box
{
public:
	/*
		Constructor, sets all graphical details up, so that it just show text.
	*/
	ListLabel(int, int, const char *);

	/*
		Destructor, Empty.
	*/
	~ListLabel();
};

//--------------------------------------------------------------
// END OF ListLabel class declaration
//--------------------------------------------------------------

//--------------------------------------------------------------
// CustomButton class declaration
//--------------------------------------------------------------

/*
	Class CustomButton

	Class designed to represent one button, it inherits Fl_Button.

	@author Acko.
*/
class CustomButton : public Fl_Button
{
public:
	/*
		Constructor, sets all graphical details up so that button is showed.
	*/
	CustomButton(int, int, int, int, const char *);

	/*
		Destructor, empty.
	*/
	~CustomButton();
};

//--------------------------------------------------------------
// END OF CustomButton class declaration
//--------------------------------------------------------------


#endif