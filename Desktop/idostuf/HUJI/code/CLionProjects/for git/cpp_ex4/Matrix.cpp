#include "Matrix.h"

/**
     * c'tor- constructs Matrix rows × cols, inits all elements to 0
     */
Matrix::Matrix(int rows, int cols)
{
	if (rows <= 0 || cols <= 0)
	{
		//return error, exit
		Matrix::_errorMsg(MATRIX_DIMS_ERROR);
	}
	_dims.rows = rows;
	_dims.cols = cols;
	//allocate array on heap
	_data = new float[rows * cols]();
}

/**
     * helper function- copies all data into another matrix
     * @param m matrix from which data is to be copied
     */
void Matrix::_matrixDeepCopy(const Matrix &m)
{
	for (int i = 0; i < _dims.cols * _dims.rows; i++)
	{
		(*this)[i] = m[i];
	}
}

/**
     * function makes a matrix of dimensions (m,n) into a vector of dimensions (m*n,1)
     * @return vector version of matrix
     */
Matrix &Matrix::vectorize()
{
	_dims.rows *= _dims.cols;
	_dims.cols = DEFAULT_DIM;
	return *this;
}

/**
     *  Prints matrix elements, no return value. Prints space after each element (incl. last
     *  element in the row). Prints newline after each row (incl. last row)
     */
void Matrix::plainPrint() const
{
	for (int i = 0; i < _dims.rows; i++)
	{
		for (int j = 0; j < _dims.cols; j++)
		{
			std::cout << (*this)(i, j) << SPACE;
		}
		std::cout << std::endl;
	}
}

/**
     * assignment operator for matrices
     * @param rhs matrix from which to assign dimensions and values
     * @return this matrix with updated dims and values
     */
Matrix &Matrix::operator=(const Matrix &rhs)
{
	if (this == &rhs)
	{
		//self assignment
		return *this;
	}
	//_data always has a value because of default constructor
	delete[] _data;
	//default assignment
	_dims = rhs._dims;
	_data = new float[_dims.rows * _dims.cols];
	//copy values to this matrix
	_matrixDeepCopy(rhs);
	return *this;
}

/**
     * multiplication operator for matrices- must be of form M(m,n) * N(n,k)
     * @param rhs matrix to multiply by
     * @return new matrix with multiplication values
     */
Matrix Matrix::operator*(const Matrix &rhs) const
{
	if (_dims.cols != rhs.getRows())
	{
		//matrices can't be multiplied
		Matrix::_errorMsg(MATRIX_MULT_ERROR);
	}
	//create new empty matrix with correct dimensions to store within values
	Matrix res = Matrix(_dims.rows, rhs.getCols());
	for (int i = 0; i < res.getRows(); i++)
	{
		for (int j = 0; j < res.getCols(); j++)
		{
			float sum = 0.0f;
			for (int k = 0; k < _dims.cols; k++)
			{
				//matrix multiplication formula
				sum += (*this)(i, k) * rhs(k, j);
			}
			res(i, j) = sum;
		}
	}
	return res;
}

/**
     * Scalar multiplication by right operator for matrices
     * @param rhs scalar by which to multiply all values in matrix
     * @return this matrix
     */
Matrix &Matrix::operator*(const float &rhs)
{
	for (int i = 0; i < _dims.cols * _dims.rows; i++)
	{
		(*this)[i] *= rhs;
	}
	return *this;
}

/**
     * Scalar multiplication by left operator for matrices
     * @param rhs scalar by which to multiply all values in matrix
     * @return matrix
     */
Matrix &operator*(const float lhs, Matrix &rhs)
{
	for (int i = 0; i < rhs.getRows() * rhs.getCols(); i++)
	{
		rhs[i] *= lhs;
	}
	return rhs;
}

/**
     * Matrix addition operator. Both matrices must be of dims (m,n)
     * @param rhs matrix to add to this matrix
     * @return matrix with updated values
     */
Matrix Matrix::operator+(const Matrix &rhs) const
{
	if (_dims.rows != rhs.getRows() || _dims.cols != rhs.getCols())
	{
		//matrices incompatible for addition
		Matrix::_errorMsg(MATRIX_ADD_ERROR);
	}
	//create new empty matrix to store within values
	Matrix res = Matrix(_dims.rows, rhs.getCols());
	for (int i = 0; i < res.getRows(); i++)
	{
		for (int j = 0; j < res.getCols(); j++)
		{
			res(i, j) = (*this)(i, j) + rhs(i, j);
		}
	}
	return res;
}

/**
     * accumalation addition for matrices. Both matrices must be of dims (m,n)
     * @param rhs matrix to add to this matrix
     * @return this matrix
     */
Matrix &Matrix::operator+=(const Matrix &rhs)
{
	if (_dims.rows != rhs.getRows() || _dims.cols != rhs.getCols())
	{
		Matrix::_errorMsg(MATRIX_ADD_ERROR);
	}
	for (int i = 0; i < _dims.rows; i++)
	{
		for (int j = 0; j < _dims.cols; j++)
		{
			(*this)(i, j) += rhs(i, j);
		}
	}
	return *this;
}

/**
     * parenthesis operator for matrices. Returns value in (i,j)th cell
     * @param row of value
     * @param col of value
     * @return value
     */
float &Matrix::operator()(const int row, const int col)
{
	if (row < 0 || row >= _dims.rows || col < 0 || col >= _dims.cols)
	{
		_errorMsg(OUT_OF_BOUNDS_ERROR);
	}
	return (*this)[row * _dims.cols + col];
}

/**
     * parenthesis operator for constant matrices. Returns value in (i,j)th cell
     * @param row of value
     * @param col of value
     * @return value
     */
const float &Matrix::operator()(const int row, const int col) const
{
	if (row < 0 || row >= _dims.rows || col < 0 || col >= _dims.cols)
	{
		_errorMsg(OUT_OF_BOUNDS_ERROR);
	}
	return (*this)[row * _dims.cols + col];
}


/**
     * square bracket operator for matrices. Returns value in (i* colDim + j)th cell
     * @param index of value
     * @return value
     */
float &Matrix::operator[](int index)
{
	if (index < 0 || index >= _dims.rows * _dims.cols)
	{
		_errorMsg(OUT_OF_BOUNDS_ERROR);
	}
	return _data[index];
}

/**
     * square bracket operator for constant matrices. Returns value in (i* colDim + j)th cell
     * @param index of value
     * @return value
     */
const float &Matrix::operator[](int index) const
{
	if (index < 0 || index >= _dims.rows * _dims.cols)
	{
		_errorMsg(OUT_OF_BOUNDS_ERROR);
	}
	return _data[index];
}

/**
     * Input stream Fills matrix elements, has to read input stream fully, otherwise,
     * that’s an error.
     * @param s input stream to read from
     * @param m matrix to read data into
     * @return input stream for concatenation
     */
istream &operator>>(istream &is, Matrix &m)
{
	//cast _data float to char* to read all bytes
	is.read((char *) m._data, m.getCols() * m.getRows() * sizeof(float));
	if (!is)
	{
		//encountered error while reading stream
		Matrix::_errorMsg(FILE_PARSING_ERROR);
	}
	return is;
}

/**
     * Output matrix in "grayscale" pixel form for user to output stream
     * @param s output stream
     * @param m matrix to be outputted
     * @return output stream for concatenation
     */
ostream &operator<<(ostream &os, const Matrix &m)
{
	for (int i = 0; i < m.getRows(); i++)
	{
		for (int j = 0; j < m.getCols(); j++)
		{
			if (m(i, j) <= THRESHOLD)
			{
				os << DOUBLE_SPACE;
			}
			else
			{
				os << DOUBLE_ASTERISK;
			}
		}
		os << std::endl;
	}
	return os;
}






