#ifndef _PROGRAM_INCLUDED_
#define _PROGRAM_INCLUDED_

#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include <FL\Fl.H>
#include <FL\fl_ask.H>
#include "FlightHandler.h"
#include "MyWindow.h"
#include "Dialogs.h"

class Program
{
private:
	static char* DEFAULT_INPUT_FOLDER;
	static char* DEFAULT_INPUT_FILE;
	static char* DEFAULT_OUTPUT_FOLDER;
	static char* DEFAULT_OUTPUT_FILE;
	static char* DEFAULT_OUTPUT_SORT_FILE;
	static const char* TEMP_FILE;
	static int DEFAULT_SORT;
	static int DEFAULT_CRITERIA;

	static const int MAX_STRING_LENGTH = 200;
	static Program* instance;

	FlightHandler* fh; // model
	Log* log; // Model View connection
	MyWindow* win; // View data

	int currentSwitch;
	bool sorted;

	void connectView();
	Program();
	
public:
	enum { SELECTION = 1, QUICK };

	static Program* getInstance();

	static const int FLIGHTS_TO_LOAD;

	char inputFolder[MAX_STRING_LENGTH];
	char inputFile[MAX_STRING_LENGTH];

	char outputFolder[MAX_STRING_LENGTH];
	char outputFile[MAX_STRING_LENGTH];
	char outputSortFile[MAX_STRING_LENGTH];

	char tempOutput[MAX_STRING_LENGTH];

	int sortType;
	int sortCriteria;

	~Program();

	void setInputFile(const char *);
	void setOutputFolder(const char *);
	void setOutputFile(const char *);
	void setOutputSortFile(const char *);

	bool isLoaded();

	// View methods
	void initView(); // win = new MyWindow();
	void destructView(); // delete win;

	void error(const char *); // std::cout << std::string(message);
	void info(const char *);
	void showList(int, int = 0, const int * = nullptr);
	void updateLabels(int, int, int);
	void show(); // win -> show();
	// end of view methods

	void addCommandLineArguments(int, char**);
	void start();

	// Handles for View signals
	static void exitHandle(Fl_Widget *, void *);
	static void nextIterationHandle(Fl_Widget *, void *);
	static void previousIterationHandle(Fl_Widget *, void *);
	static void skipToFinishHandle(Fl_Widget *, void *);
	static void saveSortedListHandle(Fl_Widget *, void *);
	static void saveSortDataHandle(Fl_Widget *, void *);

	static void loadListModelHandle(Fl_Widget *, void *);
	static void sortListModelHandle(Fl_Widget *, void *);
	static void changeSortAlgHandle(Fl_Widget *, void *);
	static void changeSortCriHandle(Fl_Widget *, void *);
	static void changeInputPathHandle(Fl_Widget *, void *);
	static void changeOutputDirHandle(Fl_Widget *, void *);

};

#endif