#include "MlpNetwork.h"
#include "Dense.h"

/**
     * class constructor
     * @param weights weight matrices for each layer
     * @param biases bias vectors for each layer
     */
MlpNetwork::MlpNetwork(const Matrix *weights, const Matrix *biases)
{
	for (int i = 0; i < MLP_SIZE; i++)
	{
		//matrix assignment operator
		_weights[i] = weights[i];
		_biases[i] = biases[i];
	}
}

/**
     * activate MlpNetwork on an input matrix
     * @param input matrix representing a 28x28 grayscale pixel image
     * @return Digit struct containing recognized digit and probability
     */
Digit MlpNetwork::operator()(Matrix &input)
{
	//create array of layers
	Dense layer[] =
			{
					Dense(_weights[0], _biases[0], Relu),
					Dense(_weights[1], _biases[1], Relu),
					Dense(_weights[2], _biases[2], Relu),
					Dense(_weights[3], _biases[3], Softmax)
			};
	//concatenate layer activations
	Matrix final = layer[3](layer[2](layer[1](layer[0](input))));
	unsigned int value = 0;
	float probability = 0.0f;
	//find maximum probability value of output matrix
	for (int i = 0; i < final.getRows(); i++)
	{
		if (final[i] > probability)
		{
			//update recognized digit and probability
			value = i;
			probability = final[i];
		}
	}
	return Digit{value, probability};
}

