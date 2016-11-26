#pragma once
#include<string>
#include<initializer_list>

class Complex {
	/*------------------��̬------------------*/
public:
	/*
	* ���ڼ��㸴����ģֵ
	*/
	static double abs(const Complex&c);

	/*------------------��------------------*/
public:
	/*
	* ʵ��
	*/
	double real;

	/*
	* �鲿
	*/
	double imag;

	/*------------------����------------------*/
	/*
	* Ĭ�Ϲ��캯��
	* ��ʼ��Ϊ(0,0)
	*/
	Complex();

	/*
	* ����һ��ʵ�εĹ��캯��
	* ��double���͵���Ϊʵ�����鲿Ϊ0
	* ���ܸ����͵���ʽת��
	*/
	Complex(double t_Real);

	/*
	* ��������ʵ�εĹ��캯��
	* �ֱ�Ϊʵ�����鲿
	*/
	Complex(double t_Real, double t_Imag);

	/*
	* �������캯��
	*/
	Complex(const Complex& c);

	/*
	* �б��ʼ��
	* �б��ȿ�����0,1,2,
	*		0��ʱ�򣬳�ʼ��Ϊ(0,0)
	*		1��ʱ�����ڳ�ʼ��ʵ��
	*		2��ʱ�����ڳ�ʼ��ʵ�����鲿
	*/
	Complex(const std::initializer_list<double> il);


	/*
	* ������ֵ�����
	*/
	Complex& operator=(const Complex& c);

	/*
	* ���ϸ�ֵ�����
	*/
	Complex& operator+=(const Complex& c);

	/*
	* ���ϸ�ֵ�����
	*/
	Complex& operator-=(const Complex& c);


	/*
	* ����
	*/
	Complex conjugate();

	/*
	* ���ɸ�ʽ���ַ���
	*/
	std::string toString();

	/*
	* ���ڴ�ӡ��ָ�������
	* Ĭ�ϵ������Ϊstd::out
	*/
	void print(std::ostream&out = std::cout);
};

/*
* ȡ�������
*/
Complex operator-(const Complex& c);

/*
* �ӷ������
*/
Complex operator+(const Complex& c1, const Complex& c2);

/*
* ���������
*/
Complex operator-(const Complex& c1, const Complex& c2);

/*
* �˷������
*/
Complex operator*(const Complex& c1, const Complex& c2);

/*
* ���������
*/
Complex operator/(const Complex& c1, const Complex& c2);

/*
* ��������
*/
bool operator==(const Complex& c1, const Complex& c2);

/*
* ���������
*/
bool operator!=(const Complex& c1, const Complex& c2);
