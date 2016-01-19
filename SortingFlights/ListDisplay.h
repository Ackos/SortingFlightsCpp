#ifndef _LIST_DISPLAY_INCLUDED_
#define _LIST_DISPLAY_INCLUDED_

#include <FL\Fl_Text_Display.H>
#include <FL\Fl_Text_Buffer.H>

/*
	Class ListDisplay

	Class designed to be visual representation of model lists.
	It inherits Fl_Text_Display and contains two text buffer and one 
	Style_Table_Entry pointers, which is used for printing text, and for
	highlighting text.

	@author Acko.
*/
class ListDisplay : public Fl_Text_Display
{
private:
	/*
		Text buffer which holds original text to be shown.
	*/
	Fl_Text_Buffer* txtBuff;

	/*
		Text buffer which holds placeholder text for highlighting purposes.
	*/
	Fl_Text_Buffer* styleBuff;

	/*
		Object for styling text (styles placeholder, and that shows up on original text).
	*/
	Fl_Text_Display::Style_Table_Entry* style;

	/*
		Array of character constants for styling. (@TODO can be optimized)
	*/
	static const char styles[];

public:
	/*
		Constructor,

		Instantiates all class attributes that should be instantiated, and
		sets some graphical details.
	*/
	ListDisplay(int, int);

	/*
		Destructor.
	*/
	~ListDisplay();

	/*
		Function which sets text into main text buffer, and creates
		appropriate text which puts into style text buffer, and then if
		highlight array is passed, creates array of styles which is used
		to style original text.
	*/
	void setText(int, const char **, const int * = nullptr);

	/*
		Restart text buffers.
	*/
	void clearText();
};

#endif