#include "FlightHandler.h"

/*
	Constructor,

	Sets fligtsToLoad to value accepted as parameter, and isLoaded flag to false.
	Also it allocates memory for Flight* array on heap, and memory for flightString array.

	@param flightsToLoad -> (int) number of Flight object to load and store.
*/
FlightHandler::FlightHandler(int flightsToLoad)
	:  flightsToLoad(flightsToLoad), isLoaded(false)
{
	flights = new Flight*[flightsToLoad];
	for (int i = 0; i < flightsToLoad; i++)
		flights[i] = nullptr;

	flightsString = new const char*[flightsToLoad];
}

/*
	Destructor,

	Cleans up after FlightHandler constructor, it deallocates heap memory reserved for
	Flight* array and FligthsString char array. Also it deallocates memory reserved for each
	Flight instance which is created by this instance in loadData().
*/
FlightHandler::~FlightHandler()
{
	clearFlights();
	delete[] flights;
	delete[] flightsString;
}

/*
	Function which takes path to database file as a parameter, and tries to read that file.

	It reads from file appropriate number of Flight objects (which creates on Heap), and store they're
	pointers into FlightHandler::Flights** array.
	If anything goes wrong while reading, it throws std::exception.

	For reading Flight objects it uses Flight::operator>>.

	@param databasePath -> (const char *) path to database file.

	@throws std::exception -> if anything goes wrong while reading.
*/
void FlightHandler::loadFlights(const char* databasePath)
{
	if (!*databasePath)
		throw std::exception("Database path not suplied! Program will load default io path.");

	// open file stream and read n flights
	std::ifstream ifs(databasePath);
	try
	{	
		if (!ifs)
			throw std::exception("Invalid path! Program will load default io path.");

		Flight* tempFlight;
		for (int i = 0; i < this -> flightsToLoad; i++)
		{
			tempFlight = new Flight();

			ifs >> *tempFlight;
			flights[i] = tempFlight;
		}
	}
	catch (const std::exception& e)
	{
		clearFlights();
		ifs.close();
		throw e;
	}
	ifs.close();
	isLoaded = true;
}

/*
	Function which takes path to file as a parameter and tries to write current Flight objects into that file.

	It open's stream and try to write all data from FlightHandler::Flights** array into that file. If second
	parameter is passed, it does index inverting while writing (writes in specific order).

	Second parameter should be pointer to an array of integers which represent order of indexes in which
	Flight objects should be written into file. Function does not do any validation on this array, if it's
	structure isn't right behaviour is unpredictable.
	@example 
		{ 0, 1, 2, 3, 4, 5, ... , flightsToLoad - 1 } -> valid, nothing changed, original structure;
		{ 1, 2, 3, 0, 5, 4, ... , flightsToLoad - 1 } -> valid, some random permutation
		{ 0, 0, 0, 0 } -> invalid, unsuitable number of elements. (all same elements means that only 
																	that one Flight will be written)
		{ 0, 1, 2, flightsToLoad + n (n >= 0), ... } -> invalid, flightsToLoad + n goes out of scope
															of original array size.

	@param path -> (const char *) path to file in which it should write.
	@param indexes -> (const int *) order in which Flights object should be written (described up)

	@throws std::exception -> if writing to file fails.
*/
void FlightHandler::writeFlights(const char* path, const int * indexes) const
{
	std::ofstream ofs(path);

	try
	{
		if (!ofs)
			throw std::exception("Can not write into file.");

		for (int i = 0; i < flightsToLoad; i++)
		{
			ofs << (indexes != nullptr ? (flights[indexes[i]] -> toString()) : flights[i] -> toString());
			if (ofs.fail())
				throw std::exception("Failed writing into file.");
		}
	}
	catch (std::exception& e)
	{
		ofs.close();
		throw e;
	}

	ofs.close();
}

/*
	Function which returns isLoaded flag (information if Flights have been loaded).

	@returns -> (bool) true if Flights are loaded, false otherwise.
*/
bool FlightHandler::checkLoaded() const
{
	return isLoaded;
}

/*
	This function fills FlightHandler::flightsString array with new information. 

	It does so by going through self Flight array and writing one by one to array.
	If parameter is passed it does index fliping when writing Flight::toString() to self array.

	Pointer to an array shouldn't be changed or deleted, it is handled internally in class.
	If it is changed outside behaviour is unpredictable.

	@param indexes -> order in which elements should be sorted in string (same as in writeFlights)

	@returns -> (const char **) pointer to an array with string representation of all Flight objects
					if instance has loaded objects, nullptr if not.
*/
const char** FlightHandler::getFlightsString(const int* indexes) const
{
	if (!isLoaded)
		return nullptr;
	
	for (int i = 0; i < flightsToLoad; i++)
		flightsString[i] = (indexes == nullptr ? flights[i] -> toString() : flights[indexes[i]] -> toString());

	return flightsString;
}

/*
	Function which allocates heap memory for new list in which it creates deep copy of instances in self list.

	This function is only DANGEROUS function for MEMORY LEAK because it doesn't clean up after it self,
	that is left to user. But interface for that is created (FlightHandler::destroyList(Flight **)), so
	whenever this function is used, after should be called destroyList on same pointer.

	@returns -> (Flight **) pointer to an array of copy list (with new values of instances).

	@note After each call of this function it should be called FlightHandler::destroyList on pointer this function
				returned.
*/
Flight** FlightHandler::copyList() const
{
	Flight** retList = new Flight*[flightsToLoad];
	for (int i = 0; i < flightsToLoad; i++)
		retList[i] = new Flight(*flights[i]);
	return retList;
}

/*
	Function which cleans up after FlightHandler::copyList() function.

	It should be called manually after each FlightHandler::copyList() to ensure no MEMORY LEAK, 
	but it shouldn't be called on pointers not created by copyList(), then behaviour is undefined.

	@param list -> (Flight **) pointer to an object which it should destroy.

	@note Call this function always after FlightHandler::copyList() with pointer that copyList returns,
			to avoid MEMORY LEAKs, BUT DO NOT CALL IT on random pointers.
*/
void FlightHandler::destroyList(Flight** list) const
{
	for (int i = 0; i < flightsToLoad; i++)
		delete list[i];
	delete[] list;
}

/*
	Function which Handle's clearing array of Flights.

	It ensures that each Flight in array first be deleted from heap. (no memory leaks).
	If any object is deleted outside class behaviour undefined.
*/
void FlightHandler::clearFlights()
{
	for (int i = 0; i < flightsToLoad; i++)
		if (flights[i] != nullptr)
		{
			delete flights[i];
			flights[i] = nullptr;
		}
}

//--------------------------------------------------------------------
// Block of compare functions, static functions which will be sent
// to Sorter namespace methods when sorting some Flight array.
//
// Each function has same structure it return's bool and accepts
// two Flight const references, which then it compares and returns
// boolean value depending on Flight instances, and sorting criteria.
//---------------------------------------------------------------------

/* Criteria: By destination in Alphabetical order */
bool FlightHandler::destinationUp(const Flight& f1, const Flight& f2)
{
	return f1.getDestination() < f2.getDestination();
}

/* Criteria: By destination in reverse alphabetical order */
bool FlightHandler::destinationDown(const Flight& f1, const Flight& f2)
{
	return f1.getDestination() > f2.getDestination();
}

/* Criteri: By Flight time (date) from newest to oldest */
bool FlightHandler::flightTimeUp(const Flight& f1, const Flight& f2)
{
	std::stringstream ss;
	ss << f1.getFlightTime();

	char temp;
	int day1, month1, year1, hour1, minute1;
	ss >> day1 >> temp >> month1 >> temp >> year1 >> temp >> hour1 >> temp >> minute1;

	ss.clear();

	ss << f2.getFlightTime();
	int day2, month2, year2, hour2, minute2;
	ss >> day2 >> temp >> month2 >> temp >> year2 >> temp >> hour2 >> temp >> minute2;

	if (year1 == year2)
		if (month1 == month2)
			if (day1 == day2)
				if (hour1 == hour2)
					return minute1 < minute2;
				else
					return hour1 > hour2;
			else
				return day1 > day2;
		else
			return month1 > month2;
	else
		return year1 > year2;
}

/* Criteria: By Flight time (date) from oldest to newest */
bool FlightHandler::flightTimeDown(const Flight& f1, const Flight& f2)
{
	std::stringstream ss;
	ss << f1.getFlightTime();

	char temp;
	int day1, month1, year1, hour1, minute1;
	ss >> day1 >> temp >> month1 >> temp >> year1 >> temp >> hour1 >> temp >> minute1;
	
	ss.clear();

	ss << f2.getFlightTime();
	int day2, month2, year2, hour2, minute2;
	ss >> day2 >> temp >> month2 >> temp >> year2 >> temp >> hour2 >> temp >> minute2;
	
	if (year1 == year2)
		if (month1 == month2)
			if (day1 == day2)
				if (hour1 == hour2)
					return minute1 < minute2;
				else
					return hour1 < hour2;
			else
				return day1 < day2;
		else
			return month1 < month2;
	else
		return year1 < year2;
}

/* Criteria: By Flight nubmer in alphabetical order */
bool FlightHandler::flightNumberUp(const Flight& f1, const Flight& f2)
{
	return f1.getFlightNumber() < f2.getFlightNumber();
}

/* Criteria: By Flight nubmer in reverse alphabetical order */
bool FlightHandler::flightNumberDown(const Flight& f1, const Flight& f2)
{
	return f1.getFlightNumber() > f2.getFlightNumber();
}

/* Criteria: By Gate number in alphabetical order */
bool FlightHandler::gateNumberUp(const Flight& f1, const Flight& f2)
{
	return f1.getGateNumber() < f2.getGateNumber();
}

/* Criteria: By Gate number in reverse alphabetical order */
bool FlightHandler::gateNumberDown(const Flight& f1, const Flight& f2)
{
	return f1.getGateNumber() > f2.getGateNumber();
}

//-------------------------------------------------------------
// End of compare function block
//-------------------------------------------------------------