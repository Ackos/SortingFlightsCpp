#include "ListDisplay.h"
#include <string>
#include <iostream>
#include <stdio.h>

const char ListDisplay::styles[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
									 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                     'R', 'S', 'T', 'U', 'V', 'X', 'Y', 'W' };

/*
	Constructor,

	Instantiates all class attributes that should be instantiated, and
	sets some graphical details.

	@param x -> (int) x coordinate.
	@param y -> (int) y coordinate.
*/
ListDisplay::ListDisplay(int x, int y)
	: Fl_Text_Display(x, y, 400, 500)
{
	txtBuff = new Fl_Text_Buffer();
	styleBuff = new Fl_Text_Buffer();
	style = nullptr;
	// color(Fl_Color(0x45454500)); // @TODO FIX THIS
	buffer(txtBuff);
}

/*
	Destructor.
*/
ListDisplay::~ListDisplay()
{
//	delete txtBuff;
//	delete styleBuff;
	if (style != nullptr)
		delete[] style;
}

/*
	Restart text buffers.
*/
void ListDisplay::clearText()
{
	txtBuff -> text("");
	styleBuff -> text("");
}

/*
	Function which sets text into main text buffer, and creates
	appropriate text which puts into style text buffer, and then if
	highlight array is passed, creates array of styles which is used
	to style original text.

	@param lineNum -> (int) number of lines to be set as text
	@param text -> (const char **) array of strings (creation and deletion handled elsewhere).
	@param highlight -> (const int *) (has default) array of indexes which should
						be highlighted, creation and deletion handled elsewhere.
*/
void ListDisplay::setText(int lineNum, const char** text, const int* highlight)
{
	txtBuff -> text("");
	styleBuff -> text("");

	char* temp;

	for (int i = 0; i < lineNum; i++)
	{
		// write as much characters to style buffer
		int j = 0;
		for (; j < 100; j++)
			if (text[i][j] == '\n')
				break;
		
		temp = new char[j + 2];

		for (int k = 0; k < j; k++)
			temp[k] = ListDisplay::styles[i % 24];		
		temp[j] = '\n';
		temp[j + 1] = '\0';

		txtBuff -> append(text[i]);
		// txtBuff -> append(temp);
		styleBuff -> append(temp);

		delete[] temp;
	}

	if (style != nullptr)
		delete[] style;

	style = new Fl_Text_Display::Style_Table_Entry[lineNum];
	for (int i = 0; i < lineNum; i++)
	{
		style[i].color = (highlight != nullptr ? (i == highlight[0] || i == highlight[1] ? FL_RED : FL_BLACK) : FL_BLACK);
		style[i].font = FL_BOLD_ITALIC; style[i].size = 16; // A - Red
	}

	int style_size = sizeof((*style))* lineNum/sizeof(style[0]);
	highlight_data(styleBuff, style, style_size, 'A', 0, 0);

	redraw();
}