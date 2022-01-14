//
// Created by Tamar Akov on 6/11/20.
//

#ifndef EX4_DENSE_H
#define EX4_DENSE_H

#include "Matrix.h"
#include "Activation.h"

/**
 * This class describes a layer in a network.
 */
class Dense
{
private:
	Matrix _weights, _bias;
	Activation _act;

public:
	/**
	 * class constructor
	 * @param w weights matrix
	 * @param b bias matrix(vector)
	 * @param a activationType function
	 */
	Dense(const Matrix &w, const Matrix &b, ActivationType a) : _weights(w), _bias(b), _act(a)
	{}

	/**
	 * getter method
	 * @return weights matrix
	 */
	const Matrix &getWeights() const
	{ return _weights; }

	/**
	 * getter method
	 * @return bias vector
	 */
	const Matrix &getBias() const
	{ return _bias; }

	/**
	 * getter method
	 * @return Activation member with activationType function
	 */
	const Activation &getActivation() const
	{ return _act; }

	/**
	 * parenthesis operator: activate layer on input matrix
	 * @param inp matrix
	 * @return output matrix with result of activation of layer
	 */
	Matrix operator()(const Matrix &inp);
};


#endif //EX4_DENSE_H
