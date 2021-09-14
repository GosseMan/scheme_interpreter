#include <iostream>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
const int HashTableSize = 1000;
const int StackSize = 10;

class Memory {
public:
	int lchild = 0;
	int rchild = 0;
	bool isFreeList = true;
};

class Hash {
public:
	string symbol;
	int LinkofValue = NULL;
};
class StackClass {
public:
	int symbol = NULL;
	int value = 0;
	bool isFull = false;
};

Memory* MemoryArray = new Memory[30];
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
	for (int i = 0; i < token.length(); i++) {
		asciisum += token[i];
	}
	return asciisum;
}

int GetHashValue(string token, int asciisum, Hash hashtable[HashTableSize]) {
	if (hashtable[asciisum].symbol.empty()) {
		hashtable[asciisum].symbol = token;
		return -asciisum;
	}
	else if (hashtable[asciisum].symbol == token) {
		return -asciisum;
	}
	else {
		return GetHashValue(token, AsciiSum(token) + 1, hashtable);
	}
}

//GetVal : HashValue -> Value
double GetVal(int HashValue) {
	if (HashTable[-HashValue].LinkofValue == NULL) {
		string check = HashTable[-HashValue].symbol;
		char  ch[10];
		bool isnum = false;
		if (stod(check) != 0 || check == "0") {
			isnum = true;
		}
		if (isnum) {
			return stod(check);
		}
	}
	else {
		return GetVal(-HashTable[-HashValue].LinkofValue);
	}

}

int Alloc() {
	for (int i = 1; i < 30; i++) {
		if (MemoryArray[i].isFreeList == true)
			return i;
	}
	cout << "메모리가 부족합니다." << endl;
	return 10000;
}

string GetNextToken(string tokenstore[30], bool isNext, bool isFinal) {
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

int Read(string tokenstore[30]) {
	int root = 0;
	bool first = true;
	int temp;
	string token = GetNextToken(tokenstore, true, false);
	int thv = GetHashValue(token, AsciiSum(token), HashTable);
	if (thv == GetHashValue("(", AsciiSum("("), HashTable)) {
		token = GetNextToken(tokenstore, true, false);
		thv = GetHashValue(token, AsciiSum(token), HashTable);
		while (GetHashValue(token, AsciiSum(token), HashTable) != GetHashValue(")", AsciiSum(")"), HashTable)) {
			if (first == true) {
				temp = Alloc();
				root = temp;
				first = false;
			}
			else {
				MemoryArray[temp].rchild = Alloc();
				temp = MemoryArray[temp].rchild;
			}

			if (thv == GetHashValue("(", AsciiSum("("), HashTable)) {
				token = GetNextToken(tokenstore, false, false);
				MemoryArray[temp].isFreeList = false;
				MemoryArray[temp].lchild = Read(tokenstore);


			}
			else {
				MemoryArray[temp].lchild = thv;
				MemoryArray[temp].isFreeList = false;
			}
			MemoryArray[temp].rchild = 0;
			token = GetNextToken(tokenstore, true, false);
			thv = GetHashValue(token, AsciiSum(token), HashTable);
		}
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

int EVAL(int root) {
	if (root > 0) {
		int token_index = MemoryArray[root].lchild;
		//PLUS
		if (token_index == GetHashValue("+", AsciiSum("+"), HashTable)) {

			if (!isnum(HashTable[-EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].symbol)) {
				cout << "Error : 숫자가 아닙니다." << endl;
				return 10000;
			}
			else if (!isnum(HashTable[-EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild)].symbol)) {
				cout << "Error : 숫자가 아닙니다." << endl;
				return 10000;
			}
			else {
				string result = to_string(GetVal(EVAL(MemoryArray[MemoryArray[root].rchild].lchild)) + GetVal(EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild)));
				return GetHashValue(result, AsciiSum(result), HashTable);
			}
		}
		//MINUS
		else if (token_index == GetHashValue("-", AsciiSum("-"), HashTable)) {
			if (!isnum(HashTable[-EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].symbol)) {
				cout << "Error : 숫자가 아닙니다." << endl;
				return 10000;
			}
			else if (!isnum(HashTable[-EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild)].symbol)) {
				cout << "Error : 숫자가 아닙니다." << endl;
				return 10000;
			}
			else {
				string result = to_string(GetVal(EVAL(MemoryArray[MemoryArray[root].rchild].lchild)) - GetVal(EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild)));
				return GetHashValue(result, AsciiSum(result), HashTable);
			}
		}
		//TIMES
		else if (token_index == GetHashValue("*", AsciiSum("*"), HashTable)) {
			if (!isnum(HashTable[-EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].symbol)) {
				cout << "Error : 숫자가 아닙니다." << endl;
				return 10000;
			}
			else if (!isnum(HashTable[-EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild)].symbol)) {
				cout << "Error : 숫자가 아닙니다." << endl;
				return 10000;
			}
			else {
				string result = to_string(GetVal(EVAL(MemoryArray[MemoryArray[root].rchild].lchild)) * GetVal(EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild)));
				return GetHashValue(result, AsciiSum(result), HashTable);
			}
		}
		//isNumber
		else if (token_index == GetHashValue("number?", AsciiSum("number?"), HashTable)) {
			if (isnum(HashTable[-EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].symbol) == true) {
				return GetHashValue("#t", AsciiSum("#t"), HashTable);
			}
			else {
				return GetHashValue("#f", AsciiSum("#f"), HashTable);
			}
		}
		//isSYMBOL
		else if (token_index == GetHashValue("symbol?", AsciiSum("symbol?"), HashTable)) {
			if (isnum(HashTable[-EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].symbol) == false) {
				return GetHashValue("#t", AsciiSum("#t"), HashTable);
			}
			else {
				return GetHashValue("#f", AsciiSum("#f"), HashTable);
			}
		}

		//isNULL
		else if (token_index == GetHashValue("null?", AsciiSum("null?"), HashTable)) {
			if (MemoryArray[root].rchild == NULL || EVAL(MemoryArray[root].rchild) == NULL) {
				return GetHashValue("#t", AsciiSum("#t"), HashTable);
			}
			else {
				return GetHashValue("#f", AsciiSum("#f"), HashTable);
			}
		}

		//CONS
		else if (token_index == GetHashValue("cons", AsciiSum("cons"), HashTable)) {
			int newmemory = Alloc();
			MemoryArray[newmemory].lchild = EVAL(MemoryArray[MemoryArray[root].rchild].lchild);
			MemoryArray[newmemory].rchild = EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild);
			return newmemory;

		}

		//COND
		else if (token_index == GetHashValue("cond", AsciiSum("cond"), HashTable)) {
			while (MemoryArray[MemoryArray[root].rchild].rchild != NULL) {
				root = MemoryArray[root].rchild;
				if (EVAL(MemoryArray[MemoryArray[root].lchild].lchild) == GetHashValue("#t", AsciiSum("#t"), HashTable)) {

					return EVAL(MemoryArray[MemoryArray[MemoryArray[root].lchild].rchild].lchild);
				}
			}
			if (MemoryArray[MemoryArray[MemoryArray[root].rchild].lchild].lchild != GetHashValue("else", AsciiSum("else"), HashTable)) {
				return 10000;
			}
			return EVAL(MemoryArray[MemoryArray[MemoryArray[MemoryArray[root].rchild].lchild].rchild].lchild);
		}

		//CAR
		else if (token_index == GetHashValue("car", AsciiSum("car"), HashTable)) {
			return MemoryArray[EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].lchild;
		}

		//CDR
		else if (token_index == GetHashValue("cdr", AsciiSum("cdr"), HashTable)) {
			return MemoryArray[EVAL(MemoryArray[MemoryArray[root].rchild].lchild)].rchild;
		}

		//DEFINE
		else if (token_index == GetHashValue("define", AsciiSum("define"), HashTable)) {
			int temp = 0;
			if (MemoryArray[MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild].lchild == GetHashValue("lambda", AsciiSum("lambda"), HashTable)) {
				HashTable[-MemoryArray[MemoryArray[root].rchild].lchild].LinkofValue = MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild;
			}
			else {
				HashTable[-MemoryArray[MemoryArray[root].rchild].lchild].LinkofValue = EVAL(MemoryArray[MemoryArray[MemoryArray[root].rchild].rchild].lchild);
			}
			return root;
		}

		//QUOTE
		else if (token_index == GetHashValue("quote", AsciiSum("quote"), HashTable)) {
			return MemoryArray[MemoryArray[root].rchild].lchild;
		}

		//USER_DEFINED_FUNCTION
		else if (MemoryArray[HashTable[-token_index].LinkofValue].lchild == GetHashValue("lambda", AsciiSum("lambda"), HashTable)) {
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
			return (token_index);
		}
	}
	else {
		int check = HashTable[-root].LinkofValue;
		while (check != 0) {
			if (HashTable[-check].LinkofValue == 0) {
				return check;
			}
			else {
				check = HashTable[-check].LinkofValue;
			}
		}
		return root;
	}
}

int main() {
	GetHashValue("#t", AsciiSum("#t"), HashTable);
	GetHashValue("#f", AsciiSum("#f"), HashTable);
	while (1) {
		string input;
		cout << "> ";
		getline(cin, input);
		string ParsedInput = parse(input);
		string token;
		int i = 0;
		string tokenstore[30];
		string newcommand = "";
		stringstream ss(PP(newcommand, ParsedInput));
		while (ss >> token) {
			tokenstore[i] = token;
			i++;
			GetHashValue(token, AsciiSum(token), HashTable);
		}

		int root = Read(tokenstore);


		//cout << "root값 : " << root << endl;
		string final = GetNextToken(tokenstore, true, true);
		/*
		cout << "] ";
		cout << "Free-list의 root node index : " << Alloc() << endl;
		cout << "Memory array의 root node index : " << root << endl << endl;
		cout << "<HashTable>" << endl;
		cout << "  HashValue  l  Symbol  | LoV" << endl;
		cout << "---------------------------" << endl;
		for (int i = 0; i < 1000; i++) {
			if (!HashTable[i].symbol.empty()) {
				cout << "    ";
				cout.width(4);
				cout << -i;
				cout << "     l    " << HashTable[i].symbol<<"     l    "<<HashTable[i].LinkofValue << endl;
			}
		}
		cout << endl << "<MemoryArray>" << endl;
		cout << "   NodeID  l   Lchild   l   Rchild   ㅣ  FreeList" << endl;
		cout << "----------------------------------------------------" << endl;

		for (int i = 0; i < 30; i++) {
			cout << "    ";
			cout.width(2);
			cout << i;
			cout << "     l  ";
			cout.width(5);
			cout << MemoryArray[i].lchild;
			cout << "     l     ";
			cout.width(2);
			cout << MemoryArray[i].rchild;

			if (MemoryArray[i].isFreeList == true) {
				if (i == 0) {
					cout << "     l  (null node)";
				}
				else if (i == Alloc()) {
					cout << "     l       O       <--(root)";
				}
				else {
					cout << "     l       O";
				}
			}
			else {
				cout << "     l       X";
			}

			cout << endl;
		}
		cout << endl;
		*/
		int result = EVAL(root);
		Print(result, true);
		cout << endl;
		/*
		for (int i = 1; i < 30; i++) {
			MemoryArray[i].isFreeList = true;
		}
		*/
		cin.clear();
	}
}
