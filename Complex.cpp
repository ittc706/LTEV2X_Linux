/*
* =====================================================================================
*
*       Filename:  Complex.cpp
*
*    Description:  Complexʵ��
*
*        Version:  1.0
*        Created:
*       Revision:
*       Compiler:  VS_2015
*
*         Author:  HCF
*            LIB:  ITTC
*
* =====================================================================================
*/

#include<cmath>
#include<sstream>
#include<iomanip>
#include<iostream>
#include<stdexcept>
#include"Complex.h"

using namespace std;

double Complex::abs(const Complex&c) {
	double res = 0;
	res += sqrt(c.real*c.real + c.imag*c.imag);
	return res;
}


Complex::Complex() :
	real(0), imag(0) {}


Complex::Complex(double t_Real) :
	real(t_Real), imag(0) {}


Complex::Complex(double t_Real, double t_Imag) :
	real(t_Real), imag(t_Imag) {}


Complex::Complex(const Complex& c) :
	real(c.real), imag(c.imag) {}


Complex::Complex(const initializer_list<double> il) {
	if (il.size() > 2) throw logic_error("Complex��ʼ���б�Ԫ����Ŀ����Ӧ��Ϊ����double���͵�Ԫ��");
	initializer_list<double>::iterator it = il.begin();
	switch (il.size()) {
	case 0:
		real = 0;
		imag = 0;
		break;
	case 1:
		real = *it;
		imag = 0;
		break;
	case 2:
		real = *it++;
		imag = *it;
	}
}


Complex& Complex::operator =(const Complex& c) {
	this->real = c.real;
	this->imag = c.imag;
	return *this;
}
Complex& Complex::operator +=(const Complex& c) {
	this->real += c.real;
	this->imag += c.imag;
	return *this;
}
Complex& Complex::operator -=(const Complex& c) {
	this->real -= c.real;
	this->imag -= c.imag;
	return *this;
}


Complex Complex::conjugate() {
	Complex res;
	res.real = real;
	res.imag = -imag;
	return res;
}


string Complex::toString() {
	ostringstream ss;
	ss.precision(2);
	//ss << fixed;//�����ÿ�ѧ����������Ϊ���ܻ���ڼ�С����ֵ
	ss << "[" << left << setw(2) << real <<"+ j"<< left << setw(2) << imag << "]";
	return ss.str();
}

void Complex::print(ostream&out) {
	out << toString() << endl;
}


Complex operator-(const Complex& c) {
	Complex res;
	res.real = -c.real;
	res.imag = -c.imag;
	return res;
}

Complex operator+(const Complex& c1, const Complex& c2) {
	return Complex(c1.real + c2.real, c1.imag + c2.imag);
}


Complex operator-(const Complex& c1, const Complex& c2) {
	return Complex(c1.real - c2.real, c1.imag - c2.imag);
}


Complex operator*(const Complex& c1, const Complex& c2) {
	double realPart = 0;
	double imagPart = 0;
	realPart = c1.real*c2.real - c1.imag*c2.imag;
	imagPart = c1.real*c2.imag + c2.real*c1.imag;
	return Complex(realPart, imagPart);
}


Complex operator/(const Complex& c1, const Complex& c2) {
	double tmp = Complex::abs(c2);
	double denominator = tmp*tmp;//��ĸ
	if (denominator == 0) throw logic_error("Complex.cpp->Complex operator/(const Complex& c1, const Complex& c2)");

	double realPart = 0;
	double imagPart = 0;
	realPart = (c1.real*c2.real + c1.imag*c2.imag) / denominator;
	imagPart = (-c1.real*c2.imag + c2.real*c1.imag) / denominator;
	return Complex(realPart, imagPart);
}


bool operator==(const Complex& c1, const Complex& c2) {
	return c1.real == c2.real&&c1.imag == c2.imag;
}


bool operator!=(const Complex& c1, const Complex& c2) {
	return !(c1 == c2);
}