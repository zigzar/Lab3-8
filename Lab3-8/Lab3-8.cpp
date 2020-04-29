//TODO: Меню, ввод с клавиатуры, ввод с файла, парсинг строки, проверка на корректность, вычисление, проверочная работа, посчитать скорость, задание по варианту
//TODO? Сделать массивами, сравнить скорость, infix -> PN, infix -> RPN, PN -> infix, PN -> RPN, RPN -> infix, RPN -> PN

// Пользовательское соглашение.
//Выбирая "принять", вы обязуетесь соблюдать следующие условия использования программы:
//1 все операторы и операнды разделять пробелами
//2 вводить только положительные числа
#include <iostream>
#include <string>

using namespace std;

struct Node														// Узел стака
{
	float data;													// Данные
	Node* next;													// Адрес следующего узла
};

struct Stack													// Стак
{
	Node* head = nullptr;										// Первый элемент стака
};

void stackPush(Stack* stack, float data);						// Добавить элемент в стак
float stackPop(Stack* stack);									// Удалить элемент из стака

int main()
{
	string ex;
	cin >> ex;
	
}

void stackPush(Stack* stack, float data)
{
	Node* node = new Node;
	node->data = data;
	node->next = stack->head;
	stack->head = node;
}

float stackPop(Stack* stack)
{
	float data = stack->head->data;
	Node* temp = stack->head;

	stack->head = stack->head->next;

	delete temp;
	return data;
}
