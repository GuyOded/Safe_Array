/* SafePointer.h
 *
 * README
 *
 *	A class representing a safe pointer.
 *	The members forth and back are representing the number
 *	of memory blocks that are allowed to be accessed.
 *	example:
 *
 *	blocks of array: 1 2 3 4 5 6 7
 *	                 ^     ^
 *	                 |     |
 *	                 |     |
 *	             m_first m_pointer
 *	      m_forth=3 There are 3 blocks ahead
 *	      m_back=3  There are 3 blocks to the back
 * If the user is trying to advance more than m_forth an exception
 * is raised. Same for m_back. out_of_range
 *
 * If the user uses the following operators: (pointer - pointer), >, <, >=, <= on pointers that don't
 * have the same m_first, an exception is raised - range_error.
 *
 * operator+ is implemented using a templated friend function to prevent implicit conversion
 * of parameter int. Keep in mind that by using this approach operator+<int> is not friend
 * with SafePointer<char>. This approach is preferable over defining a new template inside the
 * class for the function as it allows operator+<int> to be a friend to SafePointer<char>.
 *
 * For the (pointer - pointer) == > < >= <= I used the Barton-Nackman trick in order
 * to allow implicit conversion from SafeArray to SafePointer.
 *
 *  Created on: Feb 5, 2016
 *      Author: guy
 */

#ifndef SAFEPOINTER_H_
#define SAFEPOINTER_H_

#include "SafeArray.h"
#include <stdexcept>

//Declaration for + operator
template<class T> class SafePointer;

//declare operator+ as a template function so we could then use it in the class
template<class T>
SafePointer<T> operator+(int step, SafePointer<T>& safe_pointer);

template<class T>
SafePointer<T> operator+(SafePointer<T>& safe_pointer, int step);

template<class T> class SafePointer {
private:
	int m_forth;
	T* m_pointer;
	T* m_first;//In order to check if the given pointer arguments are
			   //in fact pointers in the same array(for pointer arithmetic
			   //p1 - p2). It was originally a constant pointer but due to the
			   //copy constructor
	int m_back;
public:
	SafePointer(T* pointer, int forth, T* first);
	SafePointer(const SafePointer<T>& ptr);
	SafePointer(const SafeArray<T>& sa);
	virtual ~SafePointer();

	SafePointer operator++(int);//increment postfix
	SafePointer& operator++();//prefix

	SafePointer operator--(int);//decrement postfix
	SafePointer& operator--();//prefix

	T& operator*();//dereference operator
	SafePointer operator+=(int step);
	SafePointer operator-=(int step);
	SafePointer operator-(int step);//we must block int - p as it is logically incorrect
	SafePointer operator=(SafePointer<T> operand);

	//WTF??? -https://gcc.gnu.org/bugzilla/show_bug.cgi?id=17122
	//I have zero understanding as to what the hell is going on...
	//Why must the function be declared like that?
	//Found an article about that here http://web.mst.edu/~nmjxv3/articles/templates.html

	//match template instantiation of operator<T> with the instantiation of SafePointer
	friend SafePointer<T> (::operator+ <T>)(int steps, SafePointer<T>& safe_pointer);//addition operators
	friend SafePointer<T> (::operator+ <T>)(SafePointer<T>& safe_pointer, int steps);

	/* In order for the implicit conversion from array to pointer to work properly
	*  we must inject a friend function. I didn't find any other way to do this
	*  while working with templates, but I actually like it even though it requires
	*  the implementation inside the declaration. Found it in StackOverflow.
	*  I believe that the Barton–Nackman trick is also related (I am not sure if it is
	*  actually it though)*/

	/**
	 * safe pointer subtraction
	 *
	 * @param lhs-left hand operand
	 * @param rhs-right hand operand
	 * @return the amount of blocks lhs is from rhs
	 */
	friend int operator-(const SafePointer<T>& lhs, const SafePointer<T>& rhs){
		if(lhs.m_first != rhs.m_first)
				throw std::range_error("pointers provided are not within a correct range");

		return lhs.m_pointer - rhs.m_pointer;
	}

	/**
	 * pointer equality
	 *
	 * @param lhs-left hand operand
	 * @param rhs-right hand operand
	 * @return true if the pointers are pointing to the same block
	 */
	friend bool operator==(const SafePointer<T>& lhs, const SafePointer<T>& rhs){

		return lhs.m_pointer == rhs.m_pointer;
	}

	/**
	 * pointer bigger than
	 * @param lhs-left hand operand
	 * @param rhs-right hand operand
	 * @return true if the memory block of lhs has a higher address (I guess) than rhs
	 */
	friend bool operator>(const SafePointer<T>& lhs, const SafePointer<T>& rhs){

		if(lhs.m_first != rhs.m_first)
				throw std::range_error("pointers provided are not within a correct range");

		return lhs.m_pointer > rhs.m_pointer;
	}

	/**
	 * pointer smaller than
	 * @param lhs-left hand operand
	 * @param rhs-right hand operand
	 * @return true if the memory block of rhs has a lower address than rhs
	 */
	friend bool operator<(const SafePointer<T>& lhs, const SafePointer<T>& rhs){

		if(lhs.m_first != rhs.m_first)
				throw std::range_error("pointers provided are not within a correct range");

		return rhs > lhs;
	}

	/**
	 * pointer bigger than or equals to
	 * @param lhs-left hand operand
	 * @param rhs-right hand operand
	 * @return true if lhs equals to or bigger than rhs
	 */
	friend bool operator>=(const SafePointer<T>& lhs, const SafePointer<T>& rhs){
		if(rhs == lhs)
				return true;

		if(lhs.m_first != rhs.m_first)
			throw std::range_error("pointers provided are not within a correct range");

		return rhs > lhs;
	}

	/**
	 * pointer smaller than or equals to
	 * @param lhs-left hand operand
	 * @param rhs-right hand oprand
	 * @return true if lhs == rhs or lhs < rhs
	 */
	friend bool operator<=(const SafePointer<T>& lhs, const SafePointer<T>& rhs){
		if(rhs == lhs)
				return true;

		if(lhs.m_first != rhs.m_first)
			throw std::range_error("pointers provided are not within a correct range");

		return rhs < lhs;
	}
};

/**
 * The most common constructor that is used.
 * If the user wish to use this constructor, it is his
 * responsibility to provide correct values to the parameters.
 *
 * @param pointer-the pointer that is used for dereferencing and all that
 * @param forth-number of blocks
 * @param first-the reference point of the array: if first > pointer a range_error is raised
 */
template<class T>
SafePointer<T>::SafePointer(T* pointer, int forth,T* first )
			  :m_forth(forth), m_pointer(pointer), m_first(first){
	if(pointer - first < 0)
		throw std::range_error("pointers provided are not within a correct range");

	m_back = pointer - first;
}

/**
 * a copy constructor
 *
 * @param ptr
 */
template<class T>
SafePointer<T>::SafePointer(const SafePointer<T>& ptr)
			  :m_forth(ptr.m_forth), m_pointer(ptr.m_pointer), m_first(ptr.m_first)
{
	m_back = ptr.m_pointer - ptr.m_first;
}

/**
 * a conversion constructor from SafeArray to safe pointer
 * first and pointer is m_array, forth is the array length, back is 0
 * @param sa
 */
template<class T>
SafePointer<T>::SafePointer(const SafeArray<T>& sa)
{
	SafePointer temp = (sa + 1);
	m_pointer = temp.m_first;
	m_first = temp.m_first;
	m_forth = sa.getLength() - 1;
	m_back = 0;
}

/**
 * a default destructor
 */
template<class T>
SafePointer<T>::~SafePointer() {
}

/**
 * postfix increment operator
 * @param
 * @return a copy of the pointer before the incremention
 */
template<class T>
SafePointer<T> SafePointer<T>::operator++(int)
{
	SafePointer temp(*this);

	++*this;

	return temp;
}

/**
 * prefix increment
 * @return a reference to *this after increment
 */
template<class T>
SafePointer<T>& SafePointer<T>::operator ++()
{
	if(m_forth == 0)
		throw std::out_of_range("Out of bounds from front");

	m_back++;
	m_forth--;
	m_pointer++;

	return *this;
}

/**
 * postfix decrement
 * @param none
 * @return a copy of the pointer before decremention
 */
template<class T>
SafePointer<T> SafePointer<T>::operator--(int)
{
	SafePointer temp = *this;

	--*this;

	return temp;
}

/**
 * prefix decrement
 * @return a reference to *this after decremntion
 */
template<class T>
SafePointer<T>& SafePointer<T>::operator--()
{
	if(m_first == m_pointer)
		throw std::out_of_range("Out of bounds from back");

	m_back--;
	m_forth++;
	m_pointer--;

	return *this;
}

/**
 * dereference operator
 * @return a reference to the actual value pointed to by m_pointer
 */
template<class T>
T& SafePointer<T>::operator *()
{
	return *m_pointer;
}

/**
 * addition assignment operator
 * Maybe I should've returned a reference to allow chaining but
 * it would seem a little complex.
 * @param step-number of blocks to advance the pointer by.
 * @return a copy of the *this after it was advanced
 */
template<class T>
SafePointer<T> SafePointer<T>::operator+=(int step)
{
	if(step > m_forth)
		throw std::out_of_range("Out of bounds from front");

	m_back += step;
	m_forth -= step;
	m_pointer += step;

	return *this;
}

/**
 * subtraction assignment operator
 * @param step-number of blocks to ... the pointer by
 * @return a copy of the pointer after assignment
 */
template<class T>
SafePointer<T> SafePointer<T>::operator-=(int step)
{
	if(step > m_back)
		throw std::out_of_range("Out of bounds from back");

	m_back -= step;
	m_forth += step;
	m_pointer -= step;

	return *this;
}

/**
 * (pointer - int) subtraction operator
 * @param step - number of blocks
 * @return a copy of the pointer if it was subtracted
 */
template<class T>
SafePointer<T> SafePointer<T>::operator-(int step)
{
	SafePointer<T> temp = *this;

	temp -= step;

	return temp;
}

/**
 * copy assignment operator
 * @param operand the assigned pointers
 * @return a copy of *this
 */
template<class T>
SafePointer<T> SafePointer<T>::operator=(SafePointer<T> operand)
{
	this->m_back = operand.m_back;
	this->m_forth = operand.m_forth;
	this->m_pointer = operand.m_pointer;
	this->m_first = operand.m_first;

	return *this;
}

/**
 * pointer+int operator
 * @param step-number of blocks
 * @param safe_pointer-the pointer operand
 * @return a copy of the pointer as if it was advanced
 */
template<class T>
SafePointer<T> operator+(int step, SafePointer<T>& safe_pointer)
{
	SafePointer<T> temp = safe_pointer;

	temp += step;

	return temp;
}

/**
 * int + pointer
 * same as pointer + int
 */
template<class T>
SafePointer<T> operator+(SafePointer<T>& safe_pointer, int step)
{
	SafePointer<T> temp = safe_pointer;

	temp += step;

	return temp;
}

#endif /*SAFEPOINTER_H_*/
