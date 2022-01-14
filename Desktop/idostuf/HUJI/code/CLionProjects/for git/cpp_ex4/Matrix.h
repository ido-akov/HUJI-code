// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H
//magic numbers, values for error messages
#define DEFAULT_DIM 1
#define MATRIX_DIMS_ERROR "Error: matrix dimensions are incorrect."
#define MATRIX_MULT_ERROR "Error: matrices dimensions are incompatible for multiplication."
#define MATRIX_ADD_ERROR "Error: matrices dimensions are incompatible for addition."
#define FILE_PARSING_ERROR "Error: file parsing unsuccessful"
#define SPACE ' ';
#define DOUBLE_SPACE "  ";
#define DOUBLE_ASTERISK "**";
#define THRESHOLD 0.1f
#define OUT_OF_BOUNDS_ERROR "Error: tried to access value outside matrix bounds"

#include <iostream>
#include <fstream>

//namespaces
using std::istream;
using std::ifstream;
using std::ostream;

/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
	int rows, cols;
} MatrixDims;

/**
//class for Matrix object, holds MatrixDims struct and pointer to array with float data
*/
class Matrix
{
private:
	MatrixDims _dims;
	float *_data;

	/**
	 * error printing function, exits program
	 * @param msg to be printed
	 */
	static void _errorMsg(const char *msg)
	{
		std::cerr << msg << std::endl;
		exit(EXIT_FAILURE);
	}

	/**
	 * helper function- copies all data into another matrix
	 * @param m matrix from which data is to be copied
	 */
	void _matrixDeepCopy(const Matrix &m);

public:
	/**
	 * c'tor- constructs Matrix rows × cols, inits all elements to 0
	 */
	Matrix(int rows, int cols);

	/**
	 * default c'tor- constructs 1×1 Matrix, inits the single element to 0,
	 * uses constructor delegation
	 */
	Matrix() : Matrix(DEFAULT_DIM, DEFAULT_DIM)
	{}

	/**
	 * Copy constructor- constructs matrix from another Matrix m.
	 * Uses constructor delegation, followed by deep copy of matrix data
	 * @param m matrix from which to create a copy
	 */
	Matrix(Matrix const &m) : Matrix(m.getRows(), m.getCols())
	{
		_matrixDeepCopy(m);
	}

	/**
	 * class destructor- deletes matrix data
	 */
	~Matrix()
	{ delete[] _data; };

	/**
	 * getter method
	 * @return number of cols in this matrix object
	 */
	const int &getCols() const
	{ return this->_dims.cols; }

	/**
	 * getter method
	 * @return number of rows in this matrix object
	 */
	const int &getRows() const
	{ return this->_dims.rows; }

	/**
	 * function makes a matrix of dimensions (m,n) into a vector of dimensions (m*n,1)
	 * @return vector version of matrix
	 */
	Matrix &vectorize();

	/**
	 *  Prints matrix elements, no return value. Prints space after each element (incl. last
	 *  element in the row). Prints newline after each row (incl. last row)
	 */
	void plainPrint() const;

	/**
	 * assignment operator for matrices
	 * @param rhs matrix from which to assign dimensions and values
	 * @return this matrix with updated dims and values
	 */
	Matrix &operator=(const Matrix &rhs);

	/**
	 * multiplication operator for matrices- must be of form M(m,n) * N(n,k)
	 * @param rhs matrix to multiply by
	 * @return new matrix with multiplication values
	 */
	Matrix operator*(const Matrix &rhs) const;

	/**
	 * Scalar multiplication by right operator for matrices
	 * @param rhs scalar by which to multiply all values in matrix
	 * @return this matrix
	 */
	Matrix &operator*(const float &rhs);

	/**
	 * Scalar multiplication by left operator for matrices
	 * @param rhs scalar by which to multiply all values in matrix
	 * @return matrix
	 */
	friend Matrix &operator*(float lhs, Matrix &rhs);

	/**
	 * Matrix addition operator. Both matrices must be of dims (m,n)
	 * @param rhs matrix to add to this matrix
	 * @return matrix with updated values
	 */
	Matrix operator+(const Matrix &rhs) const;

	/**
	 * accumalation addition for matrices. Both matrices must be of dims (m,n)
	 * @param rhs matrix to add to this matrix
	 * @return this matrix
	 */
	Matrix &operator+=(const Matrix &rhs);

	/**
	 * parenthesis operator for matrices. Returns value in (i,j)th cell
	 * @param row of value
	 * @param col of value
	 * @return value
	 */
	float &operator()(const int row, const int col);

	/**
	 * parenthesis operator for constant matrices. Returns value in (i,j)th cell
	 * @param row of value
	 * @param col of value
	 * @return value
	 */
	const float &operator()(const int row, const int col) const;

	/**
	 * square bracket operator for matrices. Returns value in (i* colDim + j)th cell
	 * @param index of value
	 * @return value
	 */
	float &operator[](int index);

	/**
	 * square bracket operator for constant matrices. Returns value in (i* colDim + j)th cell
	 * @param index of value
	 * @return value
	 */
	const float &operator[](int index) const;

	/**
	 * Input stream Fills matrix elements, has to read input stream fully, otherwise,
	 * that’s an error.
	 * @param s input stream to read from
	 * @param m matrix to read data into
	 * @return input stream for concatenation
	 */
	friend istream &operator>>(istream &is, Matrix &m);

	/**
	 * Output matrix in "grayscale" pixel form for user to output stream
	 * @param s output stream
	 * @param m matrix to be outputted
	 * @return output stream for concatenation
	 */
	friend ostream &operator<<(ostream &os, const Matrix &m);
};

#endif //MATRIX_H

