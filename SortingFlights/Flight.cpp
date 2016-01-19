#include "Flight.h"

/*
	Base constructor

	It sets flightLoaded flag to false, and allocates new character string on heap, for
	instances string representation.
*/
Flight::Flight()
	: flightLoaded(false)
{
	 stringRepresentation = new char[Flight::STRING_REPRESENTATION_SIZE];
}

/*
	Copy constructor.

	It provied deep copy.
*/
Flight::Flight(Flight& org)
	: flightLoaded(false)
{
	stringRepresentation = new char[Flight::STRING_REPRESENTATION_SIZE];
	destination = org.getDestination();
	flightTime = org.getFlightTime();
	flightNumber = org.getFlightNumber();
	gateNumber = org.getGateNumber();
}

/*
	Destructor.

	Clean heap memory alocated by Flight constructor.
*/
Flight::~Flight()
{
	delete stringRepresentation;
}

/*
	Getter method for destination property.

	@return (std::string) destination information.
*/
std::string Flight::getDestination() const
{
	return this -> destination;
}

/*
	Getter method for flight time property.

	@return (std::string) flight time information.
*/
std::string Flight::getFlightTime() const
{
	return this -> flightTime;
}

/*
	Getter method for flight number property.

	@return (std::string) flight number information.
*/
std::string Flight::getFlightNumber() const
{
	return this -> flightNumber;
}

/*
	Getter method for gate number property.

	@return (std::string) gate number information.
*/
std::string Flight::getGateNumber() const
{
	return this -> gateNumber;
}

/*
	Function that reads stream and gets Destination information (returns as a std::string)
	if anything goes wrong while reading it throws std::exception. It uses StreamReader namespace.

	@param _in				-> (std::istream&) stream which is used for reading.
	
	@throws std::exception	-> if reading goes wrong.

	@return (std::string) content read from input stream (stored in buffer)
*/
std::string Flight::readDestination(std::istream& _in)
{
	StreamReader::InputType it("s", Flight::MAX_STRING_SIZE, ';');
	StreamReader::UserObject* uo = StreamReader::readStream(_in, it);

	std::string ret = std::string(* (std::string*) uo -> data[0]);
	delete uo;

	return ret;
}

/*
	Function that reads stream and gets flight time information (returns as a std::string),
	format of reading is "dd.mm.yyyy HH:MM", and it must be followed or this function will throw error.
	if anything goes wrong while reading it throws std::exception. It uses StreamReader namespace.

	@param _in				-> (std::istream&) stream which is used for reading.

	@throws std::exception	-> if reading goes wrong.

	@return (std::string) content read from input stream (stored in buffer)
*/
std::string Flight::readDate(std::istream& _in)
{
	StreamReader::InputType it("ddcddcddddccddcdd", Flight::MAX_STRING_SIZE, ';');
	StreamReader::UserObject* uo = StreamReader::readStream(_in, it);

	char buffer[DATE_SIZE + 1];
	for (int i = 0; i < DATE_SIZE; i++)
		buffer[i] = * (char *) uo -> data[i];

	delete uo;
	buffer[DATE_SIZE] = '\0';

	std::stringstream ss;
	ss << buffer;

	char temp;
	int day, month, year, hour, minute;
	ss >> day >> temp >> month >> temp >> year >> temp >> hour >> temp >> minute;

	if (ss.fail())
		throw std::exception("Bad input file!");

	if (day > 31 || month > 12 || year > 2100 || hour > 23 || minute > 60)
		throw std::exception("Bad input file!");

	StreamReader::clearStream(_in, ';');

	return std::string(buffer);
}

/*
	Function that reads stream and gets flight number information (returns as a std::string),
	format of reading is 'cccDD' (c - alphanumeric char, D - digit), and it must be followed.
	if anything goes wrong while reading it throws std::exception.  It uses StreamReader namespace.

	@param _in				-> (std::istream&) stream which is used for reading.

	@throws std::exception	-> if reading goes wrong.

	@return (std::string) content read from input stream (stored in buffer)
*/
std::string Flight::readFlightNumber(std::istream& _in)
{
	StreamReader::InputType it("ccddd");
	StreamReader::UserObject* uo = StreamReader::readStream(_in, it);

	char buffer[6];
	for (int i = 0; i < 5; i++)
		buffer[i] = * (char*) uo -> data[i];
	buffer[5] = '\0';

	delete uo;
	StreamReader::clearStream(_in, ';');

	return std::string(buffer);
}

/*
	Function that reads stream and gets gate number information (returns as a std::string),
	format of reading is 'cD' (c - alphanumeric char, D - digit), and it must be followed.
	if anything goes wrong while reading it throws std::exception. It uses StreamReader namespace.

	@param _in				-> (std::istream&) stream which is used for reading.

	@throws std::exception	-> if reading goes wrong.

	@return (std::string) content read from input stream (stored in buffer)
*/
std::string Flight::readGateNumber(std::istream& _in)
{
	StreamReader::InputType it("cd");
	StreamReader::UserObject* uo = StreamReader::readStream(_in, it);

	char buffer[3];
	for (int i = 0; i < 2; i++)
		buffer[i] = * (char*) uo -> data[i];
	buffer[2] = '\0';
	
	delete uo;
	return std::string(buffer);
}

/*
	This function is designed to create string representation of Flight instance.

	It is done only once (first time function is called - in operator>>), it fills instance's
	stringRepresentation array (on heap), and every next time it just return's pointer to that
	char array. Data filled here is deallocated in destructor of Flight instance.

	@return -> pointer to char array (on heap) with string representation of instance.

	@note -> shouldn't be manually deleted (It is handled by object Constructor/Destructor pair).
*/
const char* Flight::toString() const
{
	if (!flightLoaded)
	{
		int currPos = 0;

		int length = destination.length();
		int left = (DEST_SIZE - length) / 2;

		stringRepresentation[currPos++] = ' ';
		for (int i = 0; i < FN_SIZE; i++)
			stringRepresentation[currPos++] = flightNumber[i];

		stringRepresentation[currPos++] = ' ';
		stringRepresentation[currPos++] = '|';
		stringRepresentation[currPos++] = ' ';

		if (left > 0)
		{
			for (int i = 0; i < left; i++)
				stringRepresentation[currPos++] = ' ';
			for (int i = 0; i < length; i++)
				stringRepresentation[currPos++] = destination[i];
			for (int i = left + length; i < DEST_SIZE; i++)
				stringRepresentation[currPos++] = ' ';
		}
		else
		{
			for (int i = 0; i < DEST_SIZE - 3; i++)
				stringRepresentation[currPos++] = destination[i];
			for (int i = 0; i < 3; i++)
				stringRepresentation[currPos++] = '.';
		}

		stringRepresentation[currPos++] = ' ';
		stringRepresentation[currPos++] = '|';
		stringRepresentation[currPos++] = ' ';

		for (int i = 0; i < GN_SIZE; i++)
			stringRepresentation[currPos++] = gateNumber[i];

		stringRepresentation[currPos++] = ' ';
		stringRepresentation[currPos++] = '|';
		stringRepresentation[currPos++] = ' ';

		for (int i = 0; i < DATE_SIZE; i++)
			stringRepresentation[currPos++] = flightTime[i];

		stringRepresentation[currPos++] = ' ';
		stringRepresentation[currPos++] = '\n';
		stringRepresentation[currPos++] = '\0';
	}

	return stringRepresentation;
}

/*
	Overloaded operator for reading object from some input stream.
	If reading went well (no exception raised), Flight::toString() is called, for current instance
	(so it will create initial self stringRepresentation) and it's flightLoaded flag is set to true.

	@param _in				-> (std::istream&) input stream from which instance information should be read.
	@param _f				-> (Flight&) reference to an object which should be filled with information.

	@throws std::exception	-> if anything goes wrong while reading.

	@return (std::istream&) reference to stream from which it was read.
*/
std::istream& operator>>(std::istream& _in, Flight& _f)
{
	_f.destination = Flight::readDestination(_in);
	_f.flightTime = Flight::readDate(_in);
	_f.flightNumber = Flight::readFlightNumber(_in);
	_f.gateNumber = Flight::readGateNumber(_in);

	StreamReader::clearStream(_in);

	_f.toString();
	_f.flightLoaded = true;

	return _in;
}