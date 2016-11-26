#pragma once
#include<vector>
#include<random>
#include<string>
#include<utility>
#include<iostream>
#include"Complex.h"

class RowVector {
	/*------------------��------------------*/
private:
	/*
	* �ײ���������Ϊ˽�н�ֱֹ�ӷ���
	*/
	Complex* rowVector;
public:
	/*
	* �������ȣ��������������������Ϊ����
	*/
	int col;


	/*------------------����------------------*/
private:
	/*
	* ��Դ�������ͷŶ��ڴ�
	*/
	void free();
public:
	/*
	* ���������������ͷ�rowVector
	*/
	~RowVector();

	/*
	* Ĭ�Ϲ��캯��
	* ��������Ϊ0��ָ��Ϊ��
	*/
	RowVector();

	/*
	* ����һ������ά�ȵĲ���
	* ��ʼ��������ά�ȵ���ָ����t_Col��Ԫ�س�ʼ��΢Ϊ��ֵ(0,0)
	*/
	explicit RowVector(int t_Col);

	/*
	* �������캯��
	* ע����ָ���Աʱ�����������ʵ��
	*/
	RowVector(const RowVector& t_RowVector);

	/*
	* �ƶ����캯��
	* ע���ƶ���Դ����ָ���Ա���ÿ�
	* ����Ϊ���׳��쳣��
	*/
	RowVector(RowVector&& t_RowVector) noexcept;

	/*
	* �б��캯��
	*/
	RowVector(const std::initializer_list<Complex> t_InitializeList);


	/*
	* ������ֵ�����
	*/
	RowVector& operator=(const RowVector& t_RowVector);

	/*
	* �ƶ���ֵ�����
	*/
	RowVector& operator=(RowVector&& t_RowVector) noexcept;

	/*
	* �±������
	*/
	Complex& operator[](int t_Pos);

	/*
	* �����汾���±������
	*/
	const Complex& operator[](int t_Pos) const;

	/*
	* ���ô�С
	* ���ú���ά�ȱ����ô��(0,0)
	* ���ú���ά�ȱ�С����ôɾ������Ԫ��
	*/
	void resize(int t_Size);

	/*
	* ���������ĸ�ʽ���ַ���
	*/
	std::string toString();

	/*
	* ��������ӡ��ָ�������
	*/
	void print(std::ostream&t_Out = std::cout);

};

/*
* ������Ŀȡ�������
*/
RowVector operator-(const RowVector& t_RowVector);

/*
* �����ӷ�����������
*/
RowVector operator+(const RowVector& t_RowVector1, const RowVector& t_RowVector2);
RowVector operator-(const RowVector& t_RowVector1, const RowVector& t_RowVector2);

/*
* �����븴��(����������)�ļӼ��˳������
*/
RowVector operator+(const RowVector& t_RowVector, const Complex&t_Complex);
RowVector operator+(const Complex&t_Complex, const RowVector& t_RowVector);

RowVector operator-(const RowVector& t_RowVector, const Complex&t_Complex);
RowVector operator-(const Complex&t_Complex, const RowVector& t_RowVector);

RowVector operator*(const RowVector& t_RowVector, const Complex&t_Complex);
RowVector operator*(const Complex&t_Complex, const RowVector& t_RowVector);

RowVector operator/(const RowVector& t_RowVector, const Complex&t_Complex);
RowVector operator/(const Complex&t_Complex, const RowVector& t_RowVector);

/*
* �����������Ķ�ӦԪ�س˷������
*/
RowVector elementProduct(const RowVector& t_RowVector1, const RowVector& t_RowVector2);
RowVector elementDivide(const RowVector& t_RowVector1, const RowVector& t_RowVector2);


class Matrix {
	/*------------------��̬------------------*/
public:
	/*
	* ��������棬���๲��
	*/
	static std::default_random_engine s_Engine;

	/*
	* ��������ºϲ��������ά�ȱ�����ͬ
	*/
	static Matrix verticalMerge(const Matrix& t_Matrix1, const Matrix& t_Matrix2);

	/*
	* ����Ĵ�ֱ����
	*/
	static std::pair<Matrix, Matrix> verticalSplit(const Matrix& t_Matrix, int t_LeftCol, int t_RightCol);

	/*
	* �����ˮƽ����
	*/
	static std::pair<Matrix, Matrix> horizonSplit(const Matrix& t_Matrix, int t_UpRow, int t_DownRow);

	/*
	* ����ָ��ά�ȵĵ�λ����
	*/
	static Matrix eye(const int t_Dim);
	/*------------------��------------------*/
private:
	/*
	* �ײ�������������Ϊ˽�н�ֱֹ�ӷ���
	*/
	RowVector* matrix;
public:
	/*
	* ����
	*/
	int row;

	/*
	* ����
	*/
	int col;

	/*------------------����------------------*/
private:
	/*
	* ������ڴ�
	*/
	void free();
public:
	/*
	* ��������
	*/
	~Matrix();

	/*
	* Ĭ�Ϲ��캯��
	* ���о�Ϊ0��ָ��Ϊ��
	*/
	Matrix();

	/*
	* ָ��ά�ȵĹ��캯��
	*/
	Matrix(int t_Row, int t_Col);

	/*
	* �������캯����ע����ָ��ʱ���������ʵ��
	*/
	Matrix(const Matrix& t_Matrix);

	/*
	* �ƶ����캯����ע����ָ��ʱ�ƶ������ʵ��
	*/
	Matrix(Matrix&& t_Matrix) noexcept;

	/*
	* �б��캯��
	* �б���ÿ�е�Ԫ����Ŀ���Բ�ͬ
	* �����һ�е�ά����Ϊ����
	* ����Ĳ���(0,0)
	*/
	Matrix(const std::initializer_list<RowVector>& t_InitializeList);

	/*---�����ܺ���---*/
public:
	/*
	* ��������ȷֲ�����ֵ
	* ʵ���ķ�Χ[t_RealLeft,t_RealRight]
	* �鲿�ķ�Χ[t_ImagLeft,t_ImagRight]
	*/
	void randomFill(double t_RealLeft, double t_RealRight, double t_ImagLeft, double t_ImagRight);

	/*
	* ����
	*/
	Matrix conjugate();

	/*
	* ��ת��
	*/
	Matrix transpose();

	/*
	* ����ת��
	*/
	Matrix hermitian();

	/*
	* �������
	* ������ֵΪtrueʱ���������޷����棬�᳢����α��(���������)
	*/
	Matrix inverse(bool t_TryPseudoInverse = false);

	/*
	* ��Խ��߹��ɵľ���(������)
	*/
	Matrix diag();

	/*
	* �����ȷֽ�
	*/
	std::pair<Matrix, Matrix> fullRankDecomposition();

	/*
	* ����������
	*/
	Matrix pseudoInverse();


	/*---��Ա���������---*/
public:
	/*
	* ������ֵ�����
	* ע�⣬��ָ���Աʱ��Ҳ��Ҫ����Ը�ֵ����ȷ��
	* ע��ԭָ��������ʱ��
	*/
	Matrix& operator=(const Matrix& t_Matrix);

	/*
	* �ƶ���ֵ�����
	* ��Ҫ����Ը�ֵ����ȷ��
	* ע��ԭָ��������ʱ��
	* ����Ϊ�����׳��쳣
	*/
	Matrix& operator=(Matrix&& t_Matrix) noexcept;

	/*
	* �±������(�ǳ����汾)
	*/
	RowVector& operator[](int t_Pos);

	/*
	* �±������(�����汾)
	*/
	const RowVector& operator[](int t_Pos) const;


	/*---�������ܺ���---*/
public:
	/*
	* ���ɸ�ʽ���ַ���
	*/
	std::string toString();

	/*
	* ��ӡ��ָ�������
	*/
	void print(std::ostream&t_Out = std::cout, int t_NumEnter = 0);


private:
	/*
	* ���ڼ��㵱ά��С��3ʱ�ľ�����������
	* ͬ��������ֵΪtrueʱ���޷���������α��(���������)
	*/
	Matrix inverseWhenDimlowerThan3(bool t_TryPseudoInverse);
};

/*
* ��Ŀȡ�������
*/
Matrix operator-(const Matrix& t_Matrix);


/*
* ���������㣬���ӡ�������
*/
Matrix operator+(const Matrix& t_Matrix1, const Matrix& t_Matrix2);
Matrix operator-(const Matrix& t_Matrix1, const Matrix& t_Matrix2);
Matrix operator*(const Matrix& t_Matrix1, const Matrix& t_Matrix2);

/*
* �����븴�������㣬���ӡ������ˡ���
*/
Matrix operator+(const Matrix& t_Matrix, const Complex& t_Complex);
Matrix operator+(const Complex& t_Complex, const Matrix& t_Matrix);

Matrix operator-(const Matrix& t_Matrix, const Complex& t_Complex);
Matrix operator-(const Complex& t_Complex, const Matrix& t_Matrix);

Matrix operator*(const Matrix& t_Matrix, const Complex& t_Complex);
Matrix operator*(const Complex& t_Complex, const Matrix& t_Matrix);

Matrix operator/(const Matrix& t_Matrix, const Complex& t_Complex);
Matrix operator/(const Complex& t_Complex, const Matrix& t_Matrix);


/*
* ��Ӧλ��Ԫ�ؼ���
*/
Matrix elementProduct(const Matrix& t_Matrix1, const Matrix& t_Matrix2);
Matrix elementDivide(const Matrix& t_Matrix1, const Matrix& t_Matrix2);
