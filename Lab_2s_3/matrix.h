#pragma once

namespace mtrx 
{
	struct Cell
	{
		int row, col, value;
		Cell(int row, int col, int value);
	};



	struct MtrxList
	{
		int row_am = 0, col_am =0, len = 0;

		struct Node
		{
			Cell* data;
			Node* next = nullptr;
			Node* prev = nullptr;
			Node(Cell* data);
		};
		Node* head = nullptr;
		Node* tail = nullptr;


		void add(int row, int col, int value);
		void del();
		void removeDup();
		void print();
		void printFull();

	};

	MtrxList readLst(char* const NAME);
	MtrxList* arrIdxTrns(MtrxList subject);
	MtrxList summ(MtrxList first_matrix, MtrxList second_matrix);
	MtrxList multiply(MtrxList first_matrix, MtrxList second_matrix);

	
}
