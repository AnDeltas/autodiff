#include "FMode.h"

namespace ad {
	FTangent::FTangent(const double x, const double dx = 1): x_(x), dx_(dx) { }

	double FTangent::GetX() const { return x_; }
	void FTangent::SetX(const double x) { x_ = x; }
	double FTangent::GetDx() const { return dx_; }
	void FTangent::SetDx(const double dx) { dx_ = dx; }

	FTangent FTangent::operator+(const FTangent& obj) const {
		return FTangent{GetX() + obj.GetX(), GetDx() + obj.GetDx()};
	}

	FTangent FTangent::operator-(const FTangent& obj) const {
		return FTangent{GetX() - obj.GetX(), GetDx() - obj.GetDx()};
	}

	FTangent FTangent::operator*(const FTangent& obj) const {
		return FTangent{GetX() * obj.GetX(), GetX() * obj.GetDx() + obj.GetX() * GetDx()};
	}

	FTangent FTangent::operator/(const FTangent& obj) const {
		return FTangent{
			GetX() / obj.GetX(),
			GetDx() / obj.GetX() - GetX() * obj.GetDx() / std::pow(obj.GetX(), 2)
		};
	}

	FTangent FTangent::operator+(const double x) const {
		return FTangent{GetX() + x, GetDx()};
	}

	FTangent FTangent::operator-(const double x) const {
		return FTangent{GetX() - x, GetDx()};
	}

	FTangent FTangent::operator*(const double x) const {
		return FTangent{GetX() * x, x * GetDx()};
	}

	FTangent FTangent::operator/(const double x) const {
		return FTangent{GetX() / x, GetDx() / x};
	}

	FTangent sin(const FTangent& obj) {
		return FTangent{std::sin(obj.GetX()), std::cos(obj.GetX()) * obj.GetDx()};
	}

	FTangent cos(const FTangent& obj) {
		return FTangent{std::cos(obj.GetX()), -std::sin(obj.GetX()) * obj.GetDx()};
	}

	FTangent ln(const FTangent& obj) {
		return FTangent{std::log(obj.GetX()), obj.GetDx() / obj.GetX()};
	}


	std::ostream& operator<<(std::ostream& os, const FTangent& obj) {
		return os
			<< "value: " << obj.GetX()
			<< ", grad: " << obj.GetDx();
	}
}
