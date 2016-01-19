#ifndef _SORTER_INCLUDED_
#define _SORTER_INCLUDED_

#include "Log.h"

/*
	Namespace which handles sorting arrays of data.

	It contains two main methods selectionSort and quickSort, along with defaultCompare, and recursive quickSort().
	Whole namespace is designed on template idea, it is created as generic as possible. Both sorts algorithms
	work with array of Type *, and both of them fetch comparing function thorugh parameters.

	Also both sort algorithms fetch Log * (this is non generic part of code), which if it is passed, is used
	to store each change in sorting process.

	@author Acko.
*/
namespace Sorter
{
	/*
		Default compare function, if none compare function is passed to sort algorithms. Type::operator<();
	*/
	template <typename Type>
	bool defaultCompare(const Type& t1, const Type& t2)
	{
		return t1 < t2;
	}

	/*
		Selection sort algorithm. 

		Basic selection sort, with added templates for Types which it sorts, and added support for Log *.
		If Log * is suplied as parameter, this sorting function tracks down its own iteration number, 
		switch number and compare number, and stores it inside Log *.
		
		Also if log is enabled, it creates new list
		of integers, which present indexes of list which function is sorting, and whenever it switches two elements
		of original list it switches two elements of new index list.

		@param items -> (Type **) array of Type pointers.
		@param size -> (size_t) size of an array to be sorted.
		@param compare -> (bool (*)(const Type&, const Type&)) (has default) pointer to a compare function.
		@param log -> (Log *) (has default) pointer to an Log instance which is used for tracking sort moves.
	*/
	template <typename Type>
	void selectionSort(Type* items[], size_t size, bool (*compare)(const Type&, const Type&) = &defaultCompare, Log* log = nullptr)
	{
		int* logList = new int[size];
		for (unsigned int i = 0; i < size; i++)
			logList[i] = i;

		int iterNum = 0, switchNumber = 0, compareNum = 0;

		log -> createEntry(logList, 0, 0, 0);

		for (unsigned int i = 0; i < size - 1; i++)
		{
			iterNum++;
			int min = i;
			for (unsigned int j = i + 1; j < size; j++)
			{
				compareNum++; iterNum++;
				if (compare(*items[j], *items[min]))
					min = j;
			}
			if (min != i)
			{
				if (log)
				{
					int temp = logList[i];
					logList[i] = logList[min];
					logList[min] = temp;

					log -> createEntry(logList, iterNum, ++switchNumber, compareNum);
				}

				Type* temp = items[i];
				items[i] = items[min];
				items[min] = temp;
			}
		}

//		if (log)
//			log -> writeData();
		
		delete[] logList;
	}

	/*
		Quick sort algorithm. 

		This is public interface function, other one is recursive call, shouldn't be called from outside.

		Basic quick sort, with added templates for T which it sorts, and added support for Log *.
		It doesn't do any random mixing on begining, and picking of pivot is trivial, just pick last from right.

		If Log * is suplied as parameter, this sorting function tracks down its own iteration number, 
		switch number and compare number, and stores it inside Log *.
		
		Also if log is enabled, it creates new list
		of integers, which present indexes of list which function is sorting, and whenever it switches two elements
		of original list it switches two elements of new index list.

		@param arr -> (T **) array of T pointers.
		@param size -> (size_t) size of an array to be sorted.
		@param compare -> (bool (*)(const Type&, const Type&)) (has default) pointer to a compare function.
		@param log -> (Log *) (has default) pointer to an Log instance which is used for tracking sort moves.
	*/
	template <typename T> 
	static void quickSort(T** arr, size_t size, bool (*compare)(const T&, const T&) = &defaultCompare, Log* log = nullptr)
	{
		if (log != nullptr)
		{
			int* indexes = new int[size];
			for (unsigned int i = 0; i < size; i++)
				indexes[i] = i;
			log -> createEntry(indexes, 0, 0, 0);

			int info[] = {0, 0, 0}; // iterNum, switchNum, compareNum
			quickSort<T>(arr, 0, size - 1, compare, log, info, indexes); // call to private function member
			// log -> writeData();
			delete[] indexes;
		}
		else
			quickSort<T>(arr, 0, size - 1, compare);
	}

	/*
		Quick Sort Algorithm Support.

		This is "private" recursive function which is called from quick sort "public" interface, and by itself.
		This function does sorting job. Upper one just provides "public" interface.

		@param arr -> (T **) array of T pointers.
		@param size -> (size_t) size of an array to be sorted.
		@param compare -> (bool (*)(const Type&, const Type&)) (has default) pointer to a compare function.
		@param log -> (Log *) (has default) pointer to an Log instance which is used for tracking sort moves.
		@param info -> (int *) (has default) pointer to an integer array with 3 values (iteration number, switch number, compare number).
		@param indexes -> (int *) (has default) pointer to an integer array which presents indexes of original list which is being sorted.
	*/
	template <typename T> 
	static void quickSort(T** arr, int first, int last, bool (*compare)(const T&, const T&) = &defaultCompare, Log* log = nullptr, int* info = nullptr, int* indexes = nullptr)
	{
		if (first >= last)
			return;

		info[0]++;
		int pivot = last;
		int p = -1;

		for (auto i = first; i <= last; i++)
		{
			if (info)
				info[2]++;
			if (compare(**(arr + i), **(arr + pivot)))
			{
				if (arr + i != arr + first + ++p)
				{
					auto temp = *(arr + i);
					*(arr + i) = *(arr + first + p);
					*(arr + first + p) = temp;
					if (info)
					{
						auto temp = *(indexes + i);
						*(indexes + i) = *(indexes + first + p);
						*(indexes + first + p) = temp;
						log -> createEntry(indexes, info[0], ++info[1], info[2]);
					}
				}
			}
		}

		if (arr + pivot != arr + first + ++p)
		{
			auto temp = *(arr + pivot);
			*(arr + pivot) = *(arr + first + p);
			*(arr + first + p) = temp;
			if (info)
			{
				auto temp = *(indexes + pivot);
				*(indexes + pivot) = *(indexes + first + p);
				*(indexes + first + p) = temp;
				log -> createEntry(indexes, info[0], ++info[1], info[2]);
			}
		}

		quickSort(arr, first, first + p - 1, compare, log, info, indexes);
		quickSort(arr, first + p + 1, last, compare, log, info, indexes);
	}
};

#endif