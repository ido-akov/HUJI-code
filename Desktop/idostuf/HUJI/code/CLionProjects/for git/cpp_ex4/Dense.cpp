
#include "Dense.h"

/**
     * activate layer on input matrix.
     * @param inp matrix
     * @return output matrix with result of activation of layer
     */
Matrix Dense::operator()(const Matrix &inp)
{
	return _act((_weights * inp) + _bias);
}
