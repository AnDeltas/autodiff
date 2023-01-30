#include <iostream>
#include "pointer_graph.h"
#include "backward_mode_tensor.h"

int main(int argc, char* argv[]) {
    ad::BackwardModeTensor& x1 = ad::tensor(2);
    ad::BackwardModeTensor& x2 = ad::tensor(5);

    ad::BackwardModeTensor& f = ad::ln(x1) + x1 * x2 - ad::sin(x2);

    f.Backward();

    std::cout << "grad of x1: " << x1.grad_ << std::endl;
    std::cout << "grad of x2: " << x2.grad_ << std::endl;
}
