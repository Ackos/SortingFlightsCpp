#ifndef _STREAM_READER_INCLUDED_
#define _STREAM_READER_INCLUDED_

#include <stdexcept>
#include <vector>
#include <list>
#include <iostream>

/*
	Namespace specially designed for parsing input from streams.

	It contains two classes, one for defining pattern which should be read from stream, and
	another one to be some kind of data container for data read from stream.

	Idea:
		StreamReader::InputType parses string passed to it, and stores list of ints
		(defined by its enumeration) as a types which should be read from stream. And that is
		passed to readStream function, which then uses few mini funcitions for reading Stream
		in appropriate order which InputType::info (list) declares, creates objects of those
		types on heap, and casts their pointers to (void *) which stores inside its vector.
		User gets back pointer to a StreamReader::UserObject and can access its data vector,
		and then by indexes should parse its (void *) to appropriate type pointers.
		UserObject will delete all object it created, but you must delete pointer returned to you
		by function readStream()

		@note Not exactly typesafe.

	Pattern:
		Pattern is simple string, but can contain only next characters:

		c - 'char'
		s - 'const char *'
		i - 'int'
		d - 'digit'
		f - 'float'
		D - 'double'

	Simple Example:
		try
		{
			StreamReader::InputType in("ccisD", 100, ';');
			StreamReader::UserObject* uo = StreamReader::readStream(stdin, in);

			char c = * (char *) uo -> data[0];
			char d = * (char *) uo -> data[1];
			int a = * (int *) uo -> data[2];
			std::string = * (std::string *) uo -> data[3];
			double dd = * (double *) uo -> data[4];

			delete uo;
		}
		catch (StreamReader::InvalidPattern& e)
		{
			std::cerr << e.what() << std::endl;
		}
		catch (std::exception&)
		{
			std::cerr << "Input doesn't match pattern passed!\n";
		}

	@TODO:
		fix memory leak with strings (deallocating).

	@author Acko.
*/
namespace StreamReader
{
	/*
		Exception derived class

		It will be thrown if pattern isn't valid.
	*/
	struct InvalidPattern;

	/*
		Class presenting interface between user (string passed) and UserObject.

		It contains enumeration which is stored in list of information after parsing string
		passed to constructor, and it is a bypass object which describes how stream should be
		read.

		@author Acko.
	*/
	class InputType
	{
	public:
		/*
			Integer that describes upper bond for character number when reading string type 
		*/
		int MAX_CHAR;

		/*
			Character which describes delimiter char when reading string type.

			Delimiter is not loaded into result, but it is removed from stream.
		*/
		char DELIMITER;

		/*
			Constructor, 

			It takes pattern string which is parsed into list of enumeration,
			and two optional parameters (MAX_CHAR and DELIMITER).
		*/
		InputType(const char *, int = 50, char = '\n'); 

		/*
			Enumeration which is used to describe which type should be read from stream.
		*/
		enum { CHAR, INT, DIGIT, FLOAT, DOUBLE, STRING };

		/*
			List of Enumeration, descriptor list - pattern parsed.
		*/
		std::list<int> info;
	};

	/*
		Class presenting data container.

		When stream is read, data is recorded on heap, and those pointers are casted to (void *)
		and stored inside this objects data list.

		@note All data which is pushed to this object will be destroyed when object is destoryed.
	*/
	class UserObject
	{
	private:
		/*
			Deletes all objects from heap (going thorugh vector) and clears out vector.
		*/
		void clear();
	public:
		/*
			Constructor, Empty.
		*/
		UserObject();

		/*
			Destructor deletes all objects stored in vector data.
		*/
		~UserObject();

		/*
			Function that stores void * passed to it inside data vector.
		*/
		void addValue(void *);

		/*
			Vector which holds all void pointers to data.
		*/
		std::vector<void *> data;

		/*
			Function which delete's pointer sent to it if it is not nullptr, and calls clear() method.
		*/
		void fail(void * = nullptr);
	};

	/*
		Main function from this namespace.

		It takes InputType object reference from which it reads info list, and by that, it reads
		istream sent as first parameter, alocates memory dynamically an casts new pointer to void,
		which then store inside UserObject that it also created dynamically on beggining.

		If Stream reading fail in some point, it calls self method fail, which destroys, UserObject
		created, and throws an exception.

		@TODO: fix memory leak with strings...
	*/
	UserObject* readStream(std::istream&, const InputType&);

	/*
		Function which clears given stream until it finds first occurance of character sent to it.
		Delimiter char is also cleared from stream.
	*/
	void clearStream(std::istream&, char = '\n');

	/*
		Function which deallocates memory for UserObject (and all that is allocated untill that point)
		and throws exception.
	*/
	void fail(UserObject* = nullptr, void* = nullptr);
};

#endif