#include "Program.h"

// local function (side)
bool dirExists(const std::string& dirName_in)
{
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!

  return false;    // this is not a directory!
}

//------------------------------------------------------------
// Setting default static variables and constants
//------------------------------------------------------------

Program* Program::instance = nullptr;

char* Program::DEFAULT_INPUT_FOLDER = "";
char* Program::DEFAULT_INPUT_FILE = "test.txt";
char* Program::DEFAULT_OUTPUT_FOLDER = "";
char* Program::DEFAULT_OUTPUT_FILE = "output.txt";
char* Program::DEFAULT_OUTPUT_SORT_FILE = "out_sort.txt";
const char* Program::TEMP_FILE = "temp";

int Program::DEFAULT_SORT = Program::SELECTION;
int Program::DEFAULT_CRITERIA = FlightHandler::DESTINATION_UP;

const int Program::FLIGHTS_TO_LOAD = 10;

//--------------------------------------------------------------
// END OF setting default static variables and constants
//--------------------------------------------------------------

/*
	Constructor
*/
Program::Program()
{
	Program::instance = this;

	currentSwitch = 0;
	sorted = false;

	// set default values
	strcpy_s(inputFolder, Program::DEFAULT_INPUT_FOLDER);
	strcpy_s(inputFile, Program::DEFAULT_INPUT_FILE);
	strcpy_s(outputFolder, Program::DEFAULT_OUTPUT_FOLDER);
	strcpy_s(outputFile, Program::DEFAULT_OUTPUT_FILE);
	strcpy_s(outputSortFile, Program::DEFAULT_OUTPUT_SORT_FILE);
	strcpy_s(tempOutput, Program::TEMP_FILE);

	sortType = DEFAULT_SORT;
	sortCriteria = DEFAULT_CRITERIA;

	try
	{
		log = new Log(FLIGHTS_TO_LOAD);
		fh = new FlightHandler(FLIGHTS_TO_LOAD);
		connectView();
		initView();
	}
	catch (std::exception& e)
	{
		error(e.what());
		std::exit(0);
	}
}

/*
	Destructor
*/
Program::~Program()
{
	delete fh;
	delete log;
	destructView();
}

// Singlethon
Program* Program::getInstance()
{
	if (instance == nullptr)
		instance = new Program();
	return instance;
}

void Program::addCommandLineArguments(int argc, char** argv)
{
	try
	{
		if (argc < 2)
			throw std::exception("Bad input file paths. Program will load default paths.");

		char* fileName = argv[1];
		if (*fileName == '.' || *fileName == 'C' || *fileName == 'c' || *fileName == 'D' || *fileName == 'd' || *fileName == '/')
			strcpy_s(inputFile, argv[1]);
		else
		{
			strcpy_s(inputFile, inputFolder);
			strcat_s(inputFile, argv[1]);
		}

		fh -> loadFlights(inputFile);
		showList(0, MyWindow::ORIGINAL_LIST, nullptr);
		updateLabels(0, 0, 0);
	}
	catch (std::exception& e)
	{
		strcpy_s(inputFile, Program::DEFAULT_INPUT_FILE);
		try
		{
			fh -> loadFlights(inputFile);
			showList(0, MyWindow::ORIGINAL_LIST, nullptr);
			updateLabels(0, 0, 0);
			error(e.what());
		}
		catch (std::exception&)
		{
			error("Program can not load entered or default paths. Please change input file before running program");
		}
	}
}

/*
	Main Function

	Displays View, and starts FLTK event loop.
*/
void Program::start()
{
	show();
	Fl::run();
}

//-----------------------------------------------
// View Methods
//-----------------------------------------------

void Program::initView()
{
	win = new MyWindow(FLIGHTS_TO_LOAD);
}

void Program::destructView()
{
	delete win;
}

void Program::error(const char* message)
{
	InfoDialog::getInstance(InfoDialog::_ERROR, message) -> showSelf();
}

void Program::info(const char* message)
{
	InfoDialog::getInstance(InfoDialog::INFO, message) -> showSelf();
}

void Program::show()
{
	win -> show();
}

void Program::showList(int index, int list, const int* mark)
{
	if (sorted)
		win -> showList(fh -> getFlightsString(log -> getData(index)), list, mark);
	else
		win -> showList(fh -> getFlightsString(), list, mark);
}

bool Program::isLoaded()
{
	return fh -> checkLoaded();
}

void Program::updateLabels(int iter, int sw, int cmp)
{
	this -> win -> updateLabels(iter, sw, cmp);
}

void Program::connectView()
{
	MyWindow::exit_cb = (Fl_Callback *) &Program::exitHandle;

	MyWindow::nextIteration_cb = (Fl_Callback *) &Program::nextIterationHandle;
	MyWindow::previousIteration_cb = (Fl_Callback *) &Program::previousIterationHandle;
	MyWindow::skipToFinish_cb = (Fl_Callback *) &Program::skipToFinishHandle;
	MyWindow::saveSortedList_cb = (Fl_Callback *) &Program::saveSortedListHandle;
	MyWindow::saveSortData_cb = (Fl_Callback *) &Program::saveSortDataHandle;

	MyWindow::loadListModel_cb = (Fl_Callback *) &Program::loadListModelHandle;
	MyWindow::sortListModel_cb = (Fl_Callback *) &Program::sortListModelHandle;
	MyWindow::changeSortAlg_cb = (Fl_Callback *) &Program::changeSortAlgHandle;
	MyWindow::changeSortCri_cb = (Fl_Callback *) &Program::changeSortCriHandle;
	MyWindow::changeInputPath_cb = (Fl_Callback *) &Program::changeInputPathHandle;
	MyWindow::changeOutputDir_cb = (Fl_Callback *) &Program::changeOutputDirHandle;
}

//-----------------------------------------------
// END OF View Methods
//-----------------------------------------------

//-------------------------------------------------------------------------
// DEFINING HANDLES
//-------------------------------------------------------------------------

void Program::exitHandle(Fl_Widget* w, void* userData)
{
	if ((* (int *) userData) == QuestionDialog::YES)
		std::exit(0);
}

void Program::nextIterationHandle(Fl_Widget *, void *)
{
	Program* p = Program::getInstance();
	if (p -> isLoaded() && p -> sorted)
	{
		if (p -> currentSwitch != p -> log -> size() - 1)
		{
			p -> currentSwitch++;
			const int * mark = p -> log -> getDifference(p -> currentSwitch, p -> currentSwitch - 1); // log does memory clean-up
			p -> showList(p -> currentSwitch, MyWindow::PROGRESS_LIST, mark);
			p -> updateLabels(p -> log -> getIterNumber(p -> currentSwitch), p -> log -> getSwitchNumber(p -> currentSwitch),
								p -> log -> getCompareNumber(p -> currentSwitch));
		}
		else
		{
			p -> showList(p -> currentSwitch, MyWindow::PROGRESS_LIST, nullptr);
			p -> info("No more iterations. List sorted!");
		}
	}
	else
		p -> error("You must first load and sort data!");
}

void Program::previousIterationHandle(Fl_Widget *, void *)
{
	Program* p = Program::getInstance();
	if (p -> isLoaded() && p -> sorted)
	{
		if (p -> currentSwitch > 0)
		{
			p -> currentSwitch--;
			const int * mark = p -> log -> getDifference(p -> currentSwitch, p -> currentSwitch + 1); // log does memory clean-up
			p -> showList(p -> currentSwitch, MyWindow::PROGRESS_LIST, mark);
			p -> updateLabels(p -> log -> getIterNumber(p -> currentSwitch), p -> log -> getSwitchNumber(p -> currentSwitch),
								p -> log -> getCompareNumber(p -> currentSwitch));
		}
		else
		{
			p -> showList(p -> currentSwitch, MyWindow::PROGRESS_LIST, nullptr);
			p -> info("First iteration, can't go back!");
		}
	}
	else
		p -> error("You must first load and sort data!");

}

void Program::skipToFinishHandle(Fl_Widget *, void *)
{
	Program* p = Program::getInstance();
	if (p -> isLoaded() && p -> sorted)
	{
		p -> currentSwitch = p -> log -> size() - 1;
		p -> showList(p -> currentSwitch, MyWindow::PROGRESS_LIST, nullptr);
		p -> updateLabels(p -> log -> getIterNumber(p -> currentSwitch), p -> log -> getSwitchNumber(p -> currentSwitch),
							p -> log -> getCompareNumber(p -> currentSwitch));
	}
	else
		p -> error("You must first load and sort data!");
}

void Program::saveSortedListHandle(Fl_Widget *, void *)
{
	try
	{
		Program::getInstance() -> fh -> writeFlights(Program::getInstance() -> outputFile,
														Program::getInstance() -> log -> getData(Program::getInstance() -> log -> size() - 1));
	}
	catch (std::exception&)
	{
		Program::getInstance() -> error("Unable to save sorted list.\nMake sure that output directory is valid.");
	}
}

void Program::saveSortDataHandle(Fl_Widget *, void *)
{
	try
	{
		Program::getInstance() -> log -> writeData(Program::getInstance() -> outputSortFile);
	}
	catch (std::exception&)
	{
		Program::getInstance() -> error("Unable to save sort data.");
	}
}

void Program::loadListModelHandle(Fl_Widget *, void* data)
{
	try
	{
		Program::getInstance() -> sorted = false;
		Program::getInstance() -> win -> clearList(MyWindow::ORIGINAL_LIST);
		Program::getInstance() -> win -> clearList(MyWindow::PROGRESS_LIST);
		Program::getInstance() -> updateLabels(0, 0, 0);
		Program::getInstance() -> fh -> loadFlights(Program::getInstance() -> inputFile);
		Program::getInstance() -> showList(0, MyWindow::ORIGINAL_LIST, nullptr);
	}
	catch (std::exception&)
	{
		Program::getInstance() -> error("Unable to load file. Please choose different file.");
		Program::getInstance() -> win -> clearList(MyWindow::ORIGINAL_LIST);
	}
}

void Program::sortListModelHandle(Fl_Widget *, void* userData)
{
	Program* p = Program::getInstance();
	if (p -> isLoaded())
	{
		try
		{
			p -> log -> clearEntries();

			void (* sort) (Flight**, size_t, bool (*) (const Flight&, const Flight&), Log *);
			bool (* compare) (const Flight&, const Flight&);

			if (p -> sortType == SELECTION)
				sort = &Sorter::selectionSort;
			else
				sort = &Sorter::quickSort;

			switch (p -> sortCriteria)
			{
			case FlightHandler::DESTINATION_UP:
				compare = &FlightHandler::destinationUp;
				break;
			case FlightHandler::DESTINATION_DOWN:
				compare = &FlightHandler::destinationDown;
				break;
			case FlightHandler::FLIGHT_NUMBER_UP:
				compare = &FlightHandler::flightNumberUp;
				break;
			case FlightHandler::FLIGHT_NUMBER_DOWN:
				compare = &FlightHandler::flightNumberDown;
				break;
			case FlightHandler::FLIGHT_TIME_UP:
				compare = &FlightHandler::flightTimeUp;
				break;
			case FlightHandler::FLIGHT_TIME_DOWN:
				compare = &FlightHandler::flightTimeDown;
				break;
			case FlightHandler::GATE_NUMBER_UP:
				compare = &FlightHandler::gateNumberUp;
				break;
			case FlightHandler::GATE_NUMBER_DOWN:
				compare = &FlightHandler::gateNumberDown;
				break;
			default:
				compare = nullptr;
				break;
			}

			Flight** copyList = p -> fh -> copyList();

			sort(copyList, FLIGHTS_TO_LOAD, compare, p -> log);
			p -> sorted = true;
			p -> currentSwitch = 0;
			p -> showList(p -> currentSwitch, MyWindow::PROGRESS_LIST);
			p -> updateLabels(p -> log -> getIterNumber(p -> currentSwitch), p -> log -> getSwitchNumber(p -> currentSwitch),
								p -> log -> getCompareNumber(p -> currentSwitch));

			p -> fh -> destroyList(copyList);
		}
		catch (std::exception&)
		{
			p -> error("Unable to sort list.");
		}
	}
	else
		p -> error("You must first load data from file.");
}

void Program::changeSortAlgHandle(Fl_Widget *, void* data)
{
	Program* p = Program::getInstance();
	int* option = (int *) data;
	p -> sortType = *option;
}

void Program::changeSortCriHandle(Fl_Widget *, void* data)
{
	Program* p = Program::getInstance();
	int* option = (int *) data;
	p -> sortCriteria = *option;
	std::cout << "SORT: " << p -> sortCriteria << std::endl;
}

void Program::changeInputPathHandle(Fl_Widget* w, void* data)
{
	int status = * (int *) data;
	if (status == InputDialog::OK)
	{
		Program* p = Program::getInstance();
		InputDialog* input = (InputDialog *) w;
		strcpy_s(p -> inputFile, input -> text);
		InfoDialog::getInstance(InfoDialog::INFO, "Input file path changed.\nSelect load data so that change have any effect") -> showSelf();
	}
}

void Program::changeOutputDirHandle(Fl_Widget* w, void* data)
{
	int status = * (int *) data;
	if (status == InputDialog::OK)
	{
		Program* p = Program::getInstance();
		InputDialog* input = (InputDialog *) w;
		strcpy_s(p -> outputFolder, input -> text);
		// if directory doesn't exist, show error, and return folder to default.
		if (!dirExists(std::string(p -> outputFolder)))
		{
			// error handling
			InfoDialog::getInstance(InfoDialog::_ERROR, "Directory you entered does not exist. Please try another one.") -> showSelf();
			strcpy_s(p -> outputFolder, Program::DEFAULT_OUTPUT_FOLDER);
			return;
		}
		// change output files
		int pos = 0;
		do
		{
			pos++;
		} while (p -> outputFolder[pos] != '\0');

		if (p -> outputFolder[pos - 1] != '/' && p -> outputFolder[pos - 1] != '\\')
		{
			p -> outputFolder[pos] = '/';
			p -> outputFolder[pos + 1] = '\0';
		}

		strcpy_s(p -> outputFile, p -> outputFolder);
		strcpy_s(p -> outputSortFile, p -> outputFolder);
		strcat_s(p -> outputFile, Program::DEFAULT_OUTPUT_FILE);
		strcat_s(p -> outputSortFile, Program::DEFAULT_OUTPUT_SORT_FILE);

		InfoDialog::getInstance(InfoDialog::INFO, "Output directory changed.\nSelect \"Save sorted list\" or \"Save sort data\" so that it have any effect") -> showSelf();
	}
}
//-----------------------------------------------------------------
// END OF defining handles.
//-----------------------------------------------------------------