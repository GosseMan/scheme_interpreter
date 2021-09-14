#include <iostream>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
const int HashTableSize = 1031;
const int StackSize = 20;
const int MemorySize = 31;

class Memory {
public:
	int lchild = 0;
	int rchild = 0;
	int flag = 0;
	bool isFreeList = true;
};

class Hash {
public:
	string symbol;
	int LinkofValue = 0;
};

class StackClass {
public:
	int symbol = NULL;
	int value = 0;
	bool isFull = false;
};

Memory* MemoryArray = new Memory[MemorySize];
Hash* HashTable = new Hash[HashTableSize];
StackClass* Stack = new StackClass[StackSize];

void Push(int pushsymbol, int pushvalue, StackClass stack[StackSize]) {
	for (int i = 0; i < 10; i++) {
		if (stack[i].isFull == false) {
			stack[i].symbol = pushsymbol;
			stack[i].value = pushvalue;
			stack[i].isFull = true;
			break;
		}
	}
}

void POP(StackClass stack[StackSize]) {
	for (int i = 9; i >= 0; i--) {
		if (stack[i].isFull == true) {
			stack[i].isFull = false;
			HashTable[-stack[i].symbol].LinkofValue = stack[i].value;
		}
	}
}

void flagcheck(Memory MemoryArray[MemorySize], int lambdaroot) {
	int check = lambdaroot;
	do {
		MemoryArray[check].flag = 1;
		if (MemoryArray[check].lchild > 0) {
			flagcheck(MemoryArray, MemoryArray[check].lchild);
		}
		check = MemoryArray[check].rchild;
	} while (check != 0);
}
void nowcheck(Memory MemoryArray[MemorySize], int root) {
	int check = root;
	do {
		MemoryArray[check].flag = 0;
		if (MemoryArray[check].lchild > 0) {
			nowcheck(MemoryArray, MemoryArray[check].lchild);
		}
		check = MemoryArray[check].rchild;
	} while (check != 0);
}

void GarbageCollector() {
	cout << "----- Garbage Collecting -----" << endl;
	cout << "] ";
	for (int i = 1; i < MemorySize; i++) {
		MemoryArray[i].flag = 0;
	}
	for (int i = 1; i < MemorySize; i++) {
		if (MemoryArray[i].lchild == -18) {
			flagcheck(MemoryArray, i);
		}
	}
	for (int i = 0; i < StackSize; i++) {
		if (Stack[i].value > 0) {
			flagcheck(MemoryArray, Stack[i].value);
		}
	}
	for (int i = 1; i < MemorySize; i++) {
		if (MemoryArray[i].flag == 0) {
			MemoryArray[i].isFreeList = true;
		}
	}
}

string parse(string input) {
	string a;
	for (int i = 0; i < input.size(); i++) {
		int a = input[i];
		if (a >= 'A' && a <= 'Z') {
			input[i] = a + 32;
		}
	}
	for (int i = 0; i < input.length(); i += 1) {
		a = input[i];
		if (a == "(") {
			input.replace(i, 1, "( ");
		}
		else if (a == ")") {
			input.replace(i, 1, " )");
			i += 1;
		}
		else if (a == "'") {
			input.replace(i, 1, " ' ");
			i += 1;
		}
	}
	return input;
}

int AsciiSum(string token) {
	int asciisum = 0;
	for (int i = 0; i <= token.length(); i++) {
		asciisum = asciisum + pow(int(token[i]), 2);
	}
	return asciisum % HashTableSize;
}

int GetHashValue(string token, Hash hashtable[HashTableSize]) {
	for (int i = 1; i <= 18; i++) {
		if (token == HashTable[i].symbol) {
			return -i;
		}
	}
	int asciisum = AsciiSum(token);
	if (hashtable[asciisum].symbol.empty()) {
		hashtable[asciisum].symbol = token;
		return -asciisum;
	}
	else if (hashtable[asciisum].symbol == token) {
		return -asciisum;
	}
	else {
		int check = asciisum + 1;
		while (!HashTable[check].symbol.empty()) {
			if (hashtable[check].symbol == token) {
				return -check;
			}
			check++;
		}
		hashtable[check].symbol = token;
		return -check;
	}
}

bool isnum(string input) {
	bool isnumber = true;
	if (input.empty()) {
		return false;
	}
	char check[10];
	strcpy_s(check, input.c_str());
	for (int i = 0; i < input.length(); i++) {
		if ((check[i]<'0' || check[i]>'9') && check[i] != '.' && check[i] != '+' && check[i] != '-') {
			return false;
		}
	}
	return true;
}

//GetVal : HashValue -> Value
double GetVal(int HashValue) {
	if (HashTable[-HashValue].LinkofValue == NULL) {
		string check = HashTable[-HashValue].symbol;
		bool numcheck = isnum(check);
		if (numcheck) {
			return stod(check);
		}
	}
	else {
		return GetVal(-HashTable[-HashValue].LinkofValue);
	}

}

int Alloc() {
	for (int i = 1; i < MemorySize; i++) {
		if (MemoryArray[i].isFreeList == true)
			return i;
	}
	GarbageCollector();
	for (int i = 1; i < MemorySize; i++) {
		if (MemoryArray[i].isFreeList == true)
			return i;
	}
	cout << "메모리 확장이 필요합니다." << endl;
	return 10000;
}

string GetNextToken(string tokenstore[50], bool isNext, bool isFinal) {
	static int tokenpos = 0;
	string str = tokenstore[tokenpos];
	if (isNext == true) {
		tokenpos++;
	}
	else if (isNext == false) {
		tokenpos--;
	}
	if (isFinal == true) {
		tokenpos = 0;
	}
	return str;
}

int Read(string tokenstore[50]) {
	int root = 0;
	bool first = true;
	int temp;
	string token = GetNextToken(tokenstore, true, false);
	int thv = GetHashValue(token, HashTable);
	if (thv == GetHashValue("(", HashTable)) {
		token = GetNextToken(tokenstore, true, false);
		thv = GetHashValue(token, HashTable);
		while (GetHashValue(token, HashTable) != GetHashValue(")", HashTable)) {
			if (first == true) {
				temp = Alloc();
				root = temp;
				first = false;
			}
			else {
				MemoryArray[temp].rchild = Alloc();
				temp = MemoryArray[temp].rchild;
			}

			if (thv == GetHashValue("(", HashTable)) {
				token = GetNextToken(tokenstore, false, false);
				MemoryArray[temp].isFreeList = false;
				MemoryArray[temp].flag = 1;
				MemoryArray[temp].lchild = Read(tokenstore);


			}
			else {
				MemoryArray[temp].lchild = thv;
				MemoryArray[temp].isFreeList = false;
				MemoryArray[temp].flag = 1;
			}
			MemoryArray[temp].rchild = 0;
			token = GetNextToken(tokenstore, true, false);
			thv = GetHashValue(token, HashTable);

		}
		nowcheck(MemoryArray, root);
		return root;
	}
	else {
		return thv;
	}
}

void Print(int root, bool startList) {
	if (root == NULL) {
		cout << "()";
	}
	else if (root == 10000) {
		cout << "ERROR";
	}
	else if (root < 0) {
		cout << HashTable[-root].symbol;
	}
	else if (root > 0) {
		if (startList == true) {
			cout << "(";
		}
		Print(MemoryArray[root].lchild, true);
		if (MemoryArray[root].rchild != 0) {
			cout << " ";
			Print(MemoryArray[root].rchild, false);
		}
		else {
			cout << ")";
		}
	}
}

string PP(string newcommand, string parsedinput) {
	string token;
	stringstream ss(parsedinput);
	while (!ss.eof()) {
		ss >> token;
		if (token == "define") {
			newcommand = newcommand + " " + token;
			ss >> token;
			if (token == "(") {
				ss >> token;
				newcommand = newcommand + " " + token;
				string temp = "";
				string blank = "";
				while (ss >> token) {
					temp = temp + " " + token;
				}
				newcommand = newcommand + " ( lambda (" + PP(blank, temp) + " )";
			}
			else {
				newcommand = newcommand + " " + token;
			}
		}
		else if (token == "'") {
			newcommand = newcommand + " ( quote";
			int numberL = 0;
			do {
				ss >> token;
				newcommand = newcommand + " " + token;
				if (token == "(") {
					numberL++;
				}
				else if (token == ")") {
					numberL--;
				}
			} while (numberL > 0);
			newcommand = newcommand + " )";
		}
		else {
			newcommand = newcommand + " " + token;
		}
	}
	return newcommand;
}


void PrintHash(Hash HashTable[HashTableSize]) {
	cout << "Hash Table" << endl;
	cout << "    HashValue    l     Symbol     |    Link" << endl;
	cout << "--------------------------------------------" << endl;
	for (int i = 0; i < 1000; i++) {
		if (!HashTable[i].symbol.empty()) {
			cout << "    ";
			cout.width(5);
			cout << -i;
			cout << "        l  ";
			cout.width(10);
			cout << HashTable[i].symbol << "    l    " << HashTable[i].LinkofValue << endl;
		}
	}
	cout << endl;
}

void PrintMemory(Memory MemoryArray[MemorySize]) {
	cout << endl << "Tree Array" << endl;
	cout << "   Node    l Left Child l Right Childㅣ  Flag   " << endl;
	cout << "--------------------------------------------------" << endl;

	for (int i = 1; i < MemorySize; i++) {
		cout << "    ";
		cout.width(2);
		cout << i;
		cout << "     l  ";
		cout.width(5);
		cout << MemoryArray[i].lchild;
		cout << "     l     ";
		cout.width(2);
		cout << MemoryArray[i].rchild;
		cout << "     |     " << MemoryArray[i].flag;
		//cout << "     |     " << MemoryArray[i].isFreeList;
		cout << endl;
	}
}

int EVAL(int root) {
	if (root > 0) {
		int token_index = MemoryArray[root].lchild;
		//PLUS
		if (token_index == -5) {

			if (!isnum(HashTable[-EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].symbol)) {
				cout << "Error : 숫자가 아닙니다." << endl;
				return 10000;
			}
			else if (!isnum(HashTable[-EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild)].symbol)) {
				cout << "Error : 숫자가 아닙니다." << endl;
				return 10000;
			}
			else {
				double isint = GetVal(EVAL(MemoryArray[MemoryArray[root].rchild].lchild)) + GetVal(EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild));
				if (int(isint) == isint) {
					return GetHashValue(to_string(int(isint)), HashTable);
				}
				return GetHashValue(to_string(GetVal(EVAL(MemoryArray[MemoryArray[root].rchild].lchild)) + GetVal(EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild))), HashTable);
			}
		}
		//MINUS
		else if (token_index == -6) {
			if (!isnum(HashTable[-EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].symbol)) {
				cout << "Error : 숫자가 아닙니다." << endl;
				return 10000;
			}
			else if (!isnum(HashTable[-EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild)].symbol)) {
				cout << "Error : 숫자가 아닙니다." << endl;
				return 10000;
			}
			else {
				double isint = GetVal(EVAL(MemoryArray[MemoryArray[root].rchild].lchild)) - GetVal(EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild));
				if (int(isint) == isint) {
					return GetHashValue(to_string(int(isint)), HashTable);
				}
				return GetHashValue(to_string(GetVal(EVAL(MemoryArray[MemoryArray[root].rchild].lchild)) + GetVal(EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild))), HashTable);
			}
		}
		//TIMES
		else if (token_index == -7) {
			if (!isnum(HashTable[-EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].symbol)) {
				cout << "Error : 숫자가 아닙니다." << endl;
				return 10000;
			}
			else if (!isnum(HashTable[-EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild)].symbol)) {
				cout << "Error : 숫자가 아닙니다." << endl;
				return 10000;
			}
			else {
				double isint = GetVal(EVAL(MemoryArray[MemoryArray[root].rchild].lchild)) * GetVal(EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild));
				if (int(isint) == isint) {
					return GetHashValue(to_string(int(isint)), HashTable);
				}
				return GetHashValue(to_string(GetVal(EVAL(MemoryArray[MemoryArray[root].rchild].lchild)) + GetVal(EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild))), HashTable);
			}
		}
		//isNumber
		else if (token_index == -8) {
			if (isnum(HashTable[-EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].symbol) == true) {
				return -3;
			}
			else {
				return -4;
			}
		}
		//isSYMBOL
		else if (token_index == -9) {
			if (isnum(HashTable[-EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].symbol) == false) {
				return -3;
			}
			else {
				return -4;
			}
		}

		//isNULL
		else if (token_index == -10) {
			if (MemoryArray[root].rchild == NULL || EVAL(MemoryArray[MemoryArray[root].rchild].lchild) == NULL || (MemoryArray[MemoryArray[root].rchild].lchild < 0 && HashTable[-MemoryArray[MemoryArray[root].rchild].lchild].LinkofValue == 0)) {
				return -3;
			}
			else {
				return -4;
			}
		}

		//CONS
		else if (token_index == -11) {
			if (EVAL(MemoryArray[MemoryArray[root].rchild].lchild) != 0) {
				int newmemory = Alloc();
				MemoryArray[newmemory].isFreeList = false;
				MemoryArray[newmemory].lchild = EVAL(MemoryArray[MemoryArray[root].rchild].lchild);
				MemoryArray[newmemory].rchild = EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild);
				return newmemory;
			}
			else {
				return MemoryArray[root].rchild;
			}
		}

		//COND
		else if (token_index == -12) {
			while (MemoryArray[MemoryArray[root].rchild].rchild != NULL) {

				root = MemoryArray[root].rchild;
				if (EVAL(MemoryArray[MemoryArray[root].lchild].lchild) == -3) {

					return EVAL(MemoryArray[MemoryArray[MemoryArray[root].lchild].rchild].lchild);
				}
			}
			if (MemoryArray[MemoryArray[MemoryArray[root].rchild].lchild].lchild != -13) {
				return 10000;
			}
			return EVAL(MemoryArray[MemoryArray[MemoryArray[MemoryArray[root].rchild].lchild].rchild].lchild);
		}

		//CAR
		else if (token_index == -14) {
			return MemoryArray[EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].lchild;
		}

		//CDR
		else if (token_index == -15) {
			return MemoryArray[EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].rchild;
		}
		//DEFINE
		else if (token_index == -16) {
			int temp = 0;
			if (MemoryArray[MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild].lchild == GetHashValue("lambda", HashTable)) {
				HashTable[-MemoryArray[MemoryArray[root].rchild].lchild].LinkofValue = MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild;
			}
			else {
				HashTable[-MemoryArray[MemoryArray[root].rchild].lchild].LinkofValue = EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild);
			}
			return MemoryArray[MemoryArray[root].rchild].lchild;
		}
		//QUOTE
		else if (token_index == -17) {
			return MemoryArray[MemoryArray[root].rchild].lchild;
		}
		//USER_DEFINED_FUNCTION
		else if (token_index < 0 && MemoryArray[HashTable[-token_index].LinkofValue].lchild == -18) {
			int inputroot = MemoryArray[MemoryArray[HashTable[-token_index].LinkofValue].rchild].lchild;
			//Push && Set Parameter by Function Argument
			int tempval[10];
			Push(MemoryArray[inputroot].lchild, HashTable[-MemoryArray[inputroot].lchild].LinkofValue, Stack);
			tempval[0] = EVAL(MemoryArray[MemoryArray[root].rchild].lchild);
			int cycle = 1;
			while (MemoryArray[inputroot].rchild != NULL)
				while (MemoryArray[inputroot].rchild != NULL) {
					inputroot = MemoryArray[inputroot].rchild;
					root = MemoryArray[root].rchild;
					Push(MemoryArray[inputroot].lchild, HashTable[-MemoryArray[inputroot].lchild].LinkofValue, Stack);
					tempval[cycle] = EVAL(MemoryArray[MemoryArray[root].rchild].lchild);
					cycle++;
				}
			inputroot = MemoryArray[MemoryArray[HashTable[-token_index].LinkofValue].rchild].lchild;

			for (int i = 0; i < cycle; i++) {
				HashTable[-MemoryArray[inputroot].lchild].LinkofValue = tempval[i];
				inputroot = MemoryArray[inputroot].rchild;
			}
			int result = EVAL(MemoryArray[MemoryArray[MemoryArray[HashTable[-token_index].LinkofValue].rchild].rchild].lchild);

			for (int i = 0; i < cycle; i++) {
				POP(Stack);
			}
			return result;
		}
		else {
			return token_index;
		}
	}
	else {
		int check = HashTable[-root].LinkofValue;
		if (check > 0) {
			return check;
		}
		else {
			while (check != 0) {
				if (HashTable[-check].LinkofValue == 0) {
					return check;
				}
				else {
					check = HashTable[-check].LinkofValue;
				}
			}
		}
		return root;
	}
}


int main() {
	HashTable[1].symbol = "(";
	HashTable[2].symbol = ")";
	HashTable[3].symbol = "#t";
	HashTable[4].symbol = "#f";
	HashTable[5].symbol = "+";
	HashTable[6].symbol = "-";
	HashTable[7].symbol = "*";
	HashTable[8].symbol = "number?";
	HashTable[9].symbol = "symbol?";
	HashTable[10].symbol = "null?";
	HashTable[11].symbol = "cons";
	HashTable[12].symbol = "cond";
	HashTable[13].symbol = "else";
	HashTable[14].symbol = "car";
	HashTable[15].symbol = "cdr";
	HashTable[16].symbol = "define";
	HashTable[17].symbol = "quote";
	HashTable[18].symbol = "lambda";
	for (int i = 0; i < MemorySize - 1; i++) {
		MemoryArray[i].rchild = i + 1;
	}
	while (1) {
		string input;
		cout << "> ";
		getline(cin, input);
		string ParsedInput = parse(input);
		string token;
		int i = 0;
		string tokenstore[50];
		string newcommand = "";
		stringstream ss(PP(newcommand, ParsedInput));
		while (ss >> token) {
			tokenstore[i] = token;
			i++;
			GetHashValue(token, HashTable);
		}
		cout << "] ";
		int root = Read(tokenstore);
		string final = GetNextToken(tokenstore, true, true);
		int result = EVAL(root);
		Print(result, true);
		cout << endl;
		cout << "Free list's root=" << Alloc() << endl;
		cout << "List's root=" << root << endl;
		PrintMemory(MemoryArray);
		PrintHash(HashTable);
		/*
		for (int i = 1; i < MemorySize; i++) {
			MemoryArray[i].isFreeList = true;
		}
		*/
		cin.clear();
	}
}

