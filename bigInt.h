#include <string>
#include <vector>
#define lli long long int
#define pb push_back
#define buffer vector<int>			
using namespace std;


class bigInt
{
private:
	buffer num;												//stores the number in the a vector<int>
public:
	int sign;												//sign=0 indicates positive number; sign=1 indicates negative number
	lli used;												//keeps a track of the nuber of times kMult was used
	bigInt();												//default constructor does no additional job
	bigInt(lli n, lli base = 10);							//construct object from long long and required base
	bigInt(buffer &num);									//copy constructor
	bigInt(string str);										//construct object from a numerical string
	
	void add(bigInt &a, bigInt &b);							//this=a+b
	void add(bigInt &a);									//this=this+a

	void trim(buffer &a);									//removes leading zeroes

	void sub(bigInt a, bigInt &b);							//this=a-b
	
	static buffer prefix(buffer buff, lli n, int x);		//add 'x' n times at the end of number in buffer

	buffer mult1char(buffer buff, int x);					//mutiply number by one digit
	void mult(bigInt &a, bigInt &b);						//this=a*b
	void mult(bigInt &a);									//this=this*a

	void makeEqualLength(buffer &x, buffer &y);				//equalise length of buffers to implement karatsuba multiplication
	buffer combine(buffer &c, buffer &b, buffer &a, lli m);	//combine the three products of karastsuba multiplication approrpiately
	void kmult(bigInt a, bigInt b);							//split a multiplication of large numbers into 3 smaller mults and some additions
	void kmult(bigInt &a);									//this=this*a

	void print();											//print represented number 

	static int compare(bigInt &A, bigInt &B);				//compare two numbers. returns -1(A<B),0(A==B),1(A>B)
	int compare(bigInt &B);									//compare two numbers. returns -1(A<B),0(A==B),1(A>B) here A is current number

	void set(buffer &buff);									//set number manually using vector<int>
	void set(bigInt &a);									//set number manually using another bigInt object
	void set(string &str);									//set number manually using numerical string

	buffer get();											//returns the number contained in current object
	lli kMultUsage();										//returns the number of times kMult was used
};

