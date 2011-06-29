#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
using namespace std;

/* Note the usage of classes has been minimal at best. This is intentional
 * Mainly because there are so many different modules here that clubbing them 
 * under a hood of "class" does not seem right. It will add to complexity
 * rather than solving it .Most of them are pure utility functions
 * Parsing functions can be clubbed in a class and have bene done but purposefully
 * kept static as there is no need to create a separate parser object. Granted, I am 
 * using lot of string manipulation and it makes sense to add it in one class
 * but doing that will involve lot of code change and will take time.
 * There is one way it wont take long and it is to derive my own class from string
 * class but that usually should be avoided unless there is strong compelling reason
 * for it and in project such as this one, it can be avoided. I do admit, however,
 * that this code will need refactoring if some more changes are to be induced.
 * 
 * This was done just in right from conception to implementation 10 hours in a single stretch
 * so some mistakes are bound to happen. Nevertheless, I am proud of my creation and will strive
 * making it better. Your input is greatly appreciated.
 * */
const static string numerals[] = { "zero", "one", "two",
            "three", "four", "five", "six", "seven", "eight", "nine", "ten",
            "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen",
            "seventeen", "eighteen", "ninteen", "twenty", "thirty", "forty",
            "fifty", "sixty", "seventy", "eighty", "ninety", "hundred" };
      
const static string words[] = {  "billion", "million", "thousand" };

const static unsigned long digits[] = { 1000000000L,1000000L, 1000L };
	
double static const values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8,
            9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 30, 40, 50, 60, 70,
            80, 90, 100 };

class myUtil
{
	public:
	bool static IsMathOperator(const string s);
	void static print(vector<string> v);
	bool static Rpn(const string expression, vector<string> &output);
	bool static Calc(const string expression, double &output);
};
	

class myParser
{
	public:
	double static parseNumerals(vector<string>);
	double static parse(vector<string> text);
};
//checks if it is a mathematical operator
// also the function where we can see the actual RPN (reverse polish 
// notation) being formed. Useful for debugging if some issue with RPN 
bool myUtil::IsMathOperator(const string s)
{
	cout<<s<<" ";
	if (s=="+" || s=="-" || s=="/" || s=="*")
		return true;
	else
		return false;
}

/* Given a string expression in infix, converts it to RPN notation.
 * Output is in the form of vector of strings
 * Does the actual work. Calc function (introduced later) is wrapper for this 
 * function
 * */
bool myUtil::Rpn(const string expression, vector<string> &output)
{
	output.clear();
	char *end;
	vector<string> operator_stack;
	bool expecting_operator = false;

	for (const char *ptr = expression.c_str(); *ptr; ++ptr) {
		if ((*ptr)==32)
				continue;

		// is it a number? 
		if (!expecting_operator) {
				double number = strtod(ptr, &end);
				if (end != ptr) {
						// it's a number 
						ostringstream strs;
						strs << number;
						std::string str = strs.str();

						output.push_back(str);
						ptr = end - 1;
						expecting_operator = true;
						continue;
				}
		}

		if (*ptr == '(') {
				operator_stack.push_back("(");
				expecting_operator = false; //no operator between ()
				continue;
		}

		if (*ptr == ')') {
				while (operator_stack.size() && operator_stack.back() != "(") {
						output.push_back(operator_stack.back());
						operator_stack.pop_back();
				}

				if (!operator_stack.size())
						return false; // mismatched parenthesis 

				expecting_operator = true;
				operator_stack.pop_back(); // pop
				continue;
		}

		if (*ptr == '+' || *ptr == '-') {
				while (operator_stack.size() && myUtil::IsMathOperator(operator_stack.back())) {
						output.push_back(operator_stack.back());
						operator_stack.pop_back();
				}
				//convert to string without using Boost's famous lexical cast
				ostringstream strs;
				strs << *ptr;
				std::string str = strs.str();
	
				operator_stack.push_back(str);
				expecting_operator = false;
				continue;
		}

		if (*ptr == '*' || *ptr == '/') {
				while (operator_stack.size() && (operator_stack.back() == "*" || operator_stack.back() == "/")) {
						output.push_back(operator_stack.back());
						operator_stack.pop_back();
				}
				ostringstream strs;
				strs << *ptr;
				std::string str = strs.str();

				operator_stack.push_back(str);
				expecting_operator = false;
				continue;
		}

		// error
		return false;
	}

	while (operator_stack.size()) {
		if (!myUtil::IsMathOperator(operator_stack.back()))
				return false;

		output.push_back(operator_stack.back());
		operator_stack.pop_back();
	}

	return true;
} 



/* Wrapper for doing the operation. Returns true on success
 * Gets the infix expression and converts it to postifix and
 * then calculates
 * */
bool myUtil::Calc(const string expression, double &output)
{
	vector<string> rpn;

	if (!Rpn(expression, rpn))
		return false;

	vector<double> tmp;
	for (size_t i = 0; i < rpn.size(); ++i) {
		if (myUtil::IsMathOperator(rpn[i])) {
				if (tmp.size() < 2)
						return false;//operator needs at least 2 operands
				double two = tmp.back();
				tmp.pop_back();
				double one = tmp.back();
				tmp.pop_back();
				double result;

				switch (rpn[i][0]) {
						case '*':
								result = one * two;
								break;

						case '/':
								result = one / two;
								break;

						case '+':
								result = one + two;
								break;

						case '-':
								result = one - two;
								break;

						default:
								return false;
				}

				tmp.push_back(result); //for RPN to work this is necessary
				continue;
		}

		tmp.push_back(atof(rpn[i].c_str()));
		continue;
	}

	if (tmp.size() != 1)
		return false;

	output = (static_cast<double>(tmp.back()));
	return true;
} // Calc

//Returns value of a string word	
 double getValueOf(string word) {
	 for (unsigned int i=0;i<sizeof(numerals)/sizeof(numerals[0]);i++)
	 {
		if (word == numerals[i])
		{
			
			return values[i];
		}
	 }
        return -1;
}

/* Parse numerals is the actual function which converts a string vector into the number
 * Parse is the wrapper over Parse numerals
 * So the idea is to go through each word , if it is not "" (I replaced
 * the "and" with empty word earlier), get its value from the constant arrays
 */     
double myParser::parseNumerals(vector<string> text)
{
	vector<string>::iterator i;
	long value = 0;
	for (i=text.begin();i<text.end();i++)
	{
		// *i is the word.
		string s = *i;
		if (s!="")
		{
			double subval = getValueOf(*i);
			if (subval == 100) {
					if (value == 0)
						value = 100;
					else
						value *= 100;
				} else
					value += subval;
		}
     }

        return value;
}

/* Searches a given word in the vector of strings
 * Could have used the find algorithm by STL but it returns last in case 
 * needle is not found. I do not need that, for me a simple -1 if not found would suffice
 * Hence writing my own algo */
int search (vector<string> text, string needle)
{
	for (unsigned int i=0;i<text.size();i++)
	{
		if (text[i]==needle)
			return i;
	} 
	return -1;
}


/* Custom based routine for splitting a vector about a given position
 * for e.g. vector "a" "b" "c" "d" "e" splitted about position 2 would
 * give either vector "a" "b" or vector "c" "d" depending upon the flag
 * Could have used the STL algorithm but this one was pretty easy so thought will write myself
 */
vector<string> getVector(vector<string> text,int i, bool flag)
{
	//flag == 0 means copy from beg to i
	//flag == 1 means copy from i+1 to end
	vector<string> newv;
	if (flag == false)
	{
		for (int j=0;j<i;j++)
		{
			newv.push_back(text[j]);
		}
	}
	else
	{
		for (unsigned int j=i+1;j<text.size();j++)
		{
			newv.push_back(text[j]);
		}
	}
	return newv;
}

//print a vector of strings.
void myUtil::print(vector<string> v)
{
	vector<string>::iterator it;
		for (it=v.begin();it<v.end();it++)
		{
			cout<<	*it <<"  " ;
		}
		cout<<endl;
}

/* Wrapper for parsing a string with English words into just numbers
 * Uses parseNumerals for doing actual implementation
 * */	
double myParser::parse(vector<string> text)  {
		unsigned int i=0;
		unsigned int j1;
		//error checking
		
		for (i=0;i<text.size();i++)
		{
			//checking against numerals and words. Could have used loop. But since this is never going to
			// change, loop would have been overkill. Added comment at the declaration of the variables , just
			// in case
			if (text[i] != "and" && text[i] !="million" && text[i] !="billion" && text[i] !="thousand" ) 
			{
				for (j1=0;j1<sizeof(numerals)/sizeof(numerals[0]);j1++)
				{
					if (text[i]==numerals[j1])
					{
						break;
					}
				}
				if (j1>=sizeof(numerals)/sizeof(numerals[0]))
				{
					cout<<"Invalid input ...exiting"<<endl;
					exit(-1);
				}
			}
		}
		
		for (i=0;i<text.size();i++)
		{
			if (text[i] == "and") //replace and with empty strings. RPN will take care of this later
				text[i] = ""; // parseNumerals will also handle this
		}
	    long totalValue = 0;
        bool processed = false;
        for (unsigned int n = 0; n < sizeof(words)/sizeof(words[0]); n++) {
            int j  = search(text,words[n]);
            if (j!=-1 ) {
				// this splits a vector, essentially removing the word - mathced
				// for e.g. two thousand three hunred will match on thousand (index 1)
				// text1 will be two and text2 will be three hundred
				// text2 will be process recursively
				// text1 will be computed
				vector<string> text1 = getVector(text,j,0); 
				vector<string> text2 = getVector(text,j,1);
				//print(text1);
				//print(text2);

				//special cases
                if (text1[0]=="")
                   text1[0] = "one";
				
				if (text2.size()>0){	
                if (text2[0]=="")
                    text2[0] = "zero";
				}
                totalValue = myParser::parseNumerals(text1) * digits[n]+ parse(text2);
                processed = true;
                break;
            }
        }

        if (processed)
            return totalValue;
        else
            return myParser::parseNumerals(text);
}

// returns the English word's equivalen mathematical operator
//needed for stage 1 of parser
string convertToSymbol(string line)
{
	if (line=="plus")
	{
		return "+";
	}
	else if (line == "minus")
	{
		return "-";
	}
	else if (line == "multiply")
	{
		return "*";
	}
	else if (line == "divide")
	{
		return "/";
	}
	else
	{
		//already a mathematcal operator, return as it is 
		return line;
	}
}

int main()
{
	string line;
	bool neg=false;
			
	cout<<"Enter the exp:\n"<<endl;
	getline(cin,line);
	//to signify end 
	line+= " plus";

	// converting the string into a vector of strings for easy manipulation
	istringstream iss(line);
	vector<string> input;
	vector<string> goodinput;

	//input contains the mized input - English words and numbers
	//goodinput contains only numbers
	while (std::getline(iss, line, ' '))
	{
	
		if (line =="plus" || line =="+" || line =="minus" ||line =="-" || 
		line =="multiply" ||line =="*" ||line =="divide" || line == "/")
			{
				/* this handles cases like:
				 * 2 plus 2
				 * five hundred plus 7
				 * 7 plus five hundred
				 * five hundred plus five hundred
				 * 
				 * 
				 * Idea is to store the value before the operator expressed in word
				 * that would be "plus" in the above case and store it in input
				 * We scan from left to right and when we get an operator mentioned in word
				 * we get the value which was on its left side (stored in input value)
				 * after getting the value, we clear input vetor
				 */
				 
				if (input.size()>0){
					if (isalpha(input[0].c_str()[0]))
					{
						long d = myParser::parse(input); //parse will return the number - two becomes 2
						std::ostringstream strs;
						strs << d;
						std::string str = strs.str();
						if (neg)
							goodinput.push_back("-"+str); //now it is sanitized input (means not of English word), store it in goodinput
						else
							goodinput.push_back(str);
						neg=false;
					}
					else
					{
						if (neg)
							goodinput.push_back("-"+input[0]); //now it is sanitized input (means not of English word), store it in goodinput
						else
							goodinput.push_back(input[0]);
						neg=false;
					}
			   }
				goodinput.push_back(convertToSymbol(line)); //for mathematical operator cases
				input.clear();
			
		
		   }
		 else if (line == "with")
			; //do nothing as the Reverse polish notation function will take care of it
		 else if (line == "negative")
			neg = true;
		 else if (line=="point")
		 {
			 
			 /* Building a state machine
			  * */
			 
			 
			 
			 
			 //three point ( twenty five )
			 vector<string> t;
			double d = myParser::parse(input);
			  input.clear();
			  ostringstream strs,strs2;
				strs << d;
				string str = strs.str();
			// not done error checking here
			 std::getline(iss, line, ' '); //(
			 std::getline(iss, line, ' ');
			 while (line!=")")
			 {
				t.push_back(line);
				//cout<<"line is << "<<line<<endl;
				std::getline(iss, line, ' ');
			  }
			  			  
			  d = myParser::parse(t);
			  strs2 << d;
			  string s = strs2.str();
			  
			  input.push_back(str+"."+s);
			   
		 }
		 else if (!isalpha(line.c_str()[0]))
		 {
			 //means it was of the form "702" or something
				if (neg)
					goodinput.push_back("-"+line); //directly put into good input, no need to parse as it is already not an English word
				else
					goodinput.push_back(line);
				neg=false;
		  }
		 
		else 
				input.push_back(line); //English word, store it , will parse later
	}
	
	if (neg)
	{
		cout<<"extra negative detected..exiting"<<endl;
		return -1;
	}
	goodinput.pop_back(); //popping the extra plus added initially
	cout<<"After parsing stage (1) " <<endl;
	myUtil::print(goodinput);
	
	/* now we have expression only with numbers and mathematical operators
	 * i.e 507 * 8 + 3
	 * Convert this vector of strings in a simple string
	 * This gives another chance for us to sanitise the input
	 */
	
	double d;
	string output; //name is confusing, it is simply string form of the string vector
	for (unsigned int i=0;i<goodinput.size();i++)
	{
		output += goodinput[i];
		output += " "; //append space
	}
	cout<<"After parsing stage (2), this goes to Reverse polish notation"<<endl;
	cout<<output<<endl;
	 //calculate and return the output
	if (myUtil::Calc(output,d))
	{
		//means successful
		cout<<endl<<"Answer is -->" <<d<<endl;
	}
	else
	{
		cout<<"Please check your input"<<endl;
		return -1;
	}
	
	return 0;
}

