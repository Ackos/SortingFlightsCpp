#ifndef _FLIGHT_HANDLER_INCLUDED_
#define _FLIGHT_HANDLER_INCLUDED_

#include "Flight.h"
#include "Sorter.h"

/*
	class FlightHandler, main model class.

	Designed to store and handle all Flight data inside program.
	@author Acko.
*/
class FlightHandler
{
private:
	/*
		Pointer to an array of Flight instance pointers.

		Main data of this class. Array is internally handled by class.
	*/
	Flight** flights;

	/*
		Pointer to an array of strings (const char *) representing Flight objects.

		This member of class exists to ensure no memory leaks when creating such string
		representations. In it will be stored Flights in any order. (getFlightsString())

		It's allocation and dealocation is handled internally by class.
	*/
	const char** flightsString;

	/*
		Integer which describes how many Flight objects should be loaded from database.
	*/
	int flightsToLoad;

	/*
		Boolean flag showing if FlightHandler instance has loaded Flight objects or not.
	*/
	bool isLoaded;

	/*
		Function which Handle's clearing array of Flights.

		It ensures that each Flight in array first be deleted from heap. (no memory leaks).
	*/
	void clearFlights();

public:
	/*
		Enumeration, designed for sort compare function choosing.
	*/
	enum { DESTINATION_UP = 1, DESTINATION_DOWN, FLIGHT_TIME_UP, FLIGHT_TIME_DOWN,
			FLIGHT_NUMBER_UP, FLIGHT_NUMBER_DOWN, GATE_NUMBER_UP, GATE_NUMBER_DOWN };

	/*
		Constructor, set's initial parameters and does necessary memory allocation.

		It accepts number of Flights to load as a parameter.
	*/
	FlightHandler(int);

	/*
		Destructor, does necessary memory cleaning.
	*/
	~FlightHandler();

	/*
		Function which takes path to database file as a parameter, and tries to read that file.

		It reads from file appropriate number of Flight objects (which creates on Heap), and store they're
		pointers into FlightHandler::Flights** array.
		If anything goes wrong while reading, it throws std::exception.
	*/
	void loadFlights(const char *);

	/*
		Function which takes path to file as a parameter and tries to write current Flight objects into that file.

		It open's stream and try to write all data from FlightHandler::Flights** array into that file. If second
		parameter is passed, it does index inverting while writing (writes in specific order).
	*/
	void writeFlights(const char *, const int * = nullptr) const;

	/*
		Function which returns isLoaded flag (information if Flights have been loaded).
	*/
	bool checkLoaded() const;

	/*
		This function fills FlightHandler::flightsString array with new information. 

		It does so by going through self Flight array and writing one by one to array.
		If parameter is passed it does index fliping when writing Flight::toString() to self array.
	*/
	const char** getFlightsString(const int * = nullptr) const;

	/*
		Function which allocates heap memory for new list in which it creates deep copy of instances in self list.

		This function is only dangerous function for MEMORY LEAK because it doesn't clean up after it self,
		that is left to user. But interface for that is created (FlightHandler::destroyList(Flight **)), so
		whenever this function is used, after should be called destroyList on same pointer.
	*/
	Flight** copyList() const;

	/*
		Function which cleans up after FlightHandler::copyList() function.

		It should be called manually after each FlightHandler::copyList() to ensure no MEMORY LEAK, 
		but it shouldn't be called on pointers not created by copyList(), then behaviour is undefined.
	*/
	void destroyList(Flight **) const;

	//--------------------------------------------------------------------
	// Block of compare functions, static functions which will be sent
	// to Sorter namespace methods when sorting some Flight array.
	//
	// Each function has same structure it return's bool and accepts
	// two Flight const references, which then it compares and returns
	// boolean value depending on Flight instances, and sorting criteria.
	//---------------------------------------------------------------------
	
	/* Criteria: By destination in Alphabetical order */
	static bool destinationUp(const Flight&, const Flight&);
	
	/* Criteria: By destination in reverse alphabetical order */
	static bool destinationDown(const Flight&, const Flight&);
	
	/* Criteri: By Flight time (date) from newest to oldest */
	static bool flightTimeUp(const Flight&, const Flight&);
	
	/* Criteria: By Flight time (date) from oldest to newest */
	static bool flightTimeDown(const Flight&, const Flight&);

	/* Criteria: By Flight nubmer in alphabetical order */
	static bool flightNumberUp(const Flight&, const Flight&);
	
	/* Criteria: By Flight nubmer in reverse alphabetical order */
	static bool flightNumberDown(const Flight&, const Flight&);
	
	/* Criteria: By Gate number in alphabetical order */
	static bool gateNumberUp(const Flight&, const Flight&);

	/* Criteria: By Gate number in reverse alphabetical order */
	static bool gateNumberDown(const Flight&, const Flight&);

	//-------------------------------------------------------------
	// End of compare function block
	//-------------------------------------------------------------
};

#endif