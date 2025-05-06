#include "Matrices.h"

Matrix::Matrix(int _rows, int _cols)
{
	return;
}

///Add each corresponding element.
   ///usage:  c = a + b;
Matrix operator+(const Matrix& a, const Matrix& b)
{
	return a;
}

///Matrix multiply.  See description.
///usage:  c = a * b;
Matrix operator*(const Matrix& a, const Matrix& b)
{
	return a;
}

///Matrix comparison.  See description.
///usage:  a == b
bool operator==(const Matrix& a, const Matrix& b)
{
	return false;
}

///Matrix comparison.  See description.
///usage:  a != b
bool operator!=(const Matrix& a, const Matrix& b)
{
	return false;
}

///Output matrix.
///Separate columns by ' ' and rows by '\n'
ostream& operator<<(ostream& os, const Matrix& a)
{
	return os;
}

RotationMatrix::RotationMatrix(double theta)
{
	return;
}

ScalingMatrix::ScalingMatrix(double scale)
{
	return;
}

TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols)
{
	return;
}