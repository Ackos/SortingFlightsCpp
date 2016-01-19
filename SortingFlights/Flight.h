#ifndef _FLIGHT_INCLUDED_
#define _FLIGHT_INCLUDED_

#include <string>
#include <fstream>
#include <sstream>
#include "StreamReader.h"


/*
	Class Flight implemented to describe one flight data.

	It contains destination, flight time, flight number and gate number information.
	@author Acko.
*/
class Flight
{
private:
	/*
		Pointer to char array with string representation of current instance.
		Allocation handled internally inside class.
	*/
	char* stringRepresentation;

	/* Flag describing if instance is loaded (if data is not empty) */
	bool flightLoaded;

public:
	std::string destination;
	std::string flightTime;
	std::string flightNumber;
	std::string gateNumber;

	/*
		Constructor
	*/
	Flight();

	/* 
		Copy constructor. (deep copy)
	*/
	Flight(Flight&);

	/*
		Destructor.
	*/
	~Flight();

	/*
		Function that reads stream and gets Destination information (returns as a std::string)
		if anything goes wrong while reading it throws std::exception.
	*/
	static std::string readDestination(std::istream&);

	/*
		Function that reads stream and gets Date information (returns as a std::string)
		if anything goes wrong while reading it throws std::exception.
	*/
	static std::string readDate(std::istream&);

	/*
		Function that reads stream and gets Flight number information (returns as a std::string)
		if anything goes wrong while reading it throws std::exception.
	*/
	static std::string readFlightNumber(std::istream&);

	/*
		Function that reads stream and gets Gate number information (returns as a std::string)
		if anything goes wrong while reading it throws std::exception.
	*/
	static std::string readGateNumber(std::istream&);

public:
	/*
		Constant which limits maximum number of characters in each string property
	*/
	static const int MAX_STRING_SIZE = 100;

	/*
		Constants defining how many character size is for which string
	*/
	static const int FN_SIZE = 5, GN_SIZE = 2, DATE_SIZE = 17, DEST_SIZE = 20;

	/*
		Size of string representation (in chars) of one Flight instance
	*/
	static const int STRING_REPRESENTATION_SIZE = FN_SIZE + 2 + GN_SIZE + 2 + DATE_SIZE + 2 + DEST_SIZE + 2 + 3 + 2;

	/*
		Getter method for destination property 
	*/
	std::string getDestination() const;

	/*
		Getter method for flight time property 
	*/
	std::string getFlightTime() const;

	/*
		Getter method for flight number property 
	*/
	std::string getFlightNumber() const;

	/* 
		Getter method for gate number property
	*/
	std::string getGateNumber() const;

	/*
		Function which return's pointer to string representation of Flight instance.
		That data shouldn't be changed outside class. If it is changed or deleted, instance behaviour
		is unpredictable.
	*/
	const char* toString() const;

	/*
		Overloaded operator for reading object from some input stream.
	*/
	friend std::istream& operator>>(std::istream&, Flight&);
};

#endif