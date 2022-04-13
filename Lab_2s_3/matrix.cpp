#include "matrix.h"

#include <fstream>
#include <string>
#include <cstring>
#include <iostream>

void append(char* s, char c) 
{
	int len = strlen(s);
	s[len] = c;
	s[len + 1] = '\0';
}

mtrx::Cell::Cell(int row, int col, int value)
{
	this->row = row;
	this->col = col;
	this->value = value;
}

mtrx::MtrxList::Node::Node(Cell* data)
{
	this->data = data;
}

void mtrx::MtrxList::add(int row, int col, int value)
{
	Cell* to_add = new Cell(row, col, value);
	Node* node = new Node(to_add);
	if (head == nullptr)
	{
		this->len = 1;
		head = node;
		tail = node;
	}
	else
	{
		tail->next = node;
		node->prev = tail;
		tail = node;
		tail->next = nullptr;
		this->len++;
	}
}

void mtrx::MtrxList::del()
{
	if (head == nullptr) return;
	else
	{
		Node* temp = tail;

		tail->prev->next = nullptr;
		tail = tail->prev;

		this->len--;

		delete(temp);
		temp = nullptr;
	}
}

void mtrx::MtrxList::removeDup()
{
	Node* iter1 = head;
	Node* iter2 = head;
	Node* temp;

	if (iter1->next == nullptr) return;

	while (iter1 != nullptr)
	{
		iter2 = iter1->next;
		while (iter2 != nullptr)
		{
			//main LOGIC
			if (iter2->data->row == iter1->data->row && 
				iter2->data->col == iter1->data->col)
			{
				iter1->data->value = iter1->data->value + iter2->data->value;
				temp = iter2;
		
				(iter2->prev)->next = iter2->next;
				if(iter2->next != nullptr) iter2->next->prev = iter2->prev;
				iter2 = iter2->next;

				this->len--;

				delete(temp);
				temp = NULL;
			}
			else iter2 = iter2->next;
		}
		if (iter1->next != nullptr) iter1 = iter1->next;
		else return;
	}
}

void mtrx::MtrxList::print()
{
	if (head == nullptr)
	{
		std::cout << "\nEmpty matrix\n\n";
		return;
	}

	Node* start = head;

	std::cout << "\nRow positions: ";
	while (start)
	{
		std::cout << start->data->row << " ";
		start = start->next;
	}


	start = head;
	std::cout << "\nCol positions: ";
	while (start)
	{
		std::cout << start->data->col << " ";
		start = start->next;
	}

	start = head;
	std::cout << "\nValues: ";
	while (start)
	{
		std::cout << start->data->value << " ";
		start = start->next;
	}
	std::cout << "\n\n";
}

void mtrx::MtrxList::printFull()
{
	Node* iter = head;
	
	int row = 1;
	int col = 1;

	for (row; row <= this->row_am; row++)
	{
		for (col; col <= this->col_am; col++)
		{
			if (iter != nullptr && iter->data->row == row && iter->data->col == col)
			{
				std::cout << iter->data->value << " ";
				iter = iter->next;
			}
			else std::cout << "0 ";
		}
		col = 1;
		std::cout << "\n";
	}
}

mtrx::MtrxList mtrx::readLst(char* const NAME)
{
	std::ifstream out(NAME, std::ifstream::in);
	mtrx::MtrxList result;

	result.len = 0;
	result.row_am = 0;
	result.col_am = 0;

	const int SIZE = 8;
	char buffer[SIZE];

	bool reading_num = false;
	bool reading_col = true;

	char number[8] = "";
	int temp_row = 1;
	int temp_col = 1;

	int buff_val;


	while (!out.eof())
	{
		memset(buffer, 0, sizeof(buffer));
		out.read((char*)&buffer, sizeof(buffer));
		for (int i = 0; i < SIZE; i++)
		{
			if (buffer[i] == '\n')
			{
				result.row_am++;
				
				if (reading_num)
				{
					if (reading_col)
					{
						result.col_am++;
						reading_col = false;
					}
					buff_val = std::stoi(number);
					result.add(temp_row, temp_col, buff_val);
					memset(number, 0, sizeof(number));

					reading_num = false;
					reading_col = false;

					temp_row++;
					temp_col = 1;		

					continue;
				}

				else
				{
					reading_col = false;
					temp_row++;
					temp_col = 1;

					continue;
				}

			}

			else if (buffer[i] == ' ')
			{
				if (reading_num)
				{
					buff_val = std::stoi(number);
					result.add(temp_row, temp_col, buff_val);
					memset(number, 0, sizeof(number));
					reading_num = false;
					temp_col++;

					if (reading_col) result.col_am++;
				}
				else continue;
			}

			else if (buffer[i] == '0' && !reading_num)
			{
				temp_col++;
				if (reading_col) result.col_am++;
				continue;
			}

			else if (buffer[i] == '-' || isdigit(buffer[i]))
			{
				reading_num = true;
				append(number, buffer[i]);
			}

			else
			{
				if (reading_num)
				{
					buff_val = std::stoi(number);
					result.add(temp_row, temp_col, buff_val);
					memset(number, 0, sizeof(number));
					reading_num = false;
					if (reading_col) result.col_am++;
				}
				
				result.row_am++;
				return result;
			}
		}
	}

	return result;
}

mtrx::MtrxList* mtrx::arrIdxTrns(MtrxList subject)
{
	const static int MAX = subject.row_am;

	mtrx::MtrxList* data = new mtrx::MtrxList[MAX];
	// Array representation
	// of sparse matrix
	//[,0] represents row
	//[,1] represents col
	//[,2] represents value
	for (int i = 0; i < MAX; i++)
	{
		mtrx::MtrxList to_add;
		to_add.row_am = i + 1;
		to_add.col_am = subject.col_am;
		data[i] = to_add;
	}

	mtrx::MtrxList::Node* start = subject.head;

	while (start)
	{
		data[start->data->row].add(start->data->row, start->data->col,
			start->data->value);

		start = start->next;
	}

	return data;
}

mtrx::MtrxList mtrx::summ(MtrxList first_matrix, MtrxList second_matrix)
{ 
	mtrx::MtrxList result;

	if (first_matrix.row_am != second_matrix.row_am || 
		first_matrix.col_am != second_matrix.col_am)
	{
		std::cout << "\nThe matrixs with different amount of rows and columns cannot be added to one another\n";
		return result;
	}

	result.row_am = first_matrix.row_am;
	result.col_am = second_matrix.col_am;

	mtrx::MtrxList::Node* iter1 = first_matrix.head;
	mtrx::MtrxList::Node* iter2 = second_matrix.head;

	while (iter1 != nullptr && iter2 != nullptr)
	{	 
		if (iter1->data->row == iter2->data->row)
		{
			if (iter1->data->col == iter2->data->col) 
			{
				result.add(iter1->data->row, iter1->data->col, iter1->data->value + iter2->data->value);
				iter1 = iter1->next;
				iter2 = iter2->next;
			}

			else if (iter1->data->col < iter2->data->col)
			{
				result.add(iter1->data->row, iter1->data->col, iter1->data->value);
				iter1 = iter1->next;
			}

			else
			{
				result.add(iter2->data->row, iter2->data->col, iter2->data->value);
				iter2 = iter2->next;
			}
		}

		else if (iter1->data->row < iter2->data->row)
		{
			result.add(iter1->data->row, iter1->data->col, iter1->data->value);
			iter1 = iter1->next;
		}

		else
		{
			result.add(iter2->data->row, iter2->data->col, iter2->data->value);
			iter2 = iter2->next;
		}
	}

	while (iter1 != nullptr)
	{
		result.add(iter1->data->row, iter1->data->col, iter1->data->value);
		iter1 = iter1->next;
	}

	while (iter2 != nullptr)
	{
		result.add(iter2->data->row, iter2->data->col, iter2->data->value);
		iter2 = iter2->next;
	}
	
	//result.removeDup();

	return result;
}

mtrx::MtrxList mtrx::multiply(MtrxList first_matrix, MtrxList second_matrix)
{
	mtrx::MtrxList result;

	if (first_matrix.col_am != second_matrix.row_am)
	{
		"\nThe incompatible matrixes cannot be added to one multiplied\n";
		return result;
	}

	result.row_am = first_matrix.row_am;
	result.col_am = second_matrix.col_am;

	mtrx::MtrxList::Node* first_start = first_matrix.head;

	while (first_start)
	{
		int first_row = first_start->data->row;
		int first_col = first_start->data->col;
		int first_val = first_start->data->value;

		mtrx::MtrxList::Node* second_start = second_matrix.head;

		while (second_start)
		{
			int second_row = second_start->data->row;
			int second_col = second_start->data->col;
			int second_val = second_start->data->value;

			if (first_col == second_row)
			{
				result.add(first_row, second_col, first_val * second_val);
			}

			second_start = second_start->next;
		}

		first_start = first_start->next;
	}

	result.removeDup();

	return result;
}