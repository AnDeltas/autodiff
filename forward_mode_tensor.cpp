#include "forward_mode_tensor.h"

namespace ad {
	ForwardModeTensor::ForwardModeTensor(const double x, const double dx = 1): x_(x), dx_(dx) { }

	double ForwardModeTensor::GetX() const { return x_; }
	void ForwardModeTensor::SetX(const double x) { x_ = x; }
	double ForwardModeTensor::GetDx() const { return dx_; }
	void ForwardModeTensor::SetDx(const double dx) { dx_ = dx; }

	ForwardModeTensor ForwardModeTensor::operator+(const ForwardModeTensor& obj) const {
		return ForwardModeTensor{GetX() + obj.GetX(), GetDx() + obj.GetDx()};
	}

	ForwardModeTensor ForwardModeTensor::operator-(const ForwardModeTensor& obj) const {
		return ForwardModeTensor{GetX() - obj.GetX(), GetDx() - obj.GetDx()};
	}

	ForwardModeTensor ForwardModeTensor::operator*(const ForwardModeTensor& obj) const {
		return ForwardModeTensor{GetX() * obj.GetX(), GetX() * obj.GetDx() + obj.GetX() * GetDx()};
	}

	ForwardModeTensor ForwardModeTensor::operator/(const ForwardModeTensor& obj) const {
		return ForwardModeTensor{
			GetX() / obj.GetX(),
			GetDx() / obj.GetX() - GetX() * obj.GetDx() / std::pow(obj.GetX(), 2)
		};
	}

	ForwardModeTensor ForwardModeTensor::operator+(const double x) const {
		return ForwardModeTensor{GetX() + x, GetDx()};
	}

	ForwardModeTensor ForwardModeTensor::operator-(const double x) const {
		return ForwardModeTensor{GetX() - x, GetDx()};
	}

	ForwardModeTensor ForwardModeTensor::operator*(const double x) const {
		return ForwardModeTensor{GetX() * x, x * GetDx()};
	}

	ForwardModeTensor ForwardModeTensor::operator/(const double x) const {
		return ForwardModeTensor{GetX() / x, GetDx() / x};
	}

	ForwardModeTensor sin(const ForwardModeTensor& obj) {
		return ForwardModeTensor{std::sin(obj.GetX()), std::cos(obj.GetX()) * obj.GetDx()};
	}

	ForwardModeTensor cos(const ForwardModeTensor& obj) {
		return ForwardModeTensor{std::cos(obj.GetX()), -std::sin(obj.GetX()) * obj.GetDx()};
	}

	ForwardModeTensor ln(const ForwardModeTensor& obj) {
		return ForwardModeTensor{std::log(obj.GetX()), obj.GetDx() / obj.GetX()};
	}


	std::ostream& operator<<(std::ostream& os, const ForwardModeTensor& obj) {
		return os
			<< "value: " << obj.GetX()
			<< ", grad: " << obj.GetDx();
	}
}
