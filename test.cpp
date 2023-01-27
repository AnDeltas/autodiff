#include <iostream>
#include "PtrGraph.h"
#include "BMode.h"

int main(int argc, char* argv[]) {
    ad::Tensor& x1 = ad::tensor(2);
    ad::Tensor& x2 = ad::tensor(5);

    ad::Tensor& f = ad::ln(x1) + x1 * x2 - ad::sin(x2);

    f.Backward();

    // std::cout << ad::Tensor::compute_graph_ << std::endl;
    // std::cout << f << std::endl;
    std::cout << "grad of x1: " << x1.grad_ << std::endl;
    std::cout << "grad of x2: " << x2.grad_ << std::endl;
}
