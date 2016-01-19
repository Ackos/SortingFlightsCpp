#ifndef _LOG_INCLUDED_
#define _LOG_INCLUDED_

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "StreamReader.h"

/*
	Class Log

	This class is designed to store information about steps inside sort algorithms. It has internal private
	class Entry which stores actually those data (current indexes, iteration, switch and compare number).

	Declaring Entry as a private class ensures that no one will mess up anything about storing those structures
	inside memory (no one outside class can delete some Entry object or something alike).

	This class can be sent to a sort algorithm and it will create one Entry instance for each important step
	in algorithm. Also it could read from file already writen by specific rules (Not used in this program)

	@author Acko.
*/
class Log
{
private:

	/*
		Class Entry

		Private internal class which stores data about one step in sort algorithms.

		@author Acko.
	*/
	class Entry
	{
		/* Value which determines how many indexes it stores */
		int numberOfElements;

		/* Dinamicaly allocated array of integers with index order for current sort step */
		int* data;

		/* Iteration number in current sort step */
		int iterNumber;

		/* Number of element switches until current sort step */
		int switchNumber;

		/* Number of compares until current sort step */
		int compareNumber;

	public:
		/*
			Empty constructor, it just set numberOfElements attribute.

			It is used when Entry object is created to be filled with data from reading specific file.
			@note not used in this project.
		*/
		Entry(int);

		/*
			Constructor, which fills Entry object with all values.

			It is used when Entry object is created in program to represent some sort step.
			@note only this is used.
		*/
		Entry(int, const int*, int, int, int);

		/*
			Descructor, frees up all memory allocated by Entry constructor.
		*/
		~Entry();

		/* Setter method for int array, it creates copy on heap (which is then handled internally) */
		void setData(const int*);

		/* Setter method for iteration number */
		void setIterNum(int);

		/* Setter method for switch number. */
		void setSwitchNum(int);

		/* Setter method for compare number. */
		void setCompareNum(int);

		/* Getter method for array of indexes (int) */
		const int* getData() const;

		/* Getter method for iteration number. */
		int getIterNumber() const;

		/* Getter method for switch number. */
		int getSwitchNumber() const;

		/* Getter method for compare number. */
		int getCompareNumber() const;

		/*
			Function which returns std::string representation of one Entry instance.

			It is used when writing Entry to an file. Operator not overloaded because it is
			private class.
		*/
		std::string toString() const;
	};

	// ---------------------------------------------------------------------
	// END OF ENTRY CLASS DECLARATION
	// ---------------------------------------------------------------------

	/*
		Integer which describes how many integers should be each entry.
	*/
	int numberOfElementsInEntry;

	/* Pointer to an array with differences */
	int* difference;

	/*
		Container for all Entry objects.
		
		All Entries are created dinamically (on heap), and only pointers are stored.
		Also this allocation and deallocation is handled internally by class, so nothing shouldn't
		be deleted outside (behaviour undefined).
	*/
	std::vector<Entry*> entries;

	/*
		Private method which loads one entry,

		It is instead of overloading operator>>() because Entry class is private.
	*/
	void loadEntry(std::istream& _in, Entry& _e);

public:
	/*
		Constructor,

		Sets number of integer objects inside each Entry. And allocates memory for difference array.
	*/
	Log(int);

	/*
		Destructor, cleans up heap after Log instance,

		All Entry objects created inside this class will be destructed inside class.
	*/
	~Log();

	/*
		Function which takes path to file as a parameter and tryies to write inside that file.

		If it can not write inside file, it will throw an error. Also it relies on Entry::toString()
		for writing.
	*/
	void writeData(const char *);

	/*
		Function which takes path to file as a parameter and tryies to read data from it.

		It will read data from file and create one Entry instance per line in file if file is valid.
		If it fails reading, it will throw error.

		@note this function isn't used in this project.
	*/
	void readData(const char *);

	/*
		Public interface for Entry class. 

		This function takes all arguments necessary for creating one Entry instance, and then, creates
		one instance on heap, and store it's pointer in vector of entries.

		Entry constructor will create copy of data sent to it, so first parameter can be const int *,
		those data will stay untouched, and can be deleted after calling createEntry().
	*/
	void createEntry(const int*, int, int, int);

	/*
		Getter method for data of i-th Entry stored in Log instance.
	*/
	const int* getData(int) const;

	/*
		Getter method for iteration number stored in i-th Entry of Log instance.
	*/
	int getIterNumber(int) const;

	/*
		Getter method for switch number stored in i-th Entry of Log instance.
	*/
	int getSwitchNumber(int) const;

	/*
		Getter method for compare number stored in i-th Entry of Log instance.
	*/
	int getCompareNumber(int) const;

	/*
		Fills instances difference array, and returns pointer to it.

		Managing memory for this array is handled internally should not be redefined outside.
	*/
	const int* getDifference(int, int);

	/*
		Function which goes through vector of Entry objects and deletes each one of them, and clears vector.
	*/
	void clearEntries();

	/*
		Function which returns how many Entry objects are stored inside Log instance.
	*/
	int size();
};

#endif