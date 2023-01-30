#pragma once
#include <ostream>

namespace ad {
	class ForwardModeTensor {
	private:
		double x_;
		double dx_;

	public:
		explicit ForwardModeTensor(double x, double dx);

		ForwardModeTensor operator+(const ForwardModeTensor& obj) const;
		ForwardModeTensor operator-(const ForwardModeTensor& obj) const;
		ForwardModeTensor operator*(const ForwardModeTensor& obj) const; 
		ForwardModeTensor operator/(const ForwardModeTensor& obj) const; 
		ForwardModeTensor operator+(const double x) const;
		ForwardModeTensor operator-(const double x) const;
		ForwardModeTensor operator*(const double x) const;
		ForwardModeTensor operator/(const double x) const;

		double GetX() const;
		void SetX(const double x);
		double GetDx() const;
		void SetDx(const double dx);

		friend std::ostream& operator<<(std::ostream& os, const ForwardModeTensor& obj);
	};

	ForwardModeTensor sin(const ForwardModeTensor& obj);
	ForwardModeTensor cos(const ForwardModeTensor& obj);
	ForwardModeTensor ln(const ForwardModeTensor& obj);

}
