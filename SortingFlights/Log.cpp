#include "Log.h"

/*
	Constructor,

	Sets number of integer objects inside each Entry. And allocates memory for difference array.

	@param numberOfElementsInEntry -> self described.
*/
Log::Log(int numberOfElementsInEntry)
	: numberOfElementsInEntry(numberOfElementsInEntry)
{
	difference = new int[2];
}

/*
	Destructor,

	Cleans up after Log instance, deletes all dinamycally allocated structures.
	(each Entry object and difference array).
*/
Log::~Log()
{
	delete[] difference;
	clearEntries();
}

/*
	Function which takes path to file as a parameter and tryies to write inside that file.

	If it can not write inside file, it will throw an error. Also it relies on Entry::toString()
	for writing.
	
	@param filePath -> (const char *) path to file in which it should write data.
*/
void Log::writeData(const char* filePath)
{
	std::ofstream ofs(filePath);
	try
	{
		if (!ofs)
			throw std::exception("File unavailable!");

		for (std::vector<Entry*>::iterator it = entries.begin(); it != entries.end(); ++it)
		{
			ofs << (*it) -> toString() << "\n";
			if (ofs.fail())
				throw std::exception("Writing into file failed!");
		}
	}
	catch(std::exception& e)
	{
		ofs.close();
		throw e;
	}

	ofs.close();
}

/*
	Function which takes path to file as a parameter and tryies to read data from it.

	It will read data from file and create one Entry instance per line in file if file is valid.
	If it fails reading, it will throw error.

	@param filePath -> (const char *) path to file from which data should be read.

	@note this function isn't used in this project.
*/
void Log::readData(const char* filePath)
{
	std::ifstream ifs(filePath);
	if (!ifs)
		throw std::exception("File unavailable!");

	Entry* temp;

	do
	{
		temp = new Entry(numberOfElementsInEntry);
		try
		{
			loadEntry(ifs, *temp);
			entries.push_back(temp);
		}
		catch (const std::exception&)
		{
		}
	} while (!ifs.fail());

	delete temp; // last created Entry
	ifs.close();
}

/*
	Public interface for Entry class. 

	This function takes all arguments necessary for creating one Entry instance, and then, creates
	one instance on heap, and store it's pointer in vector of entries.

	Entry constructor will create copy of data sent to it, so first parameter can be const int *,
	those data will stay untouched, and can be deleted after calling createEntry().

	@param data -> (const int *) pointer to an array of indexes, which will be copied inside Entry object
	@param iterNum -> (int) number of iteration
	@param switchNum -> (int) number of switches
	@param compareNum -> (int) number of compares.
*/
void Log::createEntry(const int* data, int iterNum, int switchNum, int compareNum)
{
	Entry* e = new Entry(numberOfElementsInEntry, data, iterNum, switchNum, compareNum);
	entries.push_back(e);
}

/*
	Getter method for integer array of indexes inside i-th Entry stored in Log instance.

	@param i -> (int) index of Entry which should be read.

	@returns (const int *) pointer to an array of indexes in i-th Entry, this is dynamically allocated,
				and that is handled by Entry class internally, so it shouldn't be changed outside.

	@note Do NOT change array which is passed back to you.
*/
const int* Log::getData(int i) const
{
	return entries[i] -> getData();
}

/*
	Getter method for iteration number inside i-th Entry stored in Log instance.

	@param i -> (int) index of Entry which should be read.

	@returns (int) iteration number of i-th Entry in Log.
*/
int Log::getIterNumber(int i) const
{
	return entries[i] -> getIterNumber();
}

/*
	Getter method for switch number inside i-th Entry stored in Log instance.

	@param i -> (int) index of Entry which should be read.

	@returns (int) switch number of i-th Entry in Log.
*/
int Log::getSwitchNumber(int i) const
{
	return entries[i] -> getSwitchNumber();
}

/*
	Getter method for compare number inside i-th Entry stored in Log instance.

	@param i -> (int) index of Entry which should be read.

	@returns (int) compare number of i-th Entry in Log.
*/
int Log::getCompareNumber(int i) const
{
	return entries[i] -> getCompareNumber();
}

/*
	Fills instances difference array, and returns pointer to it.

	Managing memory for this array is handled internally should not be redefined outside.

	@param x -> (int) index of first Entry whichs data should be compared.
	@param y -> (int) index of second Entry which data should be compared.

	@returns (const int *) pointer to an array which contains indexes on which
				x-th and y-th Entry object data differs.
*/
const int* Log::getDifference(int x, int y)
{
	const int* datax = entries[x] -> getData();
	const int* datay = entries[y] -> getData();

	int curr = 0;

	for (int i = 0; i < numberOfElementsInEntry; i++)
		if (datax[i] != datay[i])
			difference[curr++] = i;

	return difference;
}

/*
	Function which goes through vector of Entry objects and deletes each one of them, and clears vector.
*/
void Log::clearEntries()
{
	for (std::vector<Entry*>::iterator it = entries.begin(); it != entries.end(); ++it)
		delete *it;
	entries.clear();
}

/*
	Function that returns number of Entry elements stored inside Log instance.

	@returns number of Entry objects inside Log instance.
*/
int Log::size()
{
	return entries.size();
}

/*
	Private method which loads one entry,

	It is instead of overloading operator>>() because Entry class is private.
	It uses StreamReader namespace for stream manipulations, it ensures that file structure is
	valid.

	File structure must be:
		listOfIndexes;iterationNumber;switchNumber;compareNumber\n

	listOfIndexes should be list of integers divided by empty places ("Space").

	@param _in -> (std::istream&) stream from which Entry object is loaded.
	@param _e -> (Entry&) Entry instance which will be populated.
*/
void Log::loadEntry(std::istream& _in, Entry& _e)
{
	int iterNumber = 0, switchNumber = 0, compareNumber;

	char* buffer = new char[numberOfElementsInEntry + 1];
	for (int i = 0; i < numberOfElementsInEntry; i++)
		buffer[i] = 'i';
	buffer[numberOfElementsInEntry] = '\0';

	StreamReader::InputType it(buffer);
	delete[] buffer;
	StreamReader::UserObject* uo = StreamReader::readStream(_in, it);

	int* data = new int[numberOfElementsInEntry];
	for (int i = 0; i < numberOfElementsInEntry; i++)
		data[i] = * (int *) uo -> data[i];

	StreamReader::clearStream(_in, ';');

	delete uo;

	try
	{
		it = StreamReader::InputType("i");
		uo = StreamReader::readStream(_in, it);
		iterNumber = * (int *) uo -> data[0];

		delete uo;
		StreamReader::clearStream(_in, ';');

		uo = StreamReader::readStream(_in, it);
		switchNumber = * (int *) uo -> data[0];

		delete uo;
		StreamReader::clearStream(_in, ';');

		uo = StreamReader::readStream(_in, it);
		compareNumber = * (int *) uo -> data[0];

		delete uo;
		StreamReader::clearStream(_in);

		_e.setData(data);
		_e.setIterNum(iterNumber);
		_e.setSwitchNum(switchNumber);
		_e.setCompareNum(compareNumber);
	}
	catch (const std::exception& e)
	{
		delete[] data;
		throw e;
	}
}

//------------------------------------------------------------------------
// Entry Class definison
//------------------------------------------------------------------------

/*
	Empty constructor, it just set numberOfElements attribute.

	It is used when Entry object is created to be filled with data from reading specific file.
	It allocates memory for Entries indexes array.

	@param argc -> (int) number of elements in Entry.

	@note not used in this project.
*/
Log::Entry::Entry(int argc)
	: numberOfElements(argc)
{
	data = new int[argc];
}

/*
	Constructor, which fills Entry object with all values.

	It is used when Entry object is created in program to represent some sort step.
	It allocates memory for Entries indexes array and copies data sent as parameter.

	@param argc -> (int) number of elements in Entry.
	@param argv -> (const int *) pointer to an array which should be copied inside Entry objects attribute.
	@param iterNumber -> (int) number of iteration. 
	@param switchNumber -> (int) switch number.
	@param compareNum -> (int) compare number.

	@note only this is used.
*/
Log::Entry::Entry(int argc, const int* argv, int iterNumber, int switchNumber, int compareNum)
	: numberOfElements(argc), iterNumber(iterNumber), switchNumber(switchNumber), compareNumber(compareNum)
{
	data = new int[argc];
	for (int i = 0; i < argc; i++)
		data[i] = argv[i];
}

/*
	Destructor, cleans up after Entry object.

	Deallocates memory reserved for int array for Entry data.
*/
Log::Entry::~Entry()
{
	delete[] data;
}

/* 
	Setter method for int array, it creates copy on heap (which is then handled internally) 

	@param data -> (const int *) pointer to an array which should be copied inside instance.
*/
void Log::Entry::setData(const int* data)
{
	for (int i = 0; i < numberOfElements; i++)
		this -> data[i] = data[i];
}

/* 
	Setter method for iteration number

	@param in -> (int) iteration number.
*/
void Log::Entry::setIterNum(int in)
{
	iterNumber = in;
}

/*
	Setter method for switch number. 

	@param swn -> (int) switch number.
*/
void Log::Entry::setSwitchNum(int swn)
{
	switchNumber = swn;
}

/*
	Setter method for compare number.

	@param num -> (int) compare number.
*/
void Log::Entry::setCompareNum(int num)
{
	compareNumber = num;
}

/*
	Getter method for array of indexes for Entry.

	@returns (const int *) pointer to an array of indexes.
	
	@note returned array shouldn't be changed.
*/
const int* Log::Entry::getData() const
{
	return data;
}

/*
	Getter method for iteration number.

	@returns (int) iteration number.
*/
int Log::Entry::getIterNumber() const
{
	return iterNumber;
}

/*
	Getter method for switch number.

	@returns (int) switch number.
*/
int Log::Entry::getSwitchNumber() const
{
	return switchNumber;
}

/*
	Getter method for compare number.

	@returns (int) compare number.
*/
int Log::Entry::getCompareNumber() const
{
	return compareNumber;
}

/*
	Function which return's string representation of Entry.

	@return (std::string) representation of Entry class.
*/
std::string Log::Entry::toString() const
{
	std::stringstream ret;
	for (int i = 0; i < numberOfElements; i++)
		ret << data[i] << (i == numberOfElements - 1 ? ";" : " ");
	ret << iterNumber << ";" << switchNumber << ";" << compareNumber;

	return ret.str();
}

//------------------------------------------------------------------------
// END OF Entry Class definison
//------------------------------------------------------------------------