#include <iostream>
#include <string>
#include <vector>
#include "bigInt.h"
#define lli long long int
#define pb push_back
#define buffer vector<int>			//the container used to store our 'limitless' numbers
using namespace std;

lli bigInt::used = 0;
bigInt::bigInt()									//default constructor does no additional job
{
	sign = 0;	//default positive number
}
bigInt::bigInt(lli n, lli base)					//construct object from long long and required base
{
	if (n < 0)
		sign = 1;
	else
		sign = 0;
	n = abs(n);
	while (n)
	{
		num.pb(n%base);
		n /= base;
	}

}
bigInt::bigInt(buffer &num)							//copy constructor
{
	this->num = num;
}
bigInt::bigInt(string str)							//construct object from a numerical string
{
	if (str[0] == '-')
		sign = 1;
	else
		sign = 0;
	for (int i = str.length() - 1; i >= sign; --i)
		num.pb(int(str[i] - '0'));
}

void bigInt::add(bigInt &a, bigInt &b)				//this=a+b
{
	lli len1, len2, i, j;
	int sum, carry = 0;
	buffer num;
	len1 = a.num.size();
	len2 = b.num.size();
	for (i = 0, j = 0; i<len1 && j<len2; ++i, ++j)	//add the two number bit by bit till bits are left in both numbers
	{
		sum = a.num[i] + b.num[j] + carry;
		carry = sum / 10;
		num.pb(sum % 10);
	}
	//once bits are over in one of the numbers add carry to remaining bits in other number
	for (; i<len1; ++i)
	{
		sum = a.num[i] + carry;
		carry = sum / 10;
		num.pb(sum % 10);
	}
	for (; j<len2; ++j)
	{
		sum = b.num[j] + carry;
		carry = sum / 10;
		num.pb(sum % 10);
	}
	if (carry)
		num.pb(carry);
	this->num = num;
}
void bigInt::add(bigInt &a)								//this=this+a
{
	add(a, *this);
}

void bigInt::trim(buffer &a)				//removes leading zeroes
{
	while (a.size()>1 && a[a.size() - 1] == 0)
		a.pop_back();
}

void bigInt::sub(bigInt a, bigInt &b)
{
	lli len1, len2, i, j;
	int diff;
	buffer num;
	len1 = a.num.size();
	len2 = b.num.size();
	for (i = 0, j = 0; i<len1 && j<len2; ++i, ++j)
	{
		if (a.num[i]<b.num[i])
		{
			a.num[i] += 10;
			a.num[i + 1]--;
		}
		diff = a.num[i] - b.num[j];
		num.pb(diff);
	}
	for (; i<len1; ++i)
	{
		if (a.num[i]<0)
		{
			a.num[i] += 10;
			a.num[i + 1]--;
		}
		num.pb(a.num[i]);
	}
	trim(num);
	this->num = num;
}

buffer bigInt::prefix(buffer buff, lli n, int x)		//add 'x' n times at the end of number in buffer
{
	buffer result;
	while (n--)
		result.pb(x);
	for (int i = 0; i<buff.size(); ++i)
		result.pb(buff[i]);
	buff.clear();
	return result;
}
buffer bigInt::mult1char(buffer buff, int x)					//mutiply number by one digit
{
	lli len = buff.size(), i;
	int prod, carry = 0;
	buffer result;
	for (i = 0; i<len; ++i)
	{
		prod = buff[i] * x + carry;
		carry = prod / 10;
		result.pb(prod % 10);
	}
	if (carry)
		result.pb(carry);

	return result;
}
void bigInt::mult(bigInt &a, bigInt &b)							//this=a*b
{
	lli len1, len2, i;
	len1 = a.num.size();
	len2 = b.num.size();
	buffer M, Q, temp;									//multiplicand(M) and multiplier(Q)
	if (len1>len2)
	{
		M = a.get(); Q = b.get();
	}
	else
	{
		M = b.get(); Q = a.get();
		len1 = M.size();
		len2 = Q.size();
	}
	bigInt buff1(mult1char(M, Q[0])), buff2;

	for (i = 1; i<len2; ++i)
	{
		temp = prefix(mult1char(M, Q[i]), i, 0);				//add i trailing zeros to number	
		buff2.set(temp);
		//buff1.print();cout<<endl;
		//buff2.print();cout<<endl;
		buff1.add(buff2);
	}
	this->num = buff1.get();
	this->sign = a.sign^b.sign;
	trim(num);
}
void bigInt::mult(bigInt &a)									//this=this*a
{
	mult(a, *this);
}

void bigInt::makeEqualLength(buffer &x, buffer &y)				//equalise length of buffers to implement karatsuba multiplication
{
	buffer temp;
	int i;
	int len1 = x.size();
	int len2 = y.size();
	if (len1 < len2)
	{
		for (i = 0; i < len1; i++)
			temp.pb(x[i]);
		for (i = 0; i<abs(len1 - len2); ++i)
			temp.pb(0);
		x = temp;
	}
	else if (len1 > len2)
	{
		for (i = 0; i < len2; i++)
			temp.pb(y[i]);
		for (i = 0; i<abs(len1 - len2); ++i)
			temp.pb(0);
		y = temp;
	}
}
buffer bigInt::combine(buffer &c, buffer &b, buffer &a, lli m)		//combine the three products of karastsuba multiplication approrpiately
{
	buffer temp1, temp2;
	bigInt result, A(a), B, C;
	lli i;
	for (i = 0; i<m * 2; ++i)
	{
		temp1.pb(0);
		if (i<m)
			temp2.pb(0);
	}
	for (i = 0; i<c.size(); ++i)
		temp1.pb(c[i]);
	C.set(temp1);
	for (i = 0; i<b.size(); ++i)
		temp2.pb(b[i]);
	B.set(temp2);
	result.add(C, B);
	result.add(A);
	return result.get();
}
void bigInt::kmult(bigInt a, bigInt b)	//split a multiplication of large numbers into 3 smaller mults and some additions
{
	//efficient only for large number (greater than ~200 digits) use normal multiplication if digits in any one number is less than 200
	if (a.num.size()<200 || b.num.size()<200)
	{
		mult(a, b);
		return;
	}
	used++;
	lli len, factor, i;
	buffer x, y, x1, x2, y1, y2;
	x = a.get(); y = b.get();
	makeEqualLength(x, y);		//make both numbers of equal length by adding leading zeros to one of them
	len = x.size();
	factor = len / 2;				//splitting factor to implement karatsuba algo
	for (i = 0; i<factor; ++i)
	{
		x1.pb(x[i]);
		y1.pb(y[i]);
	}
	for (; i<len; ++i)
	{
		x2.pb(x[i]);
		y2.pb(y[i]);
	}
	if (x1.size() == 0)
		x1.pb(0);
	if (y1.size() == 0)
		y1.pb(0);
	trim(x2);
	trim(y2);
	bigInt X1(x1), X2(x2), Y1(y1), Y2(y2), Z0, Z1, Z2, temp;
	/*X1.print();
	X2.print();
	Y1.print();
	Y2.print();*/
	Z0.kmult(X1, Y1);
	Z2.kmult(X2, Y2);
	Z1.add(X1, X2);
	temp.add(Y1, Y2);
	Z1.kmult(temp);
	Z1.sub(Z1, Z0);
	Z1.sub(Z1, Z2);

	this->num = combine(Z2.get(), Z1.get(), Z0.get(), factor);
	this->sign = a.sign^b.sign;		//new sign will be xor of signs of operands
	trim(num);						//remove leading zeros
}
void bigInt::kmult(bigInt &a)									//this=this*a
{
	kmult(a, *this);
}

void bigInt::print()											//print represented number 
{
	if (sign == 1)
		cout << "-";
	for (int i = num.size() - 1; i >= 0; --i)
		cout << num[i];
	cout << endl;
}

int bigInt::compare(bigInt &A, bigInt &B)				//compare two numbers. returns -1(A<B),0(A==B),1(A>B)
{
	if (A.get().size() > B.get().size())
		return 1;
	else
	if (A.get().size() < B.get().size())
		return -1;
	buffer a = A.get(), b = B.get();
	for (int i = a.size() - 1; i >= 0; --i)
	{
		if (a[i]>b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
	}
	return 0;
}
int bigInt::compare(bigInt &B)			//compare two numbers. returns -1(A<B),0(A==B),1(A>B) here A is current number
{
	return compare(*this, B);
}

void bigInt::set(buffer &buff)									//set number manually using vector<int>
{
	this->num = buff;
}
void bigInt::set(bigInt &a)										//set number manually using another object
{
	this->num = a.get();
}
void bigInt::set(string &str)
{
	num.clear();
	for (int i = str.length() - 1; i >= 0; --i)
		num.pb(int(str[i] - '0'));
}

buffer bigInt::get()
{
	return this->num;
}

lli bigInt::kMultUsage()
{
	return used;
}


