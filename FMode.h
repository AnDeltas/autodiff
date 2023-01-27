#pragma once
#include <ostream>
#include <cmath>

namespace ad {
	class FTangent {
	private:
		double x_;
		double dx_;

	public:
		explicit FTangent(double x, double dx);

		FTangent operator+(const FTangent& obj) const;
		FTangent operator-(const FTangent& obj) const;
		FTangent operator*(const FTangent& obj) const; 
		FTangent operator/(const FTangent& obj) const; 
		FTangent operator+(const double x) const;
		FTangent operator-(const double x) const;
		FTangent operator*(const double x) const;
		FTangent operator/(const double x) const;

		double GetX() const;
		void SetX(const double x);
		double GetDx() const;
		void SetDx(const double dx);

		friend std::ostream& operator<<(std::ostream& os, const FTangent& obj);
	};

	FTangent sin(const FTangent& obj);
	FTangent cos(const FTangent& obj);
	FTangent ln(const FTangent& obj);

}
