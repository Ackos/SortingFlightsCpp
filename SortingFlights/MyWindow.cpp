#include "MyWindow.h"

//---------------------------------------------------------------------
// Setting external signals handles to nullptr (must be set externaly)
//---------------------------------------------------------------------

Fl_Callback* MyWindow::loadListModel_cb = nullptr;
Fl_Callback* MyWindow::sortListModel_cb = nullptr;
Fl_Callback* MyWindow::changeSortAlg_cb = nullptr;
Fl_Callback* MyWindow::changeSortCri_cb = nullptr;
Fl_Callback* MyWindow::changeInputPath_cb = nullptr;
Fl_Callback* MyWindow::changeOutputDir_cb = nullptr;
Fl_Callback* MyWindow::nextIteration_cb = nullptr;
Fl_Callback* MyWindow::previousIteration_cb = nullptr;
Fl_Callback* MyWindow::skipToFinish_cb = nullptr;
Fl_Callback* MyWindow::saveSortData_cb = nullptr;
Fl_Callback* MyWindow::saveSortedList_cb = nullptr;
Fl_Callback* MyWindow::exit_cb = nullptr;

//---------------------------------------------------------------------
// END OF setting external signals
//---------------------------------------------------------------------

//-----------------------------------------------------------
// Connecting internal signals on internal handles
//-----------------------------------------------------------

Fl_Callback* MyWindow::_showProperties_cb = (Fl_Callback *) &MyWindow::showPropertiesHandle;
Fl_Callback* MyWindow::_changeInputPath_cb = (Fl_Callback *) &MyWindow::changeInputHandle;
Fl_Callback* MyWindow::_changeOutputDir_cb = (Fl_Callback *) &MyWindow::changeOutputHandle;
Fl_Callback* MyWindow::_exit_cb = (Fl_Callback *) &MyWindow::exitHandle;

//-----------------------------------------------------------
// End of connecting internal signals on internal handles
//-----------------------------------------------------------

/*
	Constructor
*/
MyWindow::MyWindow(int num)
	: Fl_Window(MyWindow::WIDTH, MyWindow::HEIGHT, "Sorting Flights - Project"), numberOfListElements(num)
{
	color(Fl_Color(0x1A1A1A00));
	addMenu();

	begin();
	originalListLabel = new ListLabel(220, 40, "Original List");
	originalList = new ListDisplay(70, 75);
	// originalList -> color(Fl_Color(0x45454500));
	progressListLabel = new ListLabel(WIDTH - 320, 40, "Current Switch");
	progressList = new ListDisplay(WIDTH - 470, 75);

	iterationNumber = new ListLabel(WIDTH / 2 - 50, HEIGHT / 2 - 200, "Iteration number: ");
	iterationCount = new ListLabel(WIDTH / 2 + 50, HEIGHT / 2 - 200, "");
	switchNumber = new ListLabel(WIDTH / 2 - 50, HEIGHT / 2 - 170, "Switch number: ");
	switchCount = new ListLabel(WIDTH / 2 + 50, HEIGHT / 2 - 170, "");
	compareNumber = new ListLabel(WIDTH / 2 - 50, HEIGHT / 2 - 140, "Comparison number: ");
	compareCount = new ListLabel(WIDTH / 2 + 50, HEIGHT / 2 - 140, "");

	iterationLabel = new char[10];
	switchLabel = new char[10];
	compareLabel = new char[10];

	nextIteration = new CustomButton(WIDTH - 230, HEIGHT - 70, 160, 50, "Next Change");
	nextIteration -> callback(nextIteration_cb);
	nextIteration -> shortcut(FL_Right);
	previousIteration = new CustomButton(WIDTH - 460, HEIGHT - 70, 160, 50, "Previous Change");
	previousIteration -> callback(previousIteration_cb);
	previousIteration -> shortcut(FL_Left);

	loadList = new CustomButton(70, HEIGHT - 70, 160, 50, "Load List");
	loadList -> callback(loadListModel_cb);
	sortList = new CustomButton(300, HEIGHT - 70, 160, 50, "Sort List");
	sortList -> callback(sortListModel_cb);

	skipToFinish = new CustomButton(WIDTH / 2 - 80, HEIGHT / 2 - 25, 160, 50, "Skip To Finish");
	skipToFinish -> callback(skipToFinish_cb);
	skipToFinish -> shortcut(FL_End);
	end();

	resizable(*this);
	_set_fullscreen();
}

/*
	Destructor
*/
MyWindow::~MyWindow()
{
//	delete menu;
//	delete originalList;
//	delete originalListLabel;
//	delete progressList;
//	delete progressListLabel;

//	delete iterationNumber;
//	delete switchNumber;
//	delete compareNumber;
//	delete iterationCount;
//	delete switchCount;
//	delete compareCount;

//	delete nextIteration;
//	delete previousIteration;
//	delete skipToFinish;
//	delete loadList;
//	delete sortList;

	delete[] iterationLabel;
	delete[] switchLabel;
	delete[] compareLabel;
}

void MyWindow::showList(const char** text, int list, const int * mark)
{
	if (list == ORIGINAL_LIST)
		originalList -> setText(numberOfListElements, text, mark);
	else
		progressList -> setText(numberOfListElements, text, mark);
}

void MyWindow::clearList(int list)
{
	if (list == ORIGINAL_LIST)
		originalList -> clearText();
	else
		progressList -> clearText();
}

void MyWindow::updateLabels(int iter, int sw, int cmp)
{
	char temp[10];
	for (int i = 0; i < 10; i++)
		temp[i] = '\0';
	_itoa_s(iter, temp, 10);
	for (int i = 0; i < 10; i++)
		iterationLabel[i] = temp[i];

	iterationCount -> label(iterationLabel);

	for (int i = 0; i < 10; i++)
		temp[i] = '\0';
	_itoa_s(sw, temp, 10);
	for (int i = 0; i < 10; i++)
		switchLabel[i] = temp[i];

	switchCount -> label(switchLabel);
		
	for (int i = 0; i < 10; i++)
		temp[i] = '\0';
	_itoa_s(cmp, temp, 10);
	for (int i = 0; i < 10; i++)
		compareLabel[i] = temp[i];

	compareCount -> label(compareLabel);
}

int MyWindow::handle(int _event)
{
	if (_event == FL_KEYDOWN)
	{
		int key = Fl::event_key();
		if (key == FL_Escape || key == FL_ALT + FL_F + 4)
		{
			exitHandle(nullptr, nullptr);
			return 1;
		}
	}
	return Fl_Window::handle(_event);
}

void MyWindow::addMenu()
{
	menu = new Fl_Menu_Bar(0, 0, MyWindow::WIDTH, 25);
	menu -> color(Fl_Color(0x45454500));
	menu -> textfont(Fl_Font(FL_ITALIC));
	menu -> textcolor(FL_WHITE);
	menu -> textsize(16);

	menu -> box(FL_FLAT_BOX);
	menu -> down_box(FL_NO_BOX);
	menu -> down_color(FL_WHITE);

	int *option1 = new int(FlightHandler::DESTINATION_UP), *option2 = new int(FlightHandler::DESTINATION_DOWN),
		*option3 = new int(FlightHandler::FLIGHT_NUMBER_UP), *option4 = new int(FlightHandler::FLIGHT_NUMBER_DOWN),
		*option5 = new int(FlightHandler::GATE_NUMBER_UP), *option6 = new int(FlightHandler::GATE_NUMBER_DOWN),
		*option7 = new int(FlightHandler::FLIGHT_TIME_UP), *option8 = new int(FlightHandler::FLIGHT_TIME_DOWN);

	menu -> add("Options/Sort", 0, sortListModel_cb, nullptr); // external
	menu -> add("Options/Load Data", 0, loadListModel_cb, nullptr); // external
	menu -> add("Options/----------------------------------------", 0, nullptr, nullptr, FL_MENU_INACTIVE);
	menu -> add("Options/Sort Algorithm/Selection Sort\t", 0, changeSortAlg_cb, (void *) option1, FL_MENU_RADIO + FL_MENU_CHECK); // MAKE RADIO
	menu -> add("Options/Sort Algorithm/Quick Sort\t", 0, changeSortAlg_cb, (void *) option2, FL_MENU_RADIO); // MAKE RADIO
	menu -> add("Options/Sort Criteria/Destination Up\t", 0 , changeSortCri_cb, (void *) option1, FL_MENU_RADIO + FL_MENU_CHECK);
	menu -> add("Options/Sort Criteria/Destination Down\t", 0, changeSortCri_cb, (void *) option2, FL_MENU_RADIO);
	menu -> add("Options/Sort Criteria/Flight Number Up\t", 0, changeSortCri_cb, (void *) option3, FL_MENU_RADIO);
	menu -> add("Options/Sort Criteria/Flight Number Down\t", 0, changeSortCri_cb, (void *) option4, FL_MENU_RADIO);
	menu -> add("Options/Sort Criteria/Gate Number Up\t", 0, changeSortCri_cb, (void *) option5, FL_MENU_RADIO);
	menu -> add("Options/Sort Criteria/Gate Number Down\t", 0, changeSortCri_cb, (void *) option6, FL_MENU_RADIO);
	menu -> add("Options/Sort Criteria/Date Up\t", 0, changeSortCri_cb, (void *) option7, FL_MENU_RADIO);
	menu -> add("Options/Sort Criteria/Date Down\t", 0, changeSortCri_cb, (void *) option8, FL_MENU_RADIO);
	menu -> add("Options/-----------------------------------------", 0, nullptr, nullptr, FL_MENU_INACTIVE);
	menu -> add("Options/Change Input File", 0, _changeInputPath_cb, nullptr); // internal!
	menu -> add("Options/Change Output Directory", 0, _changeOutputDir_cb, nullptr); // internal
	menu -> add("Options/Save Sorted List", 0, saveSortedList_cb, nullptr); // external
	menu -> add("Options/Save Sort Data", 0, saveSortData_cb, nullptr); // external
	menu -> add("Options/---------------------------------------", 0, nullptr, nullptr, FL_MENU_INACTIVE);
	menu -> add("Options/Properties", 0, _showProperties_cb, nullptr); // internal
	menu -> add("Quit", FL_ALT + FL_F + 4, _exit_cb, nullptr); // external

	this -> begin();
	this -> add(menu);
	this -> end();
}

//------------------------------------------------------------------------
// INTERNAL HANDLES
//------------------------------------------------------------------------

void MyWindow::showPropertiesHandle(Fl_Widget* w, void* userData)
{
}

void MyWindow::changeInputHandle(Fl_Widget *, void *)
{
	InputDialog* i = InputDialog::getInstance("Enter new input file path: ", changeInputPath_cb);
	i -> showSelf();
}

void MyWindow::changeOutputHandle(Fl_Widget *, void *)
{
	InputDialog* i = InputDialog::getInstance("Enter new output directory path: ", changeOutputDir_cb);
	i -> showSelf();
}

void MyWindow::exitHandle(Fl_Widget *, void *)
{
	QuestionDialog* q = QuestionDialog::getInstance("Are you shore you want to quit?", exit_cb);
	q -> showSelf();
}

//------------------------------------------------------------------------------
// END OF Internal handles
//------------------------------------------------------------------------------