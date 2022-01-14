//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Digit.h"
#include "Dense.h"

#define MLP_SIZE 4

const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784},
								  {64,  128},
								  {20,  64},
								  {10,  20}};
const MatrixDims biasDims[] = {{128, 1},
							   {64,  1},
							   {20,  1},
							   {10,  1}};

/**
 * class represents an MlpNetwork used to identify a digit from a 28x28 matrix of Grayscale pixel data
 */
class MlpNetwork
{
private:
	Matrix _weights[MLP_SIZE];
	Matrix _biases[MLP_SIZE];

public:
	/**
	 * class constructor
	 * @param weights weight matrices for each layer
	 * @param biases bias vectors for each layer
	 */
	MlpNetwork(const Matrix weights[MLP_SIZE], const Matrix biases[MLP_SIZE]);

	/**
	 * parenthesis operator for class: activate MlpNetwork on an input matrix
	 * @param input matrix representing a 28x28 grayscale pixel image
	 * @return Digit struct containing recognized digit and probability
	 */
	Digit operator()(Matrix &input);
};

#endif // MLPNETWORK_H
