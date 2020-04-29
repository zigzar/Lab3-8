//TODO: Меню, ввод с клавиатуры, ввод с файла, парсинг строки, проверка на корректность, вычисление, проверочная работа, посчитать скорость, задание по варианту
//TODO? Сделать массивами, сравнить скорость, infix -> PN, infix -> RPN, PN -> infix, PN -> RPN, RPN -> infix, RPN -> PN

// Пользовательское соглашение.
//Выбирая "принять", вы обязуетесь соблюдать следующие условия использования программы:
//1 все бинарные операторы, скобки и операнды разделять пробелами (пример: ( 2 + 2 ) * 2 )
//2 вводить унарный минус слитно (пример: 2 * ( -1 ); 2 -1 * )

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#define VAL  0													// В стек занесено новое значение
#define ADD  1													// Сложение
#define SUB  2													// Вычитание
#define MUL  3													// Умножение
#define DIV  4													// Деление
#define SUF -1													// В стеке недостаточно операндов
#define UNK -2													// Неопознанное значение

template<typename T>
struct Node														// Узел стака
{
	T data;													// Данные
	Node* next;													// Адрес следующего узла
};

template<typename T>
struct Stack													// Стак
{
	Node<T>* head = nullptr;										// Первый элемент стака
};

template<typename T>
void stackPush(Stack<T>* stack, float data);						// Добавить элемент в стак
template<typename T>
float stackPop(Stack<T>* stack);									// Удалить элемент из стака
float calcPN(string ex, bool isRev);
template<typename T>
int parse(Stack<T>* stack, string s);

int main()
{
	setlocale(LC_ALL, "russian");
	string ex;
	cout << "Введите выражение и нажмите Enter:" << endl;
	getline(cin, ex);
	cout << calcPN(ex, false) << endl;
	system("pause");
}
template<typename T>
void stackPush(Stack<T>* stack, float data)
{
	Node<T>* node = new Node<T>;
	node->data = data;
	node->next = stack->head;
	stack->head = node;
}
template<typename T>
float stackPop(Stack<T>* stack)
{
	float data = stack->head->data;
	Node<T>* temp = stack->head;

	stack->head = stack->head->next;

	delete temp;
	return data;
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
		case VAL: continue;

			/* Вычисляем */
		case ADD:
			stackPush(&stack, stackPop(&stack) + stackPop(&stack));
			break;

		case SUB:
			if (isRev)
			{
				temp = stackPop(&stack);
				stackPush(&stack, stackPop(&stack) - temp);
			}
			else
			{
				stackPush(&stack, stackPop(&stack) - stackPop(&stack));
			}
			break;

		case MUL:
			stackPush(&stack, stackPop(&stack) * stackPop(&stack));
			break;

		case DIV:
			temp = stackPop(&stack);
			if (temp != 0) {
				stackPush(&stack, stackPop(&stack) / temp);
				break;
			}
			else {
				cerr << "Деление на ноль!" << endl;
				return(1);
			}

			/* Обработка ошибок */
		case SUF:
			cerr << "Недостаточно операндов!" << endl;
			return(1);

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
	float tval = 0;

	if (s[s.length() - 1] == '-') {										// Распознавание знаков арифметических операций
		if (stack->head != nullptr && stack->head->next != nullptr) {
			return(SUB);
		}
		else return(SUF);
	}
	if (s[s.length() - 1] == '+') {
		if (stack->head != nullptr && stack->head->next != nullptr) {
			return(ADD);
		}
		else return(SUF);
	}
	if (s[s.length() - 1] == '*') {
		if (stack->head != nullptr && stack->head->next != nullptr) {
			return(MUL);
		}
		else return(SUF);
	}
	if (s[s.length() - 1] == '/') {
		if (stack->head != nullptr && stack->head->next != nullptr) {
			return(DIV);
		}
		else return(SUF);
	}

	// Попытка сконвертировать строковый аргумент в число
	try
	{
		tval = stof(s);
	}
	catch (const std::exception&)
	{
		return(UNK);
	}

	stackPush(stack, tval);											// Сохранить число в стак

	return(VAL);
}