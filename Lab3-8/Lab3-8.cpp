// Пользовательское соглашение.
//Выбирая "принять", вы обязуетесь соблюдать следующие условия использования программы:
//1 все бинарные операторы, скобки и операнды разделять пробелами (пример: ( 2 + 2 ) * 2 )
//2 вводить унарный минус слитно (пример: 2 * ( -1 ); 2 -1 * )

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <conio.h>
#include <ctime>
#include <chrono>
#include <vector>
#include <Windows.h>

using namespace std;

#define VAL  0													// В стек занесено новое значение
#define ADD  11													// Сложение (первая цифра отвечает за приоритет, а вторая - за номер операции)
#define SUB  12													// Вычитание (первая цифра отвечает за приоритет, а вторая - за номер операции)
#define MUL  23													// Умножение (первая цифра отвечает за приоритет, а вторая - за номер операции)
#define DIV  24													// Деление (первая цифра отвечает за приоритет, а вторая - за номер операции)
#define BRO 5													// Открывающая скобка
#define BRC 6													// Закрывающая скобка
#define SUF -1													// В стеке недостаточно операндов
#define UNK -2													// Неопознанное значение

#define inpFile "input.txt"										// Файл ввода
#define testFile "test.txt"										// Файл теста
#define answerFile "answer.txt"									// Файл ответов

template<typename T>
struct Node														// Узел стака
{
	T data;														// Данные
	Node* next;													// Адрес следующего узла
};

template<typename T>
struct Stack													// Стак
{
	Node<T>* head = nullptr;									// Первый элемент стака
};

double listTime;
double arrayTime;
chrono::time_point<chrono::high_resolution_clock> timerStart, timerEnd;

bool RPNMenu(string& ex);
bool PNMenu(string& ex);
bool infixMenu(string& ex);
void transMenu();
bool inpMenu(string& ex);
void calcMenu();
void menu();
void agreement();

int getAnsInp();
int getAnsRPN();
int getAnsPN();
int getAnsInf();
int getAnsNot();
int getAnsMenu();
int getAnsAgreement();

void newFile();
void console(string& ex);
void file(string& ex);

template<typename T, typename N>
void stackPush(Stack<T>* stack, N data);					// Добавить элемент в стак
template<typename T>
T stackPop(Stack<T>* stack);								// Удалить элемент из стака
template<typename T>
void stackShow(Stack<T>* stack, bool showMessage);

float calcInfix(string ex);
float calcPN(string ex, bool isRev);
float calcTest(string ex, bool isRev, bool showMessage);
void infixToPN(string& ex, bool isRev);
void PNToInfix(string& ex, bool isRev);
int parse(string s);

float calcInfixVec(string ex);
float calcPNVec(string ex, bool isRev);
void infixToPNVec(string& ex, bool isRev);
void PNToInfixVec(string& ex, bool isRev);

void task();
void genPN(bool isRev);
void genTest();

int main()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(h, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(h, &structCursorInfo);
	setlocale(LC_ALL, "russian");
	srand(time(NULL));
	cout.setf(ios::fixed);
	agreement();
	newFile();
	menu();
	system("pause");
}

template<typename T, typename N>
void stackPush(Stack<T>* stack, N data)
{
	Node<T>* node = new Node<T>;
	node->data = data;
	node->next = stack->head;
	stack->head = node;
}

template<typename T>
T stackPop(Stack<T>* stack)
{
	T data = stack->head->data;
	Node<T>* temp = stack->head;

	stack->head = stack->head->next;

	delete temp;
	return data;
}


void newFile()
{
	ifstream fin;
	fin.open(inpFile);
	if (!fin.is_open())
	{
		cout << "Файл ввода не существует. Будет создан новый файл.";
		fin.close();
		ofstream fout;
		fout.open(inpFile);
		fout.close();
		return;
	}
	fin.close();
}

void console(string& ex)
{
	cout << "Введите выражение и нажмите Enter:" << endl;
	getline(cin, ex);
}

void file(string& ex)
{
	ifstream fin;
	fin.open(inpFile);
	if (!fin.is_open())
	{
		cout << "Файл ввода не существует. Будет создан новый файл.";
		fin.close();
		ofstream fout;
		fout.open(inpFile);
		fout.close();
		return;
	}
	getline(fin, ex);
	fin.close();
}

void PNToInfixVec(string& ex, bool isRev)
{
	if (!isRev) reverse(ex.begin(), ex.end());
	vector<string> vector;
	string token, buffer, temp1, temp2;
	stringstream bufStream;
	bufStream << ex;
	cin.clear();
	while (getline(bufStream, token, ' '))
	{
		/* Пытаемся распознать текущий аргумент как число или
		 * символ арифметической операции */
		switch (parse(token)) {
		case VAL:
			vector.push_back(token);
			break;

			// Вычисление
		case ADD:
			if (vector.size() >= 2) { // Если операндов >= 2
				temp1 =  vector.back();
				vector.pop_back();
				temp2 = vector.back();
				vector.pop_back();
				buffer = "( + )";
				buffer.insert(1, temp1);
				buffer.insert(buffer.length() - 1, temp2);
				vector.push_back(buffer);
			}
			else
			{
				cerr << "Недостаточно операндов!" << endl;system("pause");
				return;
			}
			break;

		case SUB:
			if (vector.size() >= 2) { // Если операндов >= 2
				if (isRev)
				{
					temp1 = vector.back();
					vector.pop_back();
					temp2 = vector.back();
					vector.pop_back();
					buffer = "( - )";
					buffer.insert(1, temp2);
					buffer.insert(buffer.length() - 1, temp1);
					vector.push_back(buffer);
				}
				else
				{
					temp1 = vector.back();
					vector.pop_back();
					temp2 = vector.back();
					vector.pop_back();
					buffer = "( - )";
					buffer.insert(1, temp1);
					buffer.insert(buffer.length() - 1, temp2);
					vector.push_back(buffer);
				}
			}
			else
			{
				cerr << "Недостаточно операндов!" << endl;system("pause");
				return;
			}
			break;

		case MUL:
			if (vector.size() >= 2) { // Если операндов >= 2
				temp1 = vector.back();
				vector.pop_back();
				temp2 = vector.back();
				vector.pop_back();
				buffer = " * ";
				buffer.insert(0, temp1);
				buffer.insert(buffer.length(), temp2);
				vector.push_back(buffer);
			}
			else
			{
				cerr << "Недостаточно операндов!" << endl;system("pause");
				return;
			}
			break;

		case DIV:
			if (vector.size() >= 2) { // Если операндов >= 2
				if (isRev)
				{
					temp1 = vector.back();
					vector.pop_back();
					temp2 = vector.back();
					vector.pop_back();
					buffer = " / ";
					buffer.insert(1, temp2);
					buffer.insert(buffer.length() - 1, temp1);
					vector.push_back(buffer);
				}
				else
				{
					temp1 = vector.back();
					vector.pop_back();
					temp2 = vector.back();
					vector.pop_back();
					buffer = " / ";
					buffer.insert(1, temp1);
					buffer.insert(buffer.length() - 1, temp2);
					vector.push_back(buffer);
				}
			}
			else
			{
				cerr << "Недостаточно операндов!" << endl;system("pause");
				return;
			}
			break;

		case UNK:
			cerr << "Неопознанный аргумент!" << endl;system("pause");
			return;
		}
	}
	ex = vector.back();
}

void infixToPNVec(string& ex, bool isRev)
{
	vector<string> stack;
	string op, token;
	stringstream bufStream;
	bufStream << ex;
	ex = "";
	while (getline(bufStream, token, ' '))
	{
		// Попытка опознать токен
		int tokenType = parse(token);
		if (tokenType == VAL) {
			ex += token;
			ex += " ";
		}
		else
		{
			if (tokenType == BRO) stack.push_back(token);
			else if (tokenType == BRC)
			{
				op = stack.back();
				stack.pop_back();
				while (parse(op) != BRO)
				{
					ex += op;
					ex += " ";
					op = stack.back();
					stack.pop_back();
				}
			}
			else
			{
				while (stack.size() != 0 && int(parse(stack.back()) / 10) > tokenType / 10)
				{
					ex += stack.back();
					stack.pop_back();
					ex += " ";
				}
				stack.push_back(token);
			}
		}
	}
	while (stack.size() != 0)
	{
		ex += stack.back();
		stack.pop_back();
		ex += " ";
	}
	if (!isRev) reverse(ex.begin(), ex.end());
}

void PNToInfix(string& ex, bool isRev)
{
	if (!isRev) reverse(ex.begin(), ex.end());
	Stack<string> stack;
	string token, buffer, temp1, temp2;
	stringstream bufStream;
	bufStream << ex;
	cin.clear();
	while (getline(bufStream, token, ' '))
	{
		/* Пытаемся распознать текущий аргумент как число или
		 * символ арифметической операции */
		switch (parse(token)) {
		case VAL: 
			stackPush(&stack, token);
			break;

			// Вычисление
		case ADD:
			if (stack.head != nullptr && stack.head->next != nullptr) { // Если операндов >= 2
				temp1 = stackPop(&stack);
				temp2 = stackPop(&stack);
				buffer = "( + )";
				buffer.insert(1, temp1);
				buffer.insert(buffer.length() - 1, temp2);
				stackPush(&stack, buffer);
			}
			else
			{
				cerr << "Недостаточно операндов!" << endl;system("pause");
				return;
			}
			break;

		case SUB:
			if (stack.head != nullptr && stack.head->next != nullptr) { // Если операндов >= 2
				if (isRev)
				{
					temp1 = stackPop(&stack);
					temp2 = stackPop(&stack);
					buffer = "( - )";
					buffer.insert(1, temp2);
					buffer.insert(buffer.length() - 1, temp1);
					stackPush(&stack, buffer);
				}
				else
				{
					temp1 = stackPop(&stack);
					temp2 = stackPop(&stack);
					buffer = "( - )";
					buffer.insert(1, temp1);
					buffer.insert(buffer.length() - 1, temp2);
					stackPush(&stack, buffer);
				}
			}
			else
			{
				cerr << "Недостаточно операндов!" << endl;system("pause");
				return;
			}
			break;

		case MUL:
			if (stack.head != nullptr && stack.head->next != nullptr) { // Если операндов >= 2
				temp1 = stackPop(&stack);
				temp2 = stackPop(&stack);
				buffer = " * ";
				buffer.insert(0, temp1);
				buffer.insert(buffer.length(), temp2);
				stackPush(&stack, buffer);
			}
			else
			{
				cerr << "Недостаточно операндов!" << endl;system("pause");
				return;
			}
			break;

		case DIV:
			if (stack.head != nullptr && stack.head->next != nullptr) { // Если операндов >= 2
				if (isRev)
				{
					temp1 = stackPop(&stack);
					temp2 = stackPop(&stack);
					buffer = " / ";
					buffer.insert(0, temp2);
					buffer.insert(buffer.length(), temp1);
					stackPush(&stack, buffer);
				}
				else
				{
					temp1 = stackPop(&stack);
					temp2 = stackPop(&stack);
					buffer = " / ";
					buffer.insert(0, temp1);
					buffer.insert(buffer.length(), temp2);
					stackPush(&stack, buffer);
				}
			}
			else
			{
				cerr << "Недостаточно операндов!" << endl;system("pause");
				return;
			}
			break;

		case UNK:
			cerr << "Неопознанный аргумент!" << endl;system("pause");
			return;
		}
	}
	ex = stack.head->data;
}

void infixToPN(string& ex, bool isRev)
{
	Stack<string> stack;
	string op, token;
	stringstream bufStream;
	bufStream << ex;
	ex = "";
	while (getline(bufStream, token, ' '))
	{
		// Попытка опознать токен
		int tokenType = parse(token);
		if (tokenType == VAL) {
			ex += token;
			ex += " ";
		}
		else
		{
			if (tokenType == BRO) stackPush(&stack, token);
			else if (tokenType == BRC)
			{
				op = stackPop(&stack);
				while (parse(op) != BRO)
				{
					ex += op;
					ex += " ";
					op = stackPop(&stack);
				}
			}
			else
			{
				while (stack.head != nullptr && int(parse(stack.head->data) / 10) > tokenType / 10)
				{
					ex += stackPop(&stack);
					ex += " ";
				}
				stackPush(&stack, token);
			}
		}
	}
	while (stack.head != nullptr)
	{
		ex += stackPop(&stack);
		ex += " ";
	}
	if (!isRev) reverse(ex.begin(), ex.end());
}

bool RPNMenu(string& ex)
{
	int answer = getAnsRPN();
	switch (answer)
	{
	case 0:
		PNToInfix(ex, true);
		break;
	case 1:
		reverse(ex.begin(), ex.end());
		break;
	case 2:
		return true;
		break;
	}
	return false;
}

bool PNMenu(string& ex)
{
	int answer = getAnsPN();
	switch (answer)
	{
	case 0:
		PNToInfix(ex, false);
		break;
	case 1:
		reverse(ex.begin(), ex.end());
		break;
	case 2:
		return true;
		break;
	}
	return false;
}

bool infixMenu(string& ex)
{
	int answer = getAnsInf();
	switch (answer)
	{
	case 0:
		infixToPN(ex, false);
		break;
	case 1:
		infixToPN(ex, true);
		break;
	case 2:
		return true;
		break;
	}
	return false;
}

void transMenu()
{
	string ex;
	if (inpMenu(ex)) return;		// Если выбрано "Назад", выйти из меню перевода

	int answer;
	while (true)
	{
		answer = getAnsNot();
		switch (answer)
		{
		case 0:
			if (infixMenu(ex)) continue;
			break;
		case 1:
			if (PNMenu(ex)) continue;
			break;
		case 2:
			if (RPNMenu(ex)) continue;
			break;
		case 3:
			return;
			break;
		}
		break;
	}
		cout << "Преобразованное выражение:" << endl
			<< ex << endl;
		system("pause");
}

bool inpMenu(string& ex)
{
	int answer = getAnsInp();
	switch (answer)
	{
	case 0:
		console(ex);
		break;
	case 1:
		file(ex);
		break;
	case 2:
		return true;				// Выбрано назад
		break;
	}
	return false;					// Выбран ввод выражения
}

void calcMenu()
{
	string ex;

	if (inpMenu(ex)) return;		// Если выбрано "Назад", выйти из меню вычисления

	float result = 0;

	int answer = getAnsNot();
	switch (answer)
	{
	case 0:
		timerStart = chrono::high_resolution_clock::now();
		result = calcInfix(ex);
		timerEnd = chrono::high_resolution_clock::now();
		listTime = chrono::duration_cast<chrono::nanoseconds>(timerEnd - timerStart).count();
		timerStart = chrono::high_resolution_clock::now();
		calcInfixVec(ex);
		timerEnd = chrono::high_resolution_clock::now();
		arrayTime = chrono::duration_cast<chrono::nanoseconds>(timerEnd - timerStart).count();
		break;
	case 1:
		timerStart = chrono::high_resolution_clock::now();
		result = calcPN(ex, false);
		timerEnd = chrono::high_resolution_clock::now();
		listTime = chrono::duration_cast<chrono::nanoseconds>(timerEnd - timerStart).count();
		timerStart = chrono::high_resolution_clock::now();
		calcPNVec(ex, false);
		timerEnd = chrono::high_resolution_clock::now();
		arrayTime = chrono::duration_cast<chrono::nanoseconds>(timerEnd - timerStart).count();
		break;
	case 2:
		timerStart = chrono::high_resolution_clock::now();
		result = calcPN(ex, true);
		timerEnd = chrono::high_resolution_clock::now();
		listTime = chrono::duration_cast<chrono::nanoseconds>(timerEnd - timerStart).count();
		timerStart = chrono::high_resolution_clock::now();
		calcPNVec(ex, true);
		timerEnd = chrono::high_resolution_clock::now();
		arrayTime = chrono::duration_cast<chrono::nanoseconds>(timerEnd - timerStart).count();
		break;
	case 3:
		return;
		break;
	}
	cout.unsetf(ios::fixed);
	cout << ex << " = " << result << endl;
	cout.setf(ios::fixed);
	cout << "Время вычисления списком: " << listTime / 1000000000 << " c" << endl;
	cout << "Время вычисления массивом: " << arrayTime / 1000000000 << " c" << endl;
	system("pause");
}

void menu()
{
	int answer;
	while (true)
	{
		answer = getAnsMenu();
		switch (answer)
		{
		case 0:
			calcMenu();
			break; 
		case 1:
			transMenu();
			break;		
		case 2:
			genTest();
			break;
		case 3:
			task();
			break;
		case 4:
			exit(0);
			break;
		}
	}

}

void agreement()
{
	int answer;
	answer = getAnsAgreement();
	switch (answer)
	{
	case 0:
		return;
		break;
	case 1:
		exit(0);
		break;
	}
}

int getAnsTest()
{
	int choice = 0;
	int options = 5;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;

		cout << "Выберите уровень сложности теста" << endl << endl;

		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;

		if (choice == 0) cout << "-> Легкий" << endl;
		else  cout << "   Легкий" << endl;

		if (choice == 1) cout << "-> Средний" << endl;
		else  cout << "   Средний" << endl;

		if (choice == 2) cout << "-> Тяжелый" << endl;
		else  cout << "   Тяжелый" << endl;

		if (choice == 3) cout << "-> ЛЭТИшник" << endl;
		else  cout << "   ЛЭТИшник" << endl;

		if (choice == 4) cout << "-> Назад" << endl;
		else  cout << "   Назад" << endl;

		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();
			if (ch == 80) choice++;
			if (ch == 72) choice--;
		}
		if (ch == 119) choice--;
		if (ch == 115) choice++;
		if (ch == 13) break;
	}
	system("cls");
	return choice;
}

int getAnsRPN()
{
	int choice = 0;
	int options = 3;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;

		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;

		cout << "Выберите нотацию конечного выражения:" << endl << endl;

		if (choice == 0) cout << "-> Инфиксная (нормальная)" << endl;
		else  cout << "   Инфиксная (нормальная)" << endl;

		if (choice == 1) cout << "-> Прямая польская" << endl;
		else  cout << "   Прямая польская" << endl;

		if (choice == 2) cout << "-> Назад" << endl;
		else  cout << "   Назад" << endl;

		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();
			if (ch == 80) choice++;
			if (ch == 72) choice--;
		}
		if (ch == 119) choice--;
		if (ch == 115) choice++;
		if (ch == 13) break;
	}
	system("cls");
	return choice;
}

int getAnsPN()
{
	int choice = 0;
	int options = 3;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;

		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;

		cout << "Выберите нотацию конечного выражения:" << endl << endl;

		if (choice == 0) cout << "-> Инфиксная (нормальная)" << endl;
		else  cout << "   Инфиксная (нормальная)" << endl;

		if (choice == 1) cout << "-> Обратная польская" << endl;
		else  cout << "   Обратная польская" << endl;

		if (choice == 2) cout << "-> Назад" << endl;
		else  cout << "   Назад" << endl;

		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();
			if (ch == 80) choice++;
			if (ch == 72) choice--;
		}
		if (ch == 119) choice--;
		if (ch == 115) choice++;
		if (ch == 13) break;
	}
	system("cls");
	return choice;
}

int getAnsInf()
{
	int choice = 0;
	int options = 3;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;

		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;

		cout << "Выберите нотацию конечного выражения:" << endl << endl;

		if (choice == 0) cout << "-> Прямая польская" << endl;
		else  cout << "   Прямая польская" << endl;

		if (choice == 1) cout << "-> Обратная польская" << endl;
		else  cout << "   Обратная польская" << endl;

		if (choice == 2) cout << "-> Назад" << endl;
		else  cout << "   Назад" << endl;

		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();
			if (ch == 80) choice++;
			if (ch == 72) choice--;
		}
		if (ch == 119) choice--;
		if (ch == 115) choice++;
		if (ch == 13) break;
	}
	system("cls");
	return choice;
}

int getAnsNot()
{
	int choice = 0;
	int options = 4;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;

		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;

		cout << "Выберите нотацию выражения:" << endl << endl;

		if (choice == 0) cout << "-> Инфиксная (нормальная)" << endl;
		else  cout << "   Инфиксная (нормальная)" << endl;

		if (choice == 1) cout << "-> Прямая польская" << endl;
		else  cout << "   Прямая польская" << endl;

		if (choice == 2) cout << "-> Обратная польская" << endl;
		else  cout << "   Обратная польская" << endl;

		if (choice == 3) cout << "-> Назад" << endl;
		else  cout << "   Назад" << endl;

		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();
			if (ch == 80) choice++;
			if (ch == 72) choice--;
		}
		if (ch == 119) choice--;
		if (ch == 115) choice++;
		if (ch == 13) break;
	}
	system("cls");
	return choice;
}

int getAnsInp()
{
	int choice = 0;
	int options = 3;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;
		
		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;

		cout << "Выберите способ ввода выражения:" << endl << endl;

		if (choice == 0) cout << "-> Ввести в консоль" << endl;
		else  cout << "   Ввести в консоль" << endl;

		if (choice == 1) cout << "-> Прочитать из файла" << endl;
		else  cout << "   Прочитать из файла" << endl;

		if (choice == 2) cout << "-> Назад" << endl;
		else  cout << "   Назад" << endl;

		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();
			if (ch == 80) choice++;
			if (ch == 72) choice--;
		}
		if (ch == 119) choice--;
		if (ch == 115) choice++;
		if (ch == 13) break;
	}
	system("cls");
	return choice;
}

int getAnsMenu()
{
	int choice = 0;
	int options = 5;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;

		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;

		if (choice == 0) cout << "-> Вычислить выражение" << endl;
		else  cout << "   Вычислить выражение" << endl;

		if (choice == 1) cout << "-> Преобразовать выражение" << endl;
		else  cout << "   Преобразовать выражение" << endl;

		if (choice == 2) cout << "-> Сгенерировать тест" << endl;
		else  cout << "   Сгенерировать тест" << endl;

		if (choice == 3) cout << "-> Задание по варианту" << endl;
		else  cout << "   Задание по варианту" << endl;

		if (choice == 4) cout << "-> Выход" << endl;
		else  cout << "   Выход" << endl;

		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();
			if (ch == 80) choice++;
			if (ch == 72) choice--;
		}
		if (ch == 119) choice--;
		if (ch == 115) choice++;
		if (ch == 13) break;
	}
	system("cls");
	return choice;
}

int getAnsAgreement()
{
	int choice = 0;
	int options = 2;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;

		cout << "Пользовательское соглашение." << endl
			<< "Выбирая «принять», вы обязуетесь соблюдать следующие условия использования программы:" << endl
			<< "1 все бинарные операторы, скобки и операнды разделять пробелами (пример: ( 2 + 2 ) * 2 )" << endl
			<< "2 вводить унарный минус слитно (пример: 2 * ( -1 ); 2 -1 * )" << endl << endl;

		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;

		if (choice == 0) cout << "-> Принять" << endl;
		else  cout << "   Принять" << endl;

		if (choice == 1) cout << "-> Отклонить" << endl;
		else  cout << "   Отклонить" << endl;

		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();
			if (ch == 80) choice++;
			if (ch == 72) choice--;
		}
		if (ch == 119) choice--;
		if (ch == 115) choice++;
		if (ch == 13) break;
	}
	system("cls");
	return choice;
}

float calcInfixVec(string ex)
{
	infixToPNVec(ex, true);
	float result = calcPNVec(ex, true);
	return result;
}

float calcInfix(string ex)
{
	infixToPN(ex, true);
	float result = calcPN(ex, true);
	return result;
}

float calcPNVec(string ex, bool isRev)
{
	if (!isRev) reverse(ex.begin(), ex.end());
	vector<float> vector;
	string token;
	stringstream bufStream;
	bufStream << ex;
	float temp = 0;
	while (getline(bufStream, token, ' '))
	{
		switch (parse(token)) { // Попытка распознать токен
		case VAL:
			vector.push_back(stof(token));
			break;

			// Вычисление
		case ADD:
			if (vector.size() >= 2) {
				temp = vector[vector.size() - 1] + vector[vector.size() - 2];
				vector.pop_back();
				vector.pop_back();
				vector.push_back(temp);
			}
			else
			{
				return(1);
			}
			break;

		case SUB:
			if (vector.size() >= 2) {
				if (isRev)
				{
					temp = vector[vector.size() - 2] - vector[vector.size() - 1];
				}
				else
				{
					temp = vector[vector.size() - 1] - vector[vector.size() - 2];
				}
				vector.pop_back();
				vector.pop_back();
				vector.push_back(temp);
			}
			else
			{
				return(1);
			}
			break;

		case MUL:
			if (vector.size() >= 2) {
				temp = vector[vector.size() - 1] + vector[vector.size() - 2];
				vector.pop_back();
				vector.pop_back();
				vector.push_back(temp);
			}
			else
			{
				return(1);
			}
			break;

		case DIV:
			if (vector.size() >= 2) {
				try
				{
					if (isRev)
					{
						temp = vector[vector.size() - 2] / vector[vector.size() - 1];
					}
					else
					{
						temp = vector[vector.size() - 1] / vector[vector.size() - 2];
					}
					vector.pop_back();
					vector.pop_back();
					vector.push_back(temp);
				}
				catch (const std::exception&)
				{
					return(NULL);
				}
			}
			else
			{
				return(1);
			}
			break;

		case UNK:
			return(1);
		}
	}
	return vector.back();
}

float calcPN(string ex, bool isRev)
{
	try
	{
		cout.unsetf(ios::fixed);
		if (!isRev) reverse(ex.begin(), ex.end());
		Stack<float> stack;
		string token;
		stringstream bufStream;
		bufStream << ex;
		float temp = 0;
		while (getline(bufStream, token, ' '))
		{
			switch (parse(token)) { // Попытка распознать токен
			case VAL:
				stackPush(&stack, stof(token));
				break;

				// Вычисление
			case ADD:
				if (stack.head != nullptr && stack.head->next != nullptr) {
					stackPush(&stack, stackPop(&stack) + stackPop(&stack));
				}
				else
				{
					throw "Недостаточно операндов!";
				}
				break;

			case SUB:
				if (stack.head != nullptr && stack.head->next != nullptr) {
					if (isRev)
					{
						temp = stackPop(&stack);
						stackPush(&stack, stackPop(&stack) - temp);
					}
					else
					{
						stackPush(&stack, stackPop(&stack) - stackPop(&stack));
					}
				}
				else
				{
					throw "Недостаточно операндов!";
				}
				break;

			case MUL:
				if (stack.head != nullptr && stack.head->next != nullptr) {
					stackPush(&stack, stackPop(&stack) * stackPop(&stack));
				}
				else
				{
					throw "Недостаточно операндов!";
				}
				break;

			case DIV:
				if (stack.head != nullptr && stack.head->next != nullptr) {
					try
					{
						if (isRev)
						{
							temp = stackPop(&stack);
							stackPush(&stack, stackPop(&stack) / temp);
						}
						else
						{
							stackPush(&stack, stackPop(&stack) / stackPop(&stack));
						}
					}
					catch (const std::exception&)
					{
						throw "Деление на ноль запрещено!";
					}
				}
				else
				{
					throw "Недостаточно операндов!";
				}
				break;

			case UNK:
				throw "Неопознанный аргумент!";
			}
		}
		cout.setf(ios::fixed);
		return stack.head->data;
	}
	catch (const char* exception)
	{
		cerr << exception << endl;
		system("pause");
		return 0;
	}
	catch (const std::exception&)
	{
		cerr << "Неверное выражение!" << endl;
		system("pause");
		return 0;
	}
}

float calcTest(string ex, bool isRev, bool showMessage)
{
	ofstream fout;
	fout.open(answerFile, ios::app);
	try
	{
		if (!isRev)
		{
			reverse(ex.begin(), ex.end());
			if (showMessage) fout << "Переворачиваем выражение: " << ex << endl;
		}
		Stack<float> stack;
		string token;
		stringstream bufStream;
		bufStream << ex;
		float temp1 = 0;
		float temp2 = 0;
		while (getline(bufStream, token, ' '))
		{
			switch (parse(token)) { // Попытка распознать токен
			case VAL:
				stackPush(&stack, stof(token));
				if (showMessage)
				{
					fout << "Добавляем число " << token << " в стак" << endl;
					stackShow(&stack, true);
				}
				break;

				// Вычисление
			case ADD:
				if (stack.head != nullptr && stack.head->next != nullptr) {
					temp1 = stackPop(&stack);
					temp2 = stackPop(&stack);
					if (showMessage) fout << "Берём числа " << temp1 << " и " << temp2 << " из стака и складываем: " << temp2 << " + " << temp1 << " = " << temp2 + temp1 << endl;
					stackPush(&stack, temp1 + temp2);
					if (showMessage) {
						fout << "Добавляем число " << temp2 + temp1 << " в стак" << endl;
						stackShow(&stack, true);
					}
				}
				else
				{
					throw "Недостаточно операндов!";
				}
				break;

			case SUB:
				if (stack.head != nullptr && stack.head->next != nullptr) {
					if (isRev)
					{
						temp1 = stackPop(&stack);
						temp2 = stackPop(&stack);
						if (showMessage) fout << "Берём числа " << temp1 << " и " << temp2 << " из стака и вычитаем: " << temp2 << " - " << temp1 << " = " << temp2 - temp1 << endl;
						stackPush(&stack, temp2 - temp1);
						if (showMessage)
						{ 
							fout << "Добавляем число " << temp2 - temp1 << " в стак" << endl;
							stackShow(&stack, true);
						}
					}
					else
					{
						temp1 = stackPop(&stack);
						temp2 = stackPop(&stack);
						if (showMessage) fout << "Берём числа " << temp1 << " и " << temp2 << " из стака и вычитаем: " << temp1 << " - " << temp2 << " = " << temp1 - temp2 << endl;
						stackPush(&stack, temp1 - temp2);
						if (showMessage)
						{ 
							fout << "Добавляем число " << temp1 - temp2 << " в стак" << endl;
							stackShow(&stack, true);
						}
					}
				}
				else
				{
					throw "Недостаточно операндов!";
				}
				break;

			case MUL:
				if (stack.head != nullptr && stack.head->next != nullptr) {
					temp1 = stackPop(&stack);
					temp2 = stackPop(&stack);
					if (showMessage) fout << "Берём числа " << temp1 << " и " << temp2 << " из стака и перемножаем: " << temp2 << " * " << temp1 << " = " << temp2 * temp1 << endl;
					stackPush(&stack, temp2 * temp1);
					if (showMessage)
					{ 
						fout << "Добавляем число " << temp2 * temp1 << " в стак" << endl;
						stackShow(&stack, true);
					}
				}
				else
				{
					throw "Недостаточно операндов!";
				}
				break;

			case DIV:
				if (stack.head != nullptr && stack.head->next != nullptr) {
					try
					{
						if (isRev)
						{
							temp1 = stackPop(&stack);
							temp2 = stackPop(&stack);
							if (showMessage) fout << "Берём числа " << temp1 << " и " << temp2 << " из стака и делим: " << temp2 << " / " << temp1 << " = " << temp2 / temp1 << endl;
							stackPush(&stack, temp2 / temp1);
							if (showMessage)
							{ 
							fout << "Добавляем число " << temp2 / temp1 << " в стак" << endl;
							stackShow(&stack, true);
							}
						}
						else
						{
							temp1 = stackPop(&stack);
							temp2 = stackPop(&stack);
							if (showMessage) fout << "Берём числа " << temp1 << " и " << temp2 << " из стака и делим: " << temp1 << " / " << temp2 << " = " << temp1 / temp2 << endl;
							stackPush(&stack, temp1 / temp2);
							if (showMessage) 
							{
								fout << "Добавляем число " << temp1 / temp2 << " в стак" << endl;
								stackShow(&stack, true);
							}
						}
					}
					catch (const std::exception&)
					{
						throw "Деление на ноль запрещено!";
					}
				}
				else
				{
					throw "Недостаточно операндов!";
				}
				break;

			case UNK:
				throw "Неопознанный аргумент!";
			}
		}
		fout << "Ответ: " << stack.head->data << endl;
	}
	catch (const char* exception)
	{
		cerr << exception << endl;
		system("pause");
		return 0;
	}
	catch (const std::exception&)
	{
		cerr << "Неверное выражение!" << endl;
		system("pause");
		return 0;
	}
	fout.close();
}

int parse(string s)
{
	float tval;																// Временное значение того же типа, что и стак
																		// Распознавание знаков арифметических операций
	if (s[s.length() - 1] == '-') {										// Если последний символ строки - минус (проверка на бинарный минус)
			return(SUB);
	}
	if (s[s.length() - 1] == '+') {
			return(ADD);
	}
	if (s[s.length() - 1] == '*') {
			return(MUL);
	}
	if (s[s.length() - 1] == '/') {
			return(DIV);
	}
	if (s[s.length() - 1] == '(') {
		return(BRO);
	}
	if (s[s.length() - 1] == ')') {
		return(BRC);
	}

	try																// Попытаться сконвертировать строковый аргумент в число
	{
		tval = stof(s);
	}
	catch (const std::exception&)
	{
		return(UNK);
	}

	return(VAL);
}

template <typename T>
void stackCopy(Stack<T>* stack, Stack<T>* copy)
{
	Node<T>* ptr = stack->head;
	do
	{
		stackPush(copy, ptr->data);
		ptr = ptr->next;
	} while (ptr != nullptr);
}

template<typename T>
void stackShow(Stack<T>* stack, bool showMessage)
{
	ofstream fout;
	fout.open(answerFile, ios::app);
	fout << "Стак: ";
	Node<T>* ptr = stack->head;
	do
	{
		if (showMessage) fout << ptr->data << " ";
		else cout << ptr->data << " ";
		ptr = ptr->next;
	} while (ptr != nullptr);
	if (showMessage) fout << endl;
	else cout << endl;
	fout.close();
}

void task()
{
	cout << "Вариант №4." << endl
		<< "Реализуйте стак. Заполните его случайными положительными и отрицательными числами." << endl
		<< "Преобразуйте стак в два стака. Первый должен содержать только положительные числа, второй – отрицательные." << endl << endl;

	timerStart = chrono::high_resolution_clock::now();
	Stack<float> general, pos, neg, copy;
	for (int i = 0; i < 10; i++)
	{
		stackPush(&general, rand() % 11 - 5);
	}
	timerEnd = chrono::high_resolution_clock::now();
	double listTime1 = chrono::duration_cast<chrono::nanoseconds>(timerEnd - timerStart).count();
	stackCopy(&general, &copy);			// Сделать копию стака для вывода в консоль
	timerStart = chrono::high_resolution_clock::now();
	for (int i = 0; i < 10; i++)
	{
		if (general.head->data < 0) stackPush(&neg, general.head->data);
		if (general.head->data > 0) stackPush(&pos, general.head->data);
		stackPop(&general);
	}
	timerEnd = chrono::high_resolution_clock::now();
	double listTime2 = chrono::duration_cast<chrono::nanoseconds>(timerEnd - timerStart).count();

	timerStart = chrono::high_resolution_clock::now();
	vector<float> generalV, posV, negV;
	for (int i = 0; i < 10; i++)
	{
		generalV.push_back(rand() % 11 - 5);
	}
	for (int i = 0; i < 10; i++)
	{
		if (generalV.back() < 0) negV.push_back(generalV.back());
		if (generalV.back() > 0) posV.push_back(generalV.back());
		generalV.pop_back();
	}
	timerEnd = chrono::high_resolution_clock::now();
	arrayTime = chrono::duration_cast<chrono::nanoseconds>(timerEnd - timerStart).count();

	cout.precision(0);
	cout << "Общий стак:" << endl;
	stackShow(&copy, false);
	cout << "Стак положительных чисел:" << endl;
	if (pos.head != nullptr) stackShow(&pos, false);		// Если стак не пустой, вывести числа
	cout << "Стак отрицательных чисел:" << endl;
	if (neg.head != nullptr) stackShow(&neg, false);		// Если стак не пустой, вывести числа
	cout.precision(7);
	cout << "Время выполнения списком: " << (listTime1 + listTime2) / 1000000000 << " c" << endl;
	cout << "Время выполнения массивом: " << arrayTime / 1000000000 << " c" << endl;

	system("pause");

}

void genPN(bool isRev, int operands, int taskNo) {
	int quantity = 10;
	int operations, outputOperands;
	string operand, op;
	ofstream fout;
	string ex = "";
	operands += rand() % 4;
	outputOperands = operands;
	operations = operands - 1;

	while (operands || operations) {			// Пока есть операнды и операторы
		if (rand() % 2) {
			if (operands) {
				operand = to_string(rand() % 13 + 1);
				ex += operand;
				ex += " ";
				operands--;
			}
		}
		else {
			if (operands < operations) {
				switch (rand() % 3) {
				case 0:
					op = '+';
					break;
				case 1:
					op = '*';
					break;
				case 2:
					op = '-';
					break;
				}
				operations--;
				ex += op;
				ex += " ";
			}
		}
	}

	if (!isRev) reverse(ex.begin(), ex.end());
	fout.open(testFile, ios_base::app);
	fout << taskNo << ") " << ex << endl;
	fout.close();
}

void genTest()
{
	ofstream fout;
	ifstream fin;
	string line;
	int variants, pn, rpn, taskNo, operands = 0;

	cout << "Введите количество вариантов:" << endl;
	cin >> variants;
	cout << "Введите количество задач с ППЗ:" << endl;
	cin >> pn;
	cout << "Введите количество задач с ОПЗ:" << endl;
	cin >> rpn;

	int answer = getAnsTest();
	switch (answer)
	{
	case 0:
		operands = 2;
		break;
	case 1:
		operands = 4;
		break;
	case 2:
		operands = 6;
		break;
	case 3:
		operands = 10;
		break;
	case 4:
		return;
		break;
	}

	fout.open(testFile);
	fout.close();

	for (int i = 1; i <= variants; i++)
	{
		fout.open(testFile, ios_base::app);
		fout << "Вариант " << i << endl;
		fout.close();
		taskNo = 1;
		for (int j = 0; j < pn; j++)
		{
			genPN(false, operands, taskNo);
			taskNo++;
		}
		for (int j = 0; j < rpn; j++)
		{
			genPN(true, operands, taskNo);
			taskNo++;
		}
		fout.open(testFile, ios_base::app);
		fout << endl;
		fout.close();
	}
	system(testFile);

	fout.open(answerFile);
	fout.close();

	fin.open(testFile);
	for (int i = 1; i <= variants; i++)
	{
		getline(fin, line);
		fout.open(answerFile, ios::app);
		fout << "Вариант " << i << endl;
		fout.close();
		for (int j = 0; j < pn; j++)
		{
			getline(fin, line);
			fout.open(answerFile, ios::app);
			fout << line << endl;
			fout.close();
			line = line.substr(3);
			calcTest(line, false, true);
			fout.open(answerFile, ios::app);
			fout << endl;
			fout.close();
		}
		for (int j = 0; j < rpn; j++)
		{
			getline(fin, line);
			fout.open(answerFile, ios::app);
			fout << line << endl;
			fout.close();
			line = line.substr(3);
			calcTest(line, true, true);
			fout.open(answerFile, ios::app);
			fout << endl;
			fout.close();
		}
		getline(fin, line);			// Пропустить отступ между вариантами
		fout.open(answerFile, ios::app);
		fout << endl
			<< "_______________________________________" << endl;
		fout.close();
	}
	fin.close();
	system(answerFile);
}

