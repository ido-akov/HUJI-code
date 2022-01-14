#include "Activation.h"
#include <cmath>

using std::exp;

/**
     * helper function for Relu function activation on matrix
     * @param m matrix to activate function on
     * @return matrix with updated values
     */
Matrix Activation::_activateRelu(const Matrix &m)
{
	Matrix res = Matrix(m);
	for (int i = 0; i < res.getRows() * res.getCols(); i++)
	{
		//return max between x and 0
		res[i] = fmaxf(res[i], 0.f);
	}
	return res;
}

/**
 * helper function for Softmax function activation on matrix
 * @param m matrix to activate function on
 * @return matrix with updated values
 */
Matrix Activation::_activateSoftmax(const Matrix &m)
{
	Matrix res = Matrix(m);
	float sum = 0.0f;
	for (int i = 0; i < res.getRows() * res.getCols(); i++)
	{
		res[i] = exp(m[i]);
		sum += res[i];
	}
	float normalize = 1 / sum;
	//normalize matrix values by multiplication by a scalar
	return normalize * res;
}

/**
	 * activate this instance's activationType function on matrix input.
	 * @param m matrix
	 * @return matrix with updated vals
	 */
Matrix Activation::operator()(const Matrix &m)
{
	switch (_type)
	{
		case Relu:
			return _activateRelu(m);
		default:
			return _activateSoftmax(m);
	}
}
