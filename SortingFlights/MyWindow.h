#ifndef _MY_WINDOW_INCLUDED_
#define _MY_WINDOW_INCLUDED_

#include <FL\Fl_Window.H>
#include <FL\Fl_Menu_Bar.H>

#include "ListDisplay.h"
#include "Dialogs.h"

#include "FlightHandler.h"

class MyWindow : public Fl_Window
{
	Fl_Menu_Bar* menu;
	ListDisplay* originalList;
	ListDisplay* progressList;
	ListLabel* originalListLabel;
	ListLabel* progressListLabel;
	
	ListLabel* iterationNumber;
	ListLabel* switchNumber;
	ListLabel* compareNumber;
	ListLabel* iterationCount; char* iterationLabel;
	ListLabel* switchCount; char* switchLabel;
	ListLabel* compareCount; char* compareLabel;

	CustomButton* nextIteration;
	CustomButton* previousIteration;
	CustomButton* skipToFinish;
	CustomButton* loadList;
	CustomButton* sortList;

	int numberOfListElements;

	void addMenu();
public:
	static const int WIDTH = 1200;
	static const int HEIGHT = WIDTH * 9 / 16;
	enum { ORIGINAL_LIST, PROGRESS_LIST };

	// connection interface (external)
	static Fl_Callback* loadListModel_cb;
	static Fl_Callback* sortListModel_cb;
	static Fl_Callback* changeSortAlg_cb;
	static Fl_Callback* changeSortCri_cb;
	static Fl_Callback* changeInputPath_cb;
	static Fl_Callback* changeOutputDir_cb;
	static Fl_Callback* nextIteration_cb;
	static Fl_Callback* previousIteration_cb;
	static Fl_Callback* skipToFinish_cb;
	static Fl_Callback* saveSortData_cb;
	static Fl_Callback* saveSortedList_cb;
	static Fl_Callback* exit_cb;

	// internal signals
	static Fl_Callback* _showProperties_cb;
	static Fl_Callback* _changeInputPath_cb;
	static Fl_Callback* _changeOutputDir_cb;
	static Fl_Callback* _exit_cb;

	// internal handles
	static void showPropertiesHandle(Fl_Widget *, void *);
	static void changeInputHandle(Fl_Widget *, void *);
	static void changeOutputHandle(Fl_Widget *, void *);
	static void exitHandle(Fl_Widget *, void *);

	int handle(int);

	MyWindow(int);
	~MyWindow();

	void showList(const char **, int, const int * = nullptr);
	void clearList(int);
	void updateLabels(int, int, int);
};

#endif