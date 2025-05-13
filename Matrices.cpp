#include "Matrices.h"
namespace Matrices
{
    ///Construct a matrix of the specified size.
    ///Initialize each element to 0.
    Matrix::Matrix(int _rows, int _cols)
    {
        rows = _rows;
        cols = _cols;
        a.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            a.at(i).resize(cols);
            for (int j = 0; j < cols; j++)
            {
                a.at(i).at(j) = 0;
            }
        }
    }




    ///Add each corresponding element.
    ///usage:  c = a + b;
    Matrix operator+(const Matrix& a, const Matrix& b)
    {
        // Throw error if matrices are of different size
        if ((a.getRows() != b.getRows()) || (a.getCols() != b.getCols()))
        {
            throw runtime_error("Error: dimensions must agree");
        }

        // Initialize variables and new matrix to return
        Matrix c(a.getRows(), a.getCols());

        // Iterate through every element of input matrices, set output matrix accordingly
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                c(i, j) = a(i, j) + b(i, j);
            }
        }

        // Return output matrix
        return c;
    }




    ///Matrix multiply.  See description.
    ///usage:  c = a * b;
    Matrix operator*(const Matrix& a, const Matrix& b)
    {
        // Ensure a columns are equal to b rows
        if (a.getCols() != b.getRows())
        {
            throw runtime_error("Error: dimensions must agree");
        }

        // Initialize output matrix
        Matrix c(a.getRows(), b.getCols());

        // Column for b
        for (int k = 0; k < b.getCols(); k++)
        {
            // Row for a
            for (int i = 0; i < a.getRows(); i++)
            {
                // Initialize running sum
                double runningSum = 0.0;
                // Column for a, row for b
                for (int j = 0; j < a.getCols(); j++)
                {
                    // Store multiplied result in running sum
                    runningSum += a(i, j) * b(j, k);
                }
                // Store running sum in output matrix
                c(i, k) = runningSum;
            }
        }
        // Return output matrix
        return c;
    }




    ///Matrix comparison.  See description.
    ///usage:  a == b
    bool operator==(const Matrix& a, const Matrix& b)
    {
        // Not equal if different sizes
        if ((a.getRows() != b.getRows()) || (a.getCols() != b.getCols()))
        {
            return false;
        }
        // Check every element; matrices not equal if any are different
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                if (abs(a(i, j) - b(i, j)) > 0.001)
                {
                    return false;
                }
            }
        }
        // Matrices must be equal by now
        return true;
    }




    ///Matrix comparison.  See description.
    ///usage:  a != b
    bool operator!=(const Matrix& a, const Matrix& b)
    {
        // Inverse == operator
        return !(a == b);
    }




    ///Output matrix.
    ///Separate columns by ' ' and rows by '\n'
    ostream& operator<<(ostream& os, const Matrix& a)
    {
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                os << " " << setw(10) << a(i, j);
            }
            os << endl;
        }
        return os;
    }

    // NOTE: Base-class Matrix constructor has placeholder values; look into what they should call instead?
    RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2)
    {
        return;
    }

    // NOTE: Base-class Matrix constructor has placeholder values; look into what they should call instead?
    ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2)
    {
        return;
    }

    // NOTE: Base-class Matrix constructor has placeholder values; look into what they should call instead?
    TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols)
    {
        return;
    }

}