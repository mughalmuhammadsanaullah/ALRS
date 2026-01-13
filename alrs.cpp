#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <fstream>
#include <conio.h>
using namespace std;

//Constants
const int MAX_TOKENS=100;
const int MAX_VARS=5;
const int MAX_SIZE=50;

//Main Menu Functions
void mainMenu();
void truthTable();
void argumentValidator();
void checkEquivalence();
void viewSavedResults();
void instructions();
void showExit();

//Core Logic Functions
void tokenize(char input[], char tokens[MAX_TOKENS], int &tokenCount);
void shuntingYardAlgorithm(char tokens[MAX_TOKENS], int tokenCount, char postfix[MAX_TOKENS], int &postfixCount);
bool applyOperator(char op,int val1,int val2=0);
bool evaluatePostfix(char postfix[MAX_TOKENS],int postfixCount,char variables[],int values[],int variableCount);
int extractVariables(char tokens[MAX_TOKENS],int tokenCount,char uniqueVars[]);

//Helper Functions
bool isAlphabet(char c);
bool isOperator(char c);
int operatorPrecedence(char op);
void getBinary(int temp,int currentValues[],int variableCount);
bool isValid(char tokens[],int tokenCount);
void saveToFile(string content);
void clearScreen();

//Stack Implementation Functions
void pushChar(char stack[],int &top,char value);
char peekChar(char stack[],int top);
char popChar(char stack[],int &top);
void pushInt(int stack[],int &top,int value);
int popInt(int stack[],int &top);

int main()
{
	cout<<"-------------------------------------------------------"<<endl;
	cout<<"                 Welcome to ALRS"<<endl;
	cout<<"-------------------------------------------------------"<<endl<<endl;
	do
	{
		int choice;
		cout<<endl;
		mainMenu();
		cout<<endl<<"Enter your choice: ";
		cin>>choice;
		clearScreen();
		
		while(choice<1 || choice>6)
		{
			cout<<"Invalid Choice! Enter your choice again:"<<endl;
			cin>>choice;
		}
	
		switch(choice)
		{
			case 1:
				truthTable();
				cout<<endl<<"Press any key to continue..."<<endl;
				getch();
				clearScreen();
				break;
			case 2:
				argumentValidator();
				cout<<endl<<"Press any key to continue..."<<endl;
				getch();
				clearScreen();
				break;
			case 3:
				checkEquivalence();
				cout<<endl<<"Press any key to continue..."<<endl;
				getch();
				clearScreen();
				break;
			case 4:
				instructions();
				cout<<endl<<"Press any key to continue..."<<endl;
				getch();
				clearScreen();
				break;
			case 5:	
			    viewSavedResults();
			    cout<<endl<<"Press any key to continue..."<<endl;
				getch();
				clearScreen();
			    break;
			case 6:
				showExit();
				return 0;    
		}
	}
	while(true);
	
	
    return 0;
}

void mainMenu()
{
	cout<<"======================================================="<<endl;
	cout<<"                    Main Menu                          "<<endl;
	cout<<"======================================================="<<endl;
	cout<<"1. Truth Table Generator"<<endl;
	cout<<"2. Validate an Argument (Premises to Conclusion)"<<endl;
	cout<<"3. Check Equivalence of Two Expressions"<<endl;
	cout<<"4. Instructions"<<endl;
	cout<<"5. View Saved Results"<<endl;
	cout<<"6. Exit"<<endl<<endl;
}

void instructions()
{
	cout<<"-------------------------------------------------------"<<endl;
	cout<<"                     Instructions                      "<<endl;
	cout<<"-------------------------------------------------------"<<endl;
	cout<<"Maximum 5 variables allowed."<<endl;
	cout<<"Unknown characters (like . , etc) are ignored automatically.";
	cout<<"& for AND(^) "<<endl;     
	cout<<"| for OR(V)  "<<endl; 
	cout<<"! for NOT(~) "<<endl;
	cout<<"> for IMPLIES(->) "<<endl;
	cout<<"= for BICONDITIONAL(<->) "<<endl;
}

void getBinary(int num,int currentValues[],int variableCount)
{
	int idx=variableCount-1;
	for(int i=0;i<variableCount;i++)
	{
		currentValues[i]=0;	
	}
	while(num!=0)
	{
		int rem=num%2;
		currentValues[idx]=rem;
		num=num/2;
		idx--;
	}
}

void showExit()
{
	cout<<"Thank you for using ALRS!"<<endl;
}

void clearScreen()
{
	system("cls");
}

bool isAlphabet(char c) 
{
	bool result=((c>='A' && c<='Z') || (c>='a' && c<='z'));
    return result;
}

bool isOperator(char c) 
{
    if(c == '!' || c == '&' || c == '|' || c == '>' || c == '=') 
	{
        return true;
    }
    return false;
} 

int operatorPrecedence(char op)
{
	if(op=='!') 
		return 4;
	if(op=='&') 
		return 3;
	if(op=='|') 
		return 2;
	if(op=='>') 
		return 1;
	if(op=='=') 
		return 0;				
}

bool isValid(char tokens[],int tokenCount)
{   
    // Check if empty
    if(tokenCount==0)
    {
        cout<<endl<<"Invalid Input. Empty expression."<<endl<<endl;
        return false;
    }
    
    // Check balanced parentheses
    int parenCount=0;
    for(int i=0;i<tokenCount;i++)
    {
        if(tokens[i]=='(')
            parenCount++;
        else if(tokens[i]==')')
            parenCount--;
            
        // If more closing than opening at any point
        if(parenCount<0)
        {
            cout<<endl<<"Invalid Input. Unbalanced parentheses (extra closing parenthesis)."<<endl<<endl;
            return false;
        }
    }
    
    // If not balanced at end
    if(parenCount!=0)
    {
        cout<<endl<<"Invalid Input. Unbalanced parentheses (missing closing parenthesis)."<<endl<<endl;
        return false;
    }
    
    // Check first token - must be variable, NOT operator, or opening parenthesis
    if(!isAlphabet(tokens[0]) && tokens[0]!='!' && tokens[0]!='(')
    {
        cout<<endl<<"Invalid Input. Expression cannot start with binary operator."<<endl<<endl;
        return false;
    }
    
    // Check last token - must be variable or closing parenthesis
    if(!isAlphabet(tokens[tokenCount-1]) && tokens[tokenCount-1]!=')')
    {
        cout<<endl<<"Invalid Input. Expression cannot end with operator."<<endl<<endl;
        return false;
    }
    
    // Check consecutive variables
    for(int i=0;i<tokenCount-1;i++)
    {
        if(isAlphabet(tokens[i]) && isAlphabet(tokens[i+1]))
        {
            cout<<endl<<"Invalid Input. Two variables together (missing operator between them)."<<endl<<endl;
            return false;
        }
    }
    
    // Check consecutive operators (except NOT which is unary)
    for(int i=0;i<tokenCount-1;i++)
    {
        if(isOperator(tokens[i]) && tokens[i]!='!' && isOperator(tokens[i+1]) && tokens[i+1]!='!')
        {
            cout<<endl<<"Invalid Input. Two binary operators together."<<endl<<endl;
            return false;
        }
    }
    
    // Check empty parentheses
    for(int i=0;i<tokenCount-1;i++)
    {
        if(tokens[i]=='(' && tokens[i+1]==')')
        {
            cout<<endl<<"Invalid Input. Empty parentheses ()."<<endl<<endl;
            return false;
        }
    }
    
    // Check operator after opening parenthesis (except NOT)
    for(int i=0;i<tokenCount-1;i++)
    {
        if(tokens[i]=='(' && isOperator(tokens[i+1]) && tokens[i+1]!='!')
        {
            cout<<endl<<"Invalid Input. Binary operator immediately after opening parenthesis."<<endl<<endl;
            return false;
        }
    }
    
    // Check operator before closing parenthesis
    for(int i=1;i<tokenCount;i++)
    {
        if(isOperator(tokens[i-1]) && tokens[i-1]!='!' && tokens[i]==')')
        {
            cout<<endl<<"Invalid Input. Operator immediately before closing parenthesis."<<endl<<endl;
            return false;
        }
    }
    
    // Check variable followed by opening parenthesis
    for(int i=0;i<tokenCount-1;i++)
    {
        if(isAlphabet(tokens[i]) && tokens[i+1]=='(')
        {
            cout<<endl<<"Invalid Input. Variable followed by opening parenthesis (missing operator)."<<endl<<endl;
            return false;
        }
    }
    
    // Check closing parenthesis followed by variable
    for(int i=0;i<tokenCount-1;i++)
    {
        if(tokens[i]==')' && isAlphabet(tokens[i+1]))
        {
            cout<<endl<<"Invalid Input. Closing parenthesis followed by variable (missing operator)."<<endl<<endl;
            return false;
        }
    }
    
    // Check closing parenthesis followed by opening parenthesis
    for(int i=0;i<tokenCount-1;i++)
    {
        if(tokens[i]==')' && tokens[i+1]=='(')
        {
            cout<<endl<<"Invalid Input. )( requires an operator between them."<<endl<<endl;
            return false;
        }
    }
    
    // Check variable followed by NOT operator (missing binary operator)
    for(int i=0;i<tokenCount-1;i++)
    {
        if(isAlphabet(tokens[i]) && tokens[i+1]=='!')
        {
            cout<<endl<<"Invalid Input. Variable followed by NOT (missing binary operator)."<<endl<<endl;
            return false;
        }
    }
    
    // Check closing parenthesis followed by NOT operator (missing binary operator)
    for(int i=0;i<tokenCount-1;i++)
    {
        if(tokens[i]==')' && tokens[i+1]=='!')
        {
            cout<<endl<<"Invalid Input. Closing parenthesis followed by NOT (missing binary operator)."<<endl<<endl;
            return false;
        }
    }
    
    // Check multiple consecutive NOT operators (allowed but warn if too many)
    int consecutiveNots=0;
    for(int i=0;i<tokenCount;i++)
    {
        if(tokens[i]=='!')
        {
            consecutiveNots++;
            if(consecutiveNots>3)
            {
                cout<<endl<<"Warning: Multiple consecutive NOT operators detected."<<endl;
                break;
            }
        }
        else
        {
            consecutiveNots=0;
        }
    }
    
    // Check that binary operators have operands on both sides
    for(int i=1;i<tokenCount-1;i++)
    {
        if(isOperator(tokens[i]) && tokens[i]!='!')
        {
            // Check left side
            bool hasLeftOperand=false;
            if(isAlphabet(tokens[i-1]) || tokens[i-1]==')')
                hasLeftOperand=true;
                
            // Check right side
            bool hasRightOperand=false;
            if(isAlphabet(tokens[i+1]) || tokens[i+1]=='!' || tokens[i+1]=='(')
                hasRightOperand=true;
                
            if(!hasLeftOperand || !hasRightOperand)
            {
                cout<<endl<<"Invalid Input. Binary operator missing operand(s)."<<endl<<endl;
                return false;
            }
        }
    }

    return true;
}

void pushChar(char stack[],int &top,char value)
{
	if(top<MAX_TOKENS-1)
	{
		top++;
		stack[top]=value;
	}
	else
	{
		cout<<"Stack is already filled!"<<endl;
	}
}

char peekChar(char stack[],int top)
{
	char result;
	if(top!=-1)
	{
		result=stack[top];
		return result;
	}
	return '\0';
}

char popChar(char stack[],int &top)
{
	char result;
	if(top!=-1)
	{
		result=stack[top];
		top--;
		return result;
	}
	return '\0';
}

void pushInt(int stack[],int &top,int value)
{
    if (top<MAX_TOKENS-1) 
	{
        top++;
        stack[top]=value;
    }
}

int popInt(int stack[], int &top) 
{
    if (top>=0) 
	{
        int value=stack[top];
        top--;
        return value;
    }
    return 0;
}

void saveToFile(string content)
{
	content+="\n";
    char choice;
    cout<<"\nDo you want to save the result to a file? (Y/N): ";
    cin>>choice;
    cin.ignore();

    if(choice == 'Y' || choice == 'y')
    {
        ofstream outFile("results.txt",ios::app); // saves to results.txt in the current folder
        if(outFile.is_open())
        {
            outFile<<content;
            outFile.close();
            cout<<"Result saved successfully in 'results.txt'\n";
        }
        else
        {
            cout<<"Error opening file!\n";
        }
    }
}


void tokenize(char input[], char tokens[MAX_TOKENS], int &tokenCount)
{
	tokenCount=0;
	int length=strlen(input);
	for (int i=0;i<length;i++) 
	{
		char current=input[i];
        // Ignore spaces
        if (current==' ') 
		{
            continue;
        }
        // If it's a variable
        if (isAlphabet(current)) 
		{
            // Convert to uppercase for consistency (case-insensitive)
            if(current>='a' && current<='z')
            {
                current=current-'a'+'A';
            }
            tokens[tokenCount]=current;
            tokenCount++;
        }
        // If it's an operator or parenthesis
        else if (isOperator(current) || current=='(' || current==')')
		{
            tokens[tokenCount]=current;
            tokenCount++;
        }
        else 
		{
            // Invalid character found
            cout <<"Ignoring unknown character: "<<current<<endl;
        }
    }
}

void shuntingYardAlgorithm(char tokens[MAX_TOKENS], int tokenCount, char postfix[MAX_TOKENS], int &postfixCount)
{
	char operatorStack[MAX_TOKENS];
	int top=-1;
	postfixCount=0;
	for (int i=0;i<tokenCount;i++) 
	{
        char currentTokenChar = tokens[i];

        if(isAlphabet(currentTokenChar)) //Alphabets(Variables):Directly adding to output
		{
            postfix[postfixCount]=currentTokenChar;
            postfixCount++;
        }
        else if(currentTokenChar == '(')//'(' :Adding to operatorStack
		{
            pushChar(operatorStack,top,currentTokenChar);
        }
        else if(currentTokenChar == ')')//')' :Popping all operators from operatorStack to output until '(' 
		{
            while (top != -1 && peekChar(operatorStack, top) != '(') 
			{
                postfix[postfixCount] = popChar(operatorStack,top);
                postfixCount++;
            }
            // Pop the '(' but not adding it to the output
            if (top != -1) 
			{
                popChar(operatorStack, top);
            }
        }
        else if(isOperator(currentTokenChar))
		{
            // While stack top has higher or equal precedence, pop it to output
            while (top != -1 && peekChar(operatorStack, top) != '(' && operatorPrecedence(peekChar(operatorStack,top)) >= operatorPrecedence(currentTokenChar)) 
			{
                postfix[postfixCount]= popChar(operatorStack, top);
                postfixCount++;
            }
            // Push the current operator onto the stack
            pushChar(operatorStack, top, currentTokenChar);
        }
    }
    while(top!=-1) 
	{
        postfix[postfixCount]= popChar(operatorStack, top);
        postfixCount++;
    }
}

bool applyOperator(char op,int val1,int val2)
{
	switch(op)
	{
		case '!':
			return (!val1);
		case '&':
			return (val1 && val2);
		case '|':
			return (val1 || val2);
		case '>':
			return (!val1 || val2);
		case '=':
			return val1==val2;
	}
}

bool evaluatePostfix(char postfix[MAX_TOKENS], int postfixCount, char variables[], int values[], int variableCount)
{
	int evalStack[MAX_TOKENS];
	int top=-1;
	for(int i=0;i<postfixCount;i++)
	{
		char token=postfix[i];
		if(isAlphabet(token))
		{
			int val=0;
			for(int j=0;j<variableCount;j++)
			{
				if(variables[j]==token)
				{
					val=values[j];
					break;
				}
			}
			pushInt(evalStack,top,val);
		}
		else if(isOperator(token))
		{
			if (token == '!') 
			{
            int operand=popInt(evalStack,top);
            bool result=applyOperator(token,operand);
            pushInt(evalStack,top,result);
            }
			else
			{
				int operand2 =popInt(evalStack,top);
				int operand1 =popInt(evalStack,top);
				bool result=applyOperator(token,operand1,operand2);
				pushInt(evalStack,top,result);
			}
		}
	}
	
	return popInt(evalStack,top);//This is the final result of the expression
}

int extractVariables(char tokens[MAX_TOKENS], int tokenCount, char uniqueVars[])
{
    int count=0;
    for (int i=0;i<tokenCount;i++) 
	{
        if(isAlphabet(tokens[i]))
		{
            char currentVariable=tokens[i];
            bool alreadyExists=false; //Check if we already have this variable
            for(int j=0;j<count;j++) 
			{
                if(uniqueVars[j]==currentVariable) 
				{
                    alreadyExists=true;
                    break;
                }
            }
            
            if(!alreadyExists) // If new then add it
			{
                if (count<MAX_VARS) 
				{
                    uniqueVars[count]=currentVariable;
                    count++;
                } 
				else
				{
                    cout<<"Invalid Expression!"<<endl;
                    cout<<"This system only supports up to 5 variables."<<endl;
                    return -1; 
                }
            }
        }
    }
    return count;
}

void truthTable() 
{
	cout<<endl<<"----------------------------------------------------------"<<endl;
    cout<<"               TRUTH TABLE GENERATOR"<<endl;
    cout<<"----------------------------------------------------------"<<endl;
    char input[MAX_SIZE];
    char tokens[MAX_TOKENS];
    char postfix[MAX_TOKENS];
    char uniqueVars[MAX_VARS];
    int tokenCount,postfixCount,variableCount;
    string outputToFile="";

    cout<<"Enter a logical expression using P, Q, R..."<<endl;
    cout<<"Enter Expression: ";
    cin.ignore();
    cin.getline(input,MAX_SIZE);
    
    //1.Tokenize
    tokenize(input,tokens,tokenCount);
    if(tokenCount==0)
	{
        cout<<"You entered an empty expression."<<endl;
        return;
    }
    bool valid=isValid(tokens,tokenCount);
    if(!valid)
    {
    	cout<<"Invalid Expression!"<<endl<<"Enter expression again!"<<endl;
    	cin.getline(input,MAX_SIZE);
    	tokenize(input,tokens,tokenCount);
	}

    //2.Finding all unique Variables
    variableCount=extractVariables(tokens,tokenCount,uniqueVars);
    if(variableCount == -1) return; // Stop if too many vars

    //3.Convert to Postfix
    shuntingYardAlgorithm(tokens,tokenCount,postfix,postfixCount);

    char header[500]="";
    int i;
    // Add variables to header
    for(i=0;i<variableCount;i++)
    {
    	char current[2]={uniqueVars[i],'\0'};
    	strcat(header,current);
    	strcat(header,"\t");
	}
	strcat(header,"| Result");
    
    cout<<"Postfix: "<<postfix<<endl;
    cout<<endl<<header<<endl;
    outputToFile=outputToFile+header;
    outputToFile=outputToFile+"\n";
    cout<< "-------------------------------------------------" << endl;

    // Step 5: Iterate through all Truth Combinations
    int rows = pow(2, variableCount);
    int currentValues[MAX_VARS];

    for (int i=0;i<rows;i++) 
	{
        char rowString[500]="";
        int temp=i;
		getBinary(temp,currentValues,variableCount);
        // Generate T/F values based on bits of 'i'
        for (int j=0;j<variableCount;j++)
		{ 
            // Add to display string
            if (currentValues[j]==1) 
				strcat(rowString,"T\t");
            else 
				strcat(rowString,"F\t");
        }

        // Evaluate the expression for this row
        bool rowResult = evaluatePostfix(postfix,postfixCount,uniqueVars,currentValues,variableCount);
        
        strcat(rowString,"| ");
        if(rowResult) 
			strcat(rowString,"T");
        else 
			strcat(rowString,"F");

        // Print to screen
        cout<<rowString<<endl;
        outputToFile=outputToFile+rowString;
        outputToFile=outputToFile+"\n";
    }
    
    outputToFile=outputToFile+"\nTruth table generated successfully.\n\n";
    cout<<"Truth table generated successfully."<<endl;
    saveToFile(outputToFile);
}

void checkEquivalence()
{
	cout<<endl<<"----------------------------------------------------------"<<endl;
    cout<<"                EQUIVALENCE CHECKER"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
    char input1[MAX_SIZE];
    char input2[MAX_SIZE];
    char tokens1[MAX_TOKENS];
    char tokens2[MAX_TOKENS];
    char postfix1[MAX_TOKENS];
    char postfix2[MAX_TOKENS];
    char uniqueVars1[MAX_VARS];
    char uniqueVars2[MAX_VARS];
    int tokenCount1,postfixCount1,variableCount1,tokenCount2,postfixCount2,variableCount2;
    bool valid1,valid2;
    string outputToFile="";

    cout<<"Enter first logical expression: ";
    cin.ignore();
    cin.getline(input1,MAX_SIZE);
    cout<<"Enter second logical expression: ";
    cin.getline(input2,MAX_SIZE);
    
    
    //1.Tokenize
    tokenize(input1,tokens1,tokenCount1);
    valid1=isValid(tokens1,tokenCount1);
    if(!valid1)
    {
    	cout<<"Invalid Expression!"<<endl<<"Enter expression again!"<<endl;
    	cin.getline(input1,MAX_SIZE);
    	tokenize(input1,tokens1,tokenCount1);
	}
	
	tokenize(input2,tokens2,tokenCount2);
	valid2=isValid(tokens2,tokenCount2);
    if(!valid2)
    {
    	cout<<"Invalid Expression!"<<endl<<"Enter expression again!"<<endl;
    	cin.getline(input1,MAX_SIZE);
    	tokenize(input2,tokens2,tokenCount2);
	}
    if(tokenCount1==0 || tokenCount2==0)
	{
        cout<<"You entered an empty expression."<<endl;
        return;
    }

    //2.Finding all unique Variables
    variableCount1=extractVariables(tokens1,tokenCount1,uniqueVars1);
    variableCount2=extractVariables(tokens2,tokenCount2,uniqueVars2);
    if(variableCount1 == -1 || variableCount2==-1) return; // Stop if too many vars

    //3.Convert to Postfix
    shuntingYardAlgorithm(tokens1,tokenCount1,postfix1,postfixCount1);
	shuntingYardAlgorithm(tokens2,tokenCount2,postfix2,postfixCount2);
    char header[500]="";
    int i;
    // Add variables to header
    int maxVars;
    bool isFirstBig=false;
    if(variableCount1>variableCount2)
    {
    	maxVars=variableCount1;
    	isFirstBig=true;
	}
	else
	{
		maxVars=variableCount2;
		isFirstBig=false;
	}
	
    for(i=0;i<maxVars;i++)
    {
    	char current[2];
    	if(isFirstBig)
    	{
    		current[0]=uniqueVars1[i];
    		current[1]='\0';
		}
    	else
    	{
    		current[0]=uniqueVars2[i];
    		current[1]='\0';
		}
    	strcat(header,current);
    	strcat(header,"\t");
	}
	strcat(header,"| Result1");
	strcat(header,"\t");
	strcat(header,"| Result2");
	outputToFile=outputToFile+header;
	outputToFile=outputToFile+"\n";
	
    cout<<"Postfix1: "<<postfix1<<endl;
    cout<<"Postfix2: "<<postfix2<<endl;
    cout<<endl<<header<<endl;
    cout<< "--------------------------------------------------------------" << endl;

    // Step 5: Iterate through all Truth Combinations
    int rows=pow(2,maxVars);
    int currentValues[MAX_VARS];
	bool result[rows][2];
	
    for (int i=0;i<rows;i++) 
	{
        char rowString[500]="";
        int temp=i;
		getBinary(temp,currentValues,maxVars);
        // Generate T/F values based on bits of 'i'
        for (int j=0;j<maxVars;j++)
		{ 
            // Add to display string
            if (currentValues[j]==1) 
				strcat(rowString,"T\t");
            else 
				strcat(rowString,"F\t");
        }

        // Evaluate the expressions for this row
        bool rowResult1 = evaluatePostfix(postfix1,postfixCount1,uniqueVars1,currentValues,variableCount1);
        result[i][0]=rowResult1;
		bool rowResult2 = evaluatePostfix(postfix2,postfixCount2,uniqueVars2,currentValues,variableCount2);        
		result[i][1]=rowResult2;
		
        strcat(rowString,"| ");
        if(rowResult1) 
			strcat(rowString,"T\t");
        else 
			strcat(rowString,"F\t");

		strcat(rowString,"\t| ");
        if(rowResult2) 
			strcat(rowString,"T\t");
        else 
			strcat(rowString,"F\t");
        // Print to screen
        cout<<rowString<<endl;
        outputToFile=outputToFile+rowString;
        outputToFile=outputToFile+"\n";
    }
    
    bool isEqual=true;
    
    for(int i=0;i<rows;i++)
    {
    	if(result[i][0]!=result[i][1])
    	{
    		isEqual=false;
    		break;
		}
	}
	
    if(isEqual)
    {
    	cout<<"The expressions are logically EQUIVALENT!"<<endl;
    	outputToFile=outputToFile+"The expressions are logically EQUIVALENT!\n\n";
	}
	else
	{
		cout<<"The expressions are NOT logically EQUIVALENT!"<<endl;
		outputToFile=outputToFile+"The expressions are NOT logically EQUIVALENT!\n\n";
	}
	saveToFile(outputToFile);
}

void argumentValidator()
{
	cout<<endl<<"----------------------------------------------------------"<<endl;
    cout<<"                    ARGUMENT VALIDATOR     "<<endl;
	cout<<"----------------------------------------------------------"<<endl;
	
	int numPremises;
	cout<<"Enter number of premises (max 5): ";
	cin>>numPremises;
	
	while(numPremises<1 || numPremises>5)
	{
		cout<<"Invalid number! Enter between 1 and 5: ";
		cin>>numPremises;
	}
	
	char premises[5][MAX_SIZE];
	char conclusion[MAX_SIZE];
	char tokens[5][MAX_TOKENS];
	char conclusionTokens[MAX_TOKENS];
	char postfix[5][MAX_TOKENS];
	char conclusionPostfix[MAX_TOKENS];
	int tokenCounts[5];
	int postfixCounts[5];
	int conclusionTokenCount;
	int conclusionPostfixCount;
	
	char allVars[MAX_VARS];
	int varCount=0;
	
	string outputToFile="";
	
	cin.ignore();
	
	// Input premises
	for(int i=0;i<numPremises;i++)
	{
		cout<<"Enter premise "<<(i+1)<<": ";
		cin.getline(premises[i],MAX_SIZE);
		
		// Tokenize and validate
		tokenize(premises[i],tokens[i],tokenCounts[i]);
		if(!isValid(tokens[i],tokenCounts[i]))
		{
			cout<<"Invalid premise! Enter again: ";
			i--;
			continue;
		}
		
		// Extract variables from this premise
		char tempVars[MAX_VARS];
		int tempCount=extractVariables(tokens[i],tokenCounts[i],tempVars);
		
		// Add new variables to allVars
		for(int j=0;j<tempCount;j++)
		{
			bool exists=false;
			for(int k=0;k<varCount;k++)
			{
				if(allVars[k]==tempVars[j])
				{
					exists=true;
					break;
				}
			}
			if(!exists && varCount<MAX_VARS)
			{
				allVars[varCount]=tempVars[j];
				varCount++;
			}
		}
		
		// Convert to postfix
		shuntingYardAlgorithm(tokens[i],tokenCounts[i],postfix[i],postfixCounts[i]);
	}
	
	// Input conclusion
	cout<<"Enter conclusion: ";
	cin.getline(conclusion,MAX_SIZE);
	tokenize(conclusion,conclusionTokens,conclusionTokenCount);
	
	if(!isValid(conclusionTokens,conclusionTokenCount))
	{
		cout<<"Invalid conclusion!"<<endl;
		return;
	}
	
	// Extract variables from conclusion
	char tempVars[MAX_VARS];
	int tempCount=extractVariables(conclusionTokens,conclusionTokenCount,tempVars);
	for(int j=0;j<tempCount;j++)
	{
		bool exists=false;
		for(int k=0;k<varCount;k++)
		{
			if(allVars[k]==tempVars[j])
			{
				exists=true;
				break;
			}
		}
		if(!exists && varCount<MAX_VARS)
		{
			allVars[varCount]=tempVars[j];
			varCount++;
		}
	}
	
	shuntingYardAlgorithm(conclusionTokens,conclusionTokenCount,conclusionPostfix,conclusionPostfixCount);
	
	// Build header
	char header[500]="";
	for(int i=0;i<varCount;i++)
	{
		char temp[2]={allVars[i],'\0'};
		strcat(header,temp);
		strcat(header,"\t");
	}
	for(int i=0;i<numPremises;i++)
	{
		strcat(header,"| P");
		char num[2]={(char)('1'+i),'\0'};
		strcat(header,num);
		strcat(header,"\t");
	}
	strcat(header,"| Conc\t| Valid");
	
	cout<<endl<<header<<endl;
	outputToFile=outputToFile+header;
	outputToFile=outputToFile+"\n";
	cout<<"--------------------------------------------------------------"<<endl;
	
	// Generate truth table
	int rows=pow(2,varCount);
	int currentValues[MAX_VARS];
	bool isValidArgument=true;
	
	for(int i=0;i<rows;i++)
	{
		char rowString[500]="";
		getBinary(i,currentValues,varCount);
		
		// Print variable values
		for(int j=0;j<varCount;j++)
		{
			if(currentValues[j]==1)
				strcat(rowString,"T\t");
			else
				strcat(rowString,"F\t");
		}
		
		// Evaluate all premises
		bool allPremisesTrue=true;
		for(int p=0;p<numPremises;p++)
		{
			bool premiseResult=evaluatePostfix(postfix[p],postfixCounts[p],allVars,currentValues,varCount);
			strcat(rowString,"| ");
			if(premiseResult)
				strcat(rowString,"T\t");
			else
			{
				strcat(rowString,"F\t");
				allPremisesTrue=false;
			}
		}
		
		// Evaluate conclusion
		bool conclusionResult=evaluatePostfix(conclusionPostfix,conclusionPostfixCount,allVars,currentValues,varCount);
		strcat(rowString,"| ");
		if(conclusionResult)
			strcat(rowString,"T\t");
		else
			strcat(rowString,"F\t");
		
		// Check validity: if all premises are true, conclusion must be true
		strcat(rowString,"| ");
		if(allPremisesTrue && !conclusionResult)
		{
			isValidArgument=false;
			strcat(rowString,"X");
		}
		else
		{
			strcat(rowString,"-");
		}
		
		cout<<rowString<<endl;
		outputToFile=outputToFile+rowString;
		outputToFile=outputToFile+"\n";
	}
	
	cout<<endl;
	if(isValidArgument)
	{
		cout<<"The argument is VALID!"<<endl;
		cout<<"(In all cases where premises are true, conclusion is also true)"<<endl;
		outputToFile=outputToFile+"\nThe argument is VALID!\n\n";
	}
	else
	{
		cout<<"The argument is INVALID!"<<endl;
		cout<<"(There exists a case where premises are true but conclusion is false)"<<endl;
		outputToFile=outputToFile+"\nThe argument is INVALID!\n\n";
	}
	
	saveToFile(outputToFile);
}

void viewSavedResults()
{
    ifstream inFile("results.txt");
    
    if(!inFile.is_open())
    {
        cout<<"No saved results found or file could not be opened."<<endl;
        return;
    }
    
	cout<<endl<<"------------------------------------------------"<<endl;
    cout<<"             Saved Results"<<endl;
	cout<<"------------------------------------------------"<<endl;
	
    string line;
    while(getline(inFile, line))
    {
        cout<<line<<"\n";
    }

    inFile.close();
}

