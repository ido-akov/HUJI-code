#include "Matrix.h"

#ifndef ACTIVATION_H
#define ACTIVATION_H

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
	Relu,
	Softmax
};

/**
 * class for activating function on matrix
 */
class Activation
{
private:
	ActivationType _type;

	/**
	 * helper function for Softmax function activation on matrix
	 * @param m matrix to activate function on
	 * @return matrix with updated values
	 */
	static Matrix _activateSoftmax(const Matrix &m);

	/**
	 * helper function for Relu function activation on matrix
	 * @param m matrix to activate function on
	 * @return matrix with updated values
	 */
	static Matrix _activateRelu(const Matrix &m);

public:
/**
 * constructor for class
 * @param type ActivationType function
 */
	explicit Activation(ActivationType type) : _type(type)
	{}

	/**
	 * Getter method
	 * @return activation type
	 */
	ActivationType &getActivationType()
	{ return _type; }

	/**
	 * parenthesis operator for class: activate this instance's activationType function on
	 * matrix input
	 * @param m matrix
	 * @return matrix with updated vals
	 */
	Matrix operator()(const Matrix &m);
};

#endif //ACTIVATION_H
