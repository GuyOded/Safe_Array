#include <iostream>
#include "SafeArray.h"
#include <exception>

SafeArray<char> string_to_array(const char a[], int length);
void print_char_array(SafeArray<char>& arr);
void s_copy(SafeArray<char>& dst, SafeArray<char>& src);
bool s_equality2(SafeArray<char>& t, SafeArray<char>& p);
bool s_equality1(SafeArray<char>& t, SafeArray<char>& p);
bool s_equality3(SafePointer<char> s, SafePointer<char> t);

int main()
{
	using namespace std;
	SafeArray<char> str1 = string_to_array("hello", 8);
	print_char_array(str1);
	SafeArray<char> str2 = str1;
	std::cout << "\n" << s_equality1(str1, str2) << std::endl;
	std::cout << s_equality2(str2, str2) << std::endl;
	std::cout << s_equality3(str1, str2) << endl;

	try
	{
		str1 + 9;
	}
	catch(const std::out_of_range& e)
	{
		cout << e.what() << endl;
	}

	try
	{
		SafePointer<char> sp = (str1 + 9);
	}
	catch(const exception& e)
	{
		cout << e.what() << endl;
	}

	SafePointer<char> sp1 = str1 + 5;
	try
	{
		sp1++;
	}
	catch(const exception& e)
	{
		cout << e.what() << endl;
	}

	//same behavior for < > <= >=
	SafePointer<char> sp2 = str2 + 2;
	try
	{
		sp2 - sp1;
	}
	catch(const exception& e)
	{
		cout << e.what() << endl;
	}

	cout << (sp2 == sp1) << endl;

	sp2--;
	sp2--;

	//check pointers equality
	cout << (str2 == sp2) << endl;
	cout << *str2 << *sp2;

	return 0;
}

bool s_equality1(SafeArray<char>& t, SafeArray<char>& p)
{
	SafePointer<char> pt = t;
	SafePointer<char> pp = p;
	int limit = 0;
	if(((limit = t.getLength()) != p.getLength()))
		return false;

	for(int i = 0; i < limit; i++)
	{
		if(*(pp + i) != *(pt + i))
			return false;
	}

	return true;
}

bool s_equality2(SafeArray<char>& t, SafeArray<char>& p)
{
	int limit = 0;
	if(((limit = t.getLength()) != p.getLength()))
		return false;

	for(int i = 0; i < limit; ++i)
	{
		if(*(t + i) != *(p + i))
			return false;
	}
	return true;
}

bool s_equality3(SafePointer<char> s, SafePointer<char> t)
{
	for(; *s == *t; s++, t++)
		if(*s == '\0')
			return true;
	return false;
}

void s_copy(SafeArray<char>& dst, SafeArray<char>& src)
{
	int min_length = dst.getLength() >= src.getLength() ? src.getLength() : dst.getLength();

	for(int i = 0; i < min_length; i++)
	{
		dst[i] = src[i];
	}
}

void print_char_array(SafeArray<char>& arr)
{
	for(int i = 0; (i < arr.getLength()) && (arr[i] != '\0'); i++)
	{
		std::cout << arr[i];
	}
}

SafeArray<char> string_to_array(const char a[], int length)
{
	SafeArray<char> str(length);
	for(int i = 0; a[i] != '\0'; ++i)
	{
		str[i] = a[i];
	}
	return str;
}
