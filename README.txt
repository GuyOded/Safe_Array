Explanation about the classes:

SafePointer - A class representing a pointer that will throw an exception if its predetermined boundaries are crossed.
There are two members that determine the boundaries:
m_forth - the number of blocks available in the forward direction
m_back - the number of blocks available from the back
example:

blocks of array: 1 2 3 4 5 6 7
                 ^     ^
                 |     |
                 |     |
             m_first m_pointer
     m_forth=3 There are 3 blocks ahead
     m_back=3  There are 3 blocks to the back

In addition to these members there are two more pointer members:
m_pointer - the current element that is being pointed to.

m_first - the first element in the array of blocks. It has couple of uses:
1. m_back is calculated using m_first: m_back = m_pointer - m_first
2. It is used as a reference to determine whether two safe pointers are in the same array.

Note that if m_first provided in the constructor is bigger than m_pointer, the constructor will raise
an exception - range_error.

The SafePointer supports all operations that are supported by vanilla pointers(with the exception (maybe) of assignment chaining
(I had a bad feeling about that for some reason)).

Note: SafePointer has a conversion constructor from SafeArray, so you may utilize it as you wish

Warning: If the user wish to use the SafePointer without a SafeArray the members m_forth, m_pointer and m_first must be
	 provided with correct values, or an unexpected behavior might be encountered.

Examples:

1.
int arr[3];
SafePointer<int> p(arr + 1, 1, arr);

2.
SafeArray<char> arr(10);
SafePointer<char> p1 = 3 + arr;//SafeArray addition operator returns a SafePointer, explained later
SafePointer<char> p2 = arr;//conversion constructor
SafePointer<char> p3 = SafeArray<char>(10);//conversion constructor
try
{
	p3 + 1000;
}
catch(out_of_range& e)
{
	cout << e.what();
}

3.
char c = *p;
//exception may be thrown
p++;
p--;
--p;
++p;
p += 2;
p -= 2;

4.
try
{
	int n = p1 - p2;//p1 > p2 | p1 < p2 | p1 >= p2 | p1 <= p2 | (p1 == p2 - doesn't throw, all the rest do)
}
catch(range_error& e)
{
	cout << e.what();
}
5.
//exception may be thrown
*(3 + p);
*(p + 3);
*(p - 3);

6.
p2 = p3;//copy assignment operator

SafeArray - A class representing a safe array that throws exception if its boundaries are crossed.

The SafeArray has two members: length and an array
m_array - This is a pointer to a type and it is dynamically allocated in the constructor.

Usage examples:
1.
SafeArray<int> sarr(10);
*sarr = 20;
sarr[0] = 28;
*(sarr + 8) = 250 + 3;
*(8 + sarr) = 3;
SafeArray<int> bla = sarr;//copy assignment
bla.getLength() == 0;


















