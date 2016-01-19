#include "StreamReader.h"

/*
	Local function which determines if sent char is an english alphabet character.

	@param c	-> (char) which should be checked.

	@return (bool) true if gien char is an english alphabet character, false otherwise.
*/
bool isChar(char c)
{
	return ((( int ) c >= 'A' && ( int ) c <= 'Z') || (( int ) c >= 'a' && ( int ) c <= 'z') || ( int ) c == 46 || ( int ) c == 32 || ( int ) c == 58);
}

/*
	Local function which determines if sent char is a digit

	@param c	-> (char) which should be checked.

	@return (bool) true if gien char is digit, false otherwise.
*/
bool isNum(char c)
{
	return (( int ) c >= '0' && ( int ) c <= '9');
}

/*
	Definition of derived exception class
*/
struct StreamReader::InvalidPattern : public std::invalid_argument
{
	InvalidPattern() : std::invalid_argument("Invalid pattern!")
	{
	}
};

//----------------------------------------------------------------------------------
// InputType class Definition
//---------------------------------------------------------------------------------

/*
	Constructor,
	
	It parses pattern that it got, character by character, and pumps info list with
	appropriate enumeration value.

	If MAX_SIZE or delimiter is passed it sets those values too.

	@param pattern -> (const char *) pattern to be parsed.
	@param MAX_SIZE -> (int) (has default) defines MAX_CHAR value
	@param delimiter -> (char) (has default) defines DELIMITER value

	@throws InvalidPattern -> if pattern is not valid.
*/
StreamReader::InputType::InputType(const char* pattern, int MAX_SIZE, char delimiter)
	: MAX_CHAR(MAX_SIZE), DELIMITER(delimiter)
{
	while (*pattern != '\0')
	{
		switch (*pattern)
		{
		case 'c':
			info.push_back(CHAR);
			break;
		case 'i':
			info.push_back(INT);
			break;
		case 'd':
			info.push_back(DIGIT);
			break;
		case 'f':
			info.push_back(FLOAT);
			break;
		case 'D':
			info.push_back(DOUBLE);
			break;
		case 's':
			info.push_back(STRING);
			break;
		default:
			info.clear();
			throw InvalidPattern();
			break;
		}
		++pattern;
	}
}

//-------------------------------------------------------------
// END OF InputType class definition
//------------------------------------------------------------

//-------------------------------------------------------------
// UserObject class definition
//-------------------------------------------------------------

/*
	Constructor, Empty.
*/
StreamReader::UserObject::UserObject()
{
}

/*
	Destructor,

	Clears up data vector (using clear function)

	@note here is deallocated every data object allocated earlyer.
*/
StreamReader::UserObject::~UserObject()
{
	clear();
}

/*
	Just an interface function, adds value to a vector

	@param value -> (void *) pointer to data which should be stored inside data vector.
*/
void StreamReader::UserObject::addValue(void* value)
{
	data.push_back(value);
}

/*
	Function which deletes pointer that it get in parameter, and calls self clear method.

	@param p -> (void *) (has default) pointer of data to be deleted or nullptr if there is no such data.
*/
void StreamReader::UserObject::fail(void* p)
{
	if (p != nullptr)
		delete p;
	clear();
}

/*
	Destructor helper function, goes through data vector and deletes one by one element from heap, and clears vector in end.

	@note HERE IS MEMORY LEAK... (deleting void * doesn't call destructor, which is not problem for primitive types,
									but is for std::string type...).
*/
void StreamReader::UserObject::clear()
{
	for (void* p : data)
		delete p;
	data.clear();
}

//--------------------------------------------------------------------------
// END OF UserObject class definition
//--------------------------------------------------------------------------

/*
	Main function from this namespace.

	It takes InputType object reference from which it reads info list, and by that, it reads
	istream sent as first parameter, alocates memory dynamically an casts new pointer to void,
	which then store inside UserObject that it also created dynamically on beggining.

	If Stream reading fail in some point, it calls self method fail, which destroys, UserObject
	created, and throws an exception.

	@param _in -> (std::istream&) reference to stream from which it should read
	@param _it -> (const StreamReader::InputType&) reference to InputType object which describes
					how should stream be read.

	@TODO: fix memory leak with strings...
*/
StreamReader::UserObject* StreamReader::readStream(std::istream& _in, const StreamReader::InputType& _it)
{
	UserObject* ret = new UserObject();
	for (int type : _it.info)
	{
		void* p;
		switch (type)
		{
		case InputType::CHAR:
			p = new char;
			_in.get(* (char*) p);
			if (!isChar(* (char*) p) || _in.fail())
				fail(ret, p);
			ret -> addValue(p);
			break;
		case InputType::INT:
			p = new int;
			_in >> * (int*) p;
			if (_in.fail())
				fail(ret, p);
			ret -> addValue(p);
			break;
		case InputType::DIGIT:
			p = new char;
			_in.get(* (char*) p);
			if (!isNum(* (char*) p) || _in.fail())
				fail(ret, p);
			ret -> addValue(p);
			break;
		case InputType::FLOAT:
			p = new float;
			_in >> * (float*) p;
			if (_in.fail())
				fail(ret, p);
			ret -> addValue(p);
			break;
		case InputType::DOUBLE:
			p = new double;
			_in >> * (double*) p;
			if (_in.fail())
				fail(ret, p);
			ret -> addValue(p);
			break;
		case InputType::STRING:
		{
			char* buffer = new char[_it.MAX_CHAR];
			for (int i = 0; i < _it.MAX_CHAR - 1; i++)
			{
				_in.get(buffer[i]);
				if (_in.fail())
				{
					fail(ret);
					delete[] buffer;
				}
				if (buffer[i] == _it.DELIMITER)
				{
					buffer[i] = '\0';
					break;
				}
			}
			p = new std::string(buffer);
			delete[] buffer;
			ret -> addValue(p);
			break;
		}
		default:
			throw std::exception("Bad file input");
			break;
		}
	}
	return ret;
}

/*
	Function which clears given stream until it finds first occurance of character sent to it.
	Delimiter char is also cleared from stream.

	@param _in				-> (std::istream&) stream which will be cleared.
	@param delimiter		-> (char) char which will be searched for, default value new line ('\n').

	@throws std::exception	-> if clear was unsucessufull.
*/
void StreamReader::clearStream(std::istream& _in, char delimiter)
{
	char c;
	do
	{
		_in.get(c);
		if (_in.fail())
			throw std::exception("Bad input file.");
	} while (c != delimiter);
}

/*
	Function which deallocates memory for UserObject (and all that is allocated untill that point)
	and throws exception.

	@param uo -> (UserObject * ) (has default) pointer to a UserObject which was created before fail, and
						now should be deleted, or nullptr if such object hasn't been created.
	@param p -> (void *) (has default) pointer to a data which should be deleted (left overs of a process) or nullptr
						if such data doesn't exist
*/
void StreamReader::fail(UserObject* uo, void* p)
{
	if (uo != nullptr)
	{
		uo -> fail(p);
		delete uo;
	}
	throw std::exception("File corrupted");
}