//TODO: Первичное создание файла, проверка на корректность infix, вычисление infix, проверочная работа, посчитать скорость, задание по варианту
//TODO? Сделать массивами, сравнить скорость, infix -> PN, infix -> RPN

// Пользовательское соглашение.
//Выбирая "принять", вы обязуетесь соблюдать следующие условия использования программы:
//1 все бинарные операторы, скобки и операнды разделять пробелами (пример: ( 2 + 2 ) * 2 )
//2 вводить унарный минус слитно (пример: 2 * ( -1 ); 2 -1 * )

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <conio.h>

using namespace std;

#define VAL  0													// В стек занесено новое значение
#define ADD  1													// Сложение
#define SUB  2													// Вычитание
#define MUL  3													// Умножение
#define DIV  4													// Деление
#define BRO 5													// Открывающая скобка
#define BRC 6													// Закрывающая скобка
#define SUF -1													// В стеке недостаточно операндов
#define UNK -2													// Неопознанное значение

#define inpFile "input.txt"										// Файл ввода

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

template<typename T, typename N>
void stackPush(Stack<T>* stack, N data);					// Добавить элемент в стак
template<typename T>
T stackPop(Stack<T>* stack);								// Удалить элемент из стака
template<typename T>
float stackCheck(Stack<T>* stack);								// Получить элемент из стака
float calcPN(string ex, bool isRev);
template<typename T>
int parse(Stack<T>* stack, string s);

int main()
{
	setlocale(LC_ALL, "russian");
	agreement();
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

template<typename T>
int stackCheck(Stack<T>* stack)
{
	int token = parse(stack, to_string(stack->head->data));
	return token;
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

void PNToInfix(string& ex, bool isRev)
{
	if (!isRev) reverse(ex.begin(), ex.end());
	Stack<string> stack;
	string token, buffer, temp1, temp2;
	stringstream bufStream, stackStream;
	bufStream << ex;
	cin.clear();
	while (getline(bufStream, token, ' '))
	{
		/* Пытаемся распознать текущий аргумент как число или
		 * символ арифметической операции */
		switch (parse(&stack, token)) {
		case VAL: 
			stackPush(&stack, token);
			break;

			/* Вычисляем */
		case ADD:
			temp1 = stackPop(&stack);
			temp2 = stackPop(&stack);
			buffer = "( + )";
			buffer.insert(1, temp1);
			buffer.insert(buffer.length()-1, temp2);
			stackPush(&stack, buffer);
			break;

		case SUB:
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
			break;

		case MUL:
			temp1 = stackPop(&stack);
			temp2 = stackPop(&stack);
			buffer = " * ";
			buffer.insert(0, temp1);
			buffer.insert(buffer.length(), temp2);
			stackPush(&stack, buffer);
			break;

		case DIV:
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
			break;


			/* Обработка ошибок */
		case SUF:
			cerr << "Недостаточно операндов!" << endl;
			return;

		case UNK:
			cerr << "Неопознанный аргумент!" << endl;
			return;
		}
	}
	ex = stack.head->data;
}

void infixToPN(string& ex, bool isRev)
{
	//if (!isRev) reverse(ex.begin(), ex.end());
	Stack<string> stack;
	string token, buffer, temp1, temp2;
	stringstream bufStream;
	bufStream << ex;
	while (getline(bufStream, token, ' '))
	{
		// Попытка опознать токен
		switch (parse(&stack, token)) {
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
				cerr << "Недостаточно операндов!" << endl;
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
				cerr << "Недостаточно операндов!" << endl;
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
				cerr << "Недостаточно операндов!" << endl;
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
				cerr << "Недостаточно операндов!" << endl;
				return;
			}
			break;

		case UNK:
			cerr << "Неопознанный аргумент!" << endl;
			return;
		}
	}
	ex = stack.head->data;
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
		//infixToPN(ex);
		break;
	case 1:
		//infixToRPN(ex);
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

	float result;

	int answer = getAnsNot();
	switch (answer)
	{
	case 0:
		//result = calcInfix(ex);
		break;
	case 1:
		result = calcPN(ex, false);
		break;
	case 2:
		result = calcPN(ex, true);
		break;
	case 3:
		return;
		break;
	}
	cout << ex << " = " << result << endl;
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
			//genTest();
			break;
		case 3:
			//task();
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

float calcPN(string ex, bool isRev)
{
	if (!isRev) reverse(ex.begin(), ex.end());
	Stack<float> stack;
	string token;
	stringstream bufStream;
	bufStream << ex;
	cin.clear();
	float temp = 0;
	while (getline(bufStream, token, ' '))
	{
		/* Пытаемся распознать текущий аргумент как число или
		 * символ арифметической операции */
		switch (parse(&stack, token)) {
		case VAL: 
			stackPush(&stack, stof(token));
			break;

			/* Вычисляем */
		case ADD:
			if (stack.head != nullptr && stack.head->next != nullptr) {
				stackPush(&stack, stackPop(&stack) + stackPop(&stack));
			}
			else
			{
				cerr << "Недостаточно операндов!" << endl;
				return(1);
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
				cerr << "Недостаточно операндов!" << endl;
				return(1);
			}
			break;

		case MUL:
			if (stack.head != nullptr && stack.head->next != nullptr) {
				stackPush(&stack, stackPop(&stack) * stackPop(&stack));
			}
			else
			{
				cerr << "Недостаточно операндов!" << endl;
				return(1);
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
					cerr << "Деление на ноль запрещено!" << endl;
					return(NULL);
				}
			}
			else
			{
				cerr << "Недостаточно операндов!" << endl;
				return(1);
			}
			break;

		case UNK:
			cerr << "Неопознанный аргумент!" << endl;
			return(1);
		}
	}
	return stack.head->data;
}

template<typename T>
int parse(Stack<T>* stack, string s)
{
	T tval;																// Временное значение того же типа, что и стак
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