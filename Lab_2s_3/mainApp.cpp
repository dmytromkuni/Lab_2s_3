#include "matrix.h"

int main()
{
	mtrx::MtrxList matrix1, matrix2, matrix3, matrix4;
	char NAME1[] = "matrix1.txt";
	char NAME2[] = "matrix2.txt";

	matrix1 = mtrx::readLst(NAME1);
	matrix2 = mtrx::readLst(NAME2);

	matrix3 = mtrx::multiply(matrix1, matrix2);
	matrix4 = mtrx::summ(matrix1, matrix2);

	matrix3.print();
	matrix3.printFull();

	//matrix4.print();
	//matrix4.printFull();
}