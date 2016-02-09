/*
 * SafeArray.h
 * A class representing a safe array.
 *
 * Note that the operator+ returns a SafePointer. Keep
 * that in mind when you want to get pointer to some random element in the array.
 *
 *  Created on: Feb 6, 2016
 *      Author: guy
 */

#ifndef SAFEARRAY_H_
#define SAFEARRAY_H_

#include <stdexcept>
//For the SafePointer to recognize SafeArray the deceleration is above
//The inclusion of SafePointer.h
template<class T> class SafeArray;

#include "SafePointer.h"

template<class T>
SafePointer<T> operator+(int step, const SafeArray<T>& arr);

template<class T>
SafePointer<T> operator+(const SafeArray<T>& arr, int step);

template<class T> class SafeArray {
private:
	int m_length;
	static const int MAX_LENGTH = 300;//maximum possible length
	static const int MIN_LENGTH = 1;//minimum possible length
	static const int MIN_INDEX = 0;//minimal accessible index
	void inRange(int index) const;//range check for particular object
	T* m_array;//The actual array - Warning: it is dynamically allocated in the constructor

public:
	SafeArray(int length);
	SafeArray(const SafeArray& safe_array);
	virtual ~SafeArray();
	T getElementAt(int index) const;
	void setElementAt(T value, int index);
	int getLength() const;

	T& operator[](int index);
	SafeArray& operator=(const SafeArray& operand);
	T& operator*();

	//<T> is redundant since c++ assumes that every reference to the class in decleration
	//is templated. To be explicit I added it.

	//In this case no implicit conversions are needed so I prefer to use
	//this option.

	friend SafePointer<T> (::operator+ <T>)(int step, const SafeArray<T>& arr);
	friend SafePointer<T> (::operator+ <T>)(const SafeArray<T>& arr, int step);
};

/**
 * A normal constructor
 * Allocates an array with length elements - they are not initialized.
 * @param length-the length of the array
 */
template<class T>
SafeArray<T>::SafeArray(int length) : m_length(length)
{
	if(length > SafeArray<T>::MAX_LENGTH || length < SafeArray<T>::MIN_LENGTH)
		throw std::out_of_range("Length provided is out of range of"
								"acceptable values");

	m_array = new T[length];
}

/**
 * A copy constructor.
 * I provided this one since I am dynamically allocating an array
 * @param safe_array-the copied array
 */
template<class T>
SafeArray<T>::SafeArray(const SafeArray<T>& safe_array)
{
	this->m_length = safe_array.m_length;
	this->m_array = new T[m_length];
	for(int i = 0; i < this->m_length; i++)
	{
		this->m_array[i] = safe_array.m_array[i];
	}
}

/**
 * a destructor - deletes the allocated array element
 */
template<class T>
SafeArray<T>::~SafeArray()
{
	delete[] m_array;
}

/**
 * return the contents of m_array[index]
 * @param index-index in the array
 * @return the contents of the element at the given index
 */
template<class T>
T SafeArray<T>::getElementAt(int index) const
{
	try
	{
		inRange(index);
	}
	catch(const std::out_of_range& e)
	{
		throw(e);
	}

	return m_array[index];
}

/**
 * sets m_array[index] to value
 *
 * @param value
 * @param index
 */
template<class T>
void SafeArray<T>::setElementAt(T value, int index)
{
	try
	{
		inRange(index);
	}
	catch(const std::out_of_range& e)
	{
		throw e;
	}

	m_array[index] = value;
}

/**
 * @return the length of the array
 */
template<class T>
int SafeArray<T>::getLength() const
{
	return this->m_length;
}

/**
 * overload [] operator
 * used like an array
 * @param index
 * @return a reference to m_array[index]
 */
template<class T>
T& SafeArray<T>::operator [](int index)
{
	try
	{
		inRange(index);
	}
	catch(const std::out_of_range& e)
	{
		throw e;
	}
	return m_array[index];
}

/**
 * overload dereference operator
 * used like a regular array
 * @return
 */
template<class T>
T& SafeArray<T>::operator*()
{
	return m_array[0];
}

/**
 * a copy assignment operator
 * @param operand-the copied array
 * @return a reference of *this
 */
template<class T>
SafeArray<T>& SafeArray<T>::operator =(const SafeArray<T>& operand)
{
	this->m_length = operand.m_length;
	this->m_array = new T[m_length];
	for(int i = 0; i < this->m_array; i++)
	{
		this->m_array[i] = operand.m_array[i];
	}
	return *this;
}

/**
 * raises out_of_range exception if index isn't within range
 * @param index
 */
template<class T>
void SafeArray<T>::inRange(int index) const
{
	if(index >= m_length || index < SafeArray<T>::MIN_INDEX)
		throw std::out_of_range("index is out of range");
}

/**
 * overload addition operator
 * used like a regular array except a SafePointer object is returned
 * @param step
 * @param arr
 * @return a safe pointer that points to m_array[index]
 */
template<class T>
SafePointer<T> operator+(int step, const SafeArray<T>& arr)
{
	try
	{
		arr.inRange(step);
	}
	catch(const std::out_of_range& e)
	{
		throw e;
	}

	return SafePointer<T>(arr.m_array + step, arr.m_length - step, arr.m_array);
}

template<class T>
SafePointer<T> operator+(const SafeArray<T>& arr, int step)
{
	try
	{
		arr.inRange(step);
	}
	catch(const std::out_of_range& e)
	{
		throw e;
	}

	return SafePointer<T>(arr.m_array + step, arr.m_length - step, arr.m_array);
}

#endif /* SAFEARRAY_H_ */
