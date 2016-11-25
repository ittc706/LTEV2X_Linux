/*
* =====================================================================================
*
*       Filename:  Matrix.cpp
*
*    Description:  Matrixʵ��
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


#include<iostream>
#include<iomanip>
#include<sstream>
#include<cmath>
#include<string.h>
#include"Matrix.h"
#include"Function.h"

using namespace std;

void RowVector::free() {
	Delete::safeDelete(rowVector, true);
}

RowVector::~RowVector() {
	free();
}


RowVector::RowVector() :
	col(0) {
	rowVector = nullptr;
}


RowVector::RowVector(int t_Col) :
	col(t_Col) {
	if (col < 0) throw LTEV2X_Exception("������ά�ȱ����ǷǸ���");
	rowVector = new Complex[col]();
}


RowVector::RowVector(const RowVector& t_RowVector) :
	col(t_RowVector.col) {
	rowVector = new Complex[col]();
	memcpy(rowVector, t_RowVector.rowVector, col * sizeof(Complex));
}


RowVector::RowVector(RowVector&& t_RowVector) noexcept
	:col(t_RowVector.col), rowVector(t_RowVector.rowVector) {
	//�ڳ�ʼ���б�ӹ���Դ
	t_RowVector.rowVector = nullptr;//�ÿ�ָ��
}


RowVector::RowVector(const initializer_list<Complex> t_InitializeList) {
	col = static_cast<int>(t_InitializeList.size());
	rowVector = new Complex[col]();
	int iter = 0;
	for (const Complex&c : t_InitializeList)
		rowVector[iter++] = c;
}


RowVector& RowVector::operator=(const RowVector& t_RowVector) {
	if (rowVector != t_RowVector.rowVector) {//����Ը�ֵ����ȷ��
		free();
		col = t_RowVector.col;
		rowVector = new Complex[col]();
		memcpy(rowVector, t_RowVector.rowVector, col * sizeof(Complex));
	}
	return *this;
}


RowVector& RowVector::operator=(RowVector&& t_RowVector) noexcept {
	if (rowVector != t_RowVector.rowVector) {//����Ը�ֵ����ȷ��
		free();//������Դ
		col = t_RowVector.col;
		rowVector = t_RowVector.rowVector;//�ӹ���Դ
		t_RowVector.rowVector = nullptr;//�ÿ�ָ��
	}
	return *this;
}


Complex& RowVector::operator[](int t_Pos) {
	return rowVector[t_Pos];
}


const Complex& RowVector::operator[](int t_Pos) const {
	return rowVector[t_Pos];
}


void RowVector::resize(int t_Size) {
	if (t_Size < 0) throw LTEV2X_Exception("������ά�ȱ����ǷǸ���");
	int preCol = col;
	Complex* preRowVector = rowVector;
	col = t_Size;
	rowVector = new Complex[col]();
	int copyCol = col < preCol ? col : preCol;
	memcpy(rowVector, preRowVector, copyCol * sizeof(Complex));
	/*-----------------------ATTENTION-----------------------
	* ���ﲻ�ܵ���free()������ԭ������Դ
	* free()���ͷ�thisָ��Ķ����rowVectorָ��
	* ����ʱ����rowVectorָ���Ѿ�ָ�����µ�����
	* �����ֶ�ɾ��֮ǰ��ָ��
	*------------------------ATTENTION-----------------------*/
	Delete::safeDelete(preRowVector, true);
}


string RowVector::toString() {
	ostringstream ss;
	for (int c = 0; c < col; c++) {
		ss << this->operator[](c).toString() << "  ";
	}
	return ss.str();
}


void RowVector::print(ostream&t_Out) {
	t_Out << toString();
}

//��������Ŀȡ�������
RowVector operator-(const RowVector& t_RowVector) {
	RowVector res(t_RowVector.col);
	for (int c = 0; c < t_RowVector.col; c++) {
		res[c] = -t_RowVector[c];
	}
	return res;
}


//��������������������
RowVector operator+(const RowVector& t_RowVector1, const RowVector& t_RowVector2) {
	if (t_RowVector1.col != t_RowVector2.col) throw LTEV2X_Exception("����ά�Ȳ�ͬ���޷����");
	RowVector res(t_RowVector1.col);
	for (int c = 0; c < t_RowVector1.col; c++) {
		res[c] = t_RowVector1[c] + t_RowVector2[c];
	}
	return res;
}


//��������������������
RowVector operator-(const RowVector& t_RowVector1, const RowVector& t_RowVector2) {
	if (t_RowVector1.col != t_RowVector2.col) throw LTEV2X_Exception("����ά�Ȳ�ͬ���޷����");
	RowVector res(t_RowVector1.col);
	for (int c = 0; c < t_RowVector1.col; c++) {
		res[c] = t_RowVector1[c] - t_RowVector2[c];
	}
	return res;
}


//�������븴��������
RowVector operator+(const RowVector& t_RowVector, const Complex&t_Complex) {
	RowVector res(t_RowVector.col);
	for (int c = 0; c < t_RowVector.col; c++) {
		res[c] = t_RowVector[c] + t_Complex;
	}
	return res;
}
RowVector operator+(const Complex&t_Complex, const RowVector& t_RowVector) {
	return t_RowVector + t_Complex;
}


RowVector operator-(const RowVector& t_RowVector, const Complex&t_Complex) {
	RowVector res(t_RowVector.col);
	for (int c = 0; c < t_RowVector.col; c++) {
		res[c] = t_RowVector[c] - t_Complex;
	}
	return res;
}
RowVector operator-(const Complex&t_Complex, const RowVector& t_RowVector) {
	return -t_RowVector + t_Complex;
}


RowVector operator*(const RowVector& t_RowVector, const Complex&t_Complex) {
	RowVector res(t_RowVector.col);
	for (int c = 0; c < t_RowVector.col; c++) {
		res[c] = t_RowVector[c] * t_Complex;
	}
	return res;
}
RowVector operator*(const Complex&t_Complex, const RowVector& t_RowVector) {
	return t_RowVector*t_Complex;
}


RowVector operator/(const RowVector& t_RowVector, const Complex&t_Complex) {
	if (Complex::abs(t_Complex) == 0) throw LTEV2X_Exception("����ģֵ����Ϊ0");
	RowVector res(t_RowVector.col);
	for (int c = 0; c < t_RowVector.col; c++) {
		res[c] = t_RowVector[c] / t_Complex;
	}
	return res;
}
RowVector operator/(const Complex&t_Complex, const RowVector& t_RowVector) {
	RowVector res(t_RowVector.col);
	for (int c = 0; c < t_RowVector.col; c++) {
		if (Complex::abs(t_RowVector[c]) == 0) throw LTEV2X_Exception("�����к���ģֵΪ0��Ԫ�أ�������Ϊ��ĸ");
		res[c] = t_Complex / t_RowVector[c];
	}
	return res;
}


RowVector elementProduct(const RowVector& t_RowVector1, const RowVector& t_RowVector2) {
	if (t_RowVector1.col != t_RowVector2.col) throw LTEV2X_Exception("����ά�Ȳ�ƥ��");
	RowVector res(t_RowVector1.col);
	for (int c = 0; c < t_RowVector1.col; c++) {
		res[c] = t_RowVector1[c] * t_RowVector2[c];
	}
	return res;
}
RowVector elementDivide(const RowVector& t_RowVector1, const RowVector& t_RowVector2) {
	if (t_RowVector1.col != t_RowVector2.col) throw LTEV2X_Exception("����ά�Ȳ�ƥ��");
	RowVector res(t_RowVector1.col);
	for (int c = 0; c < t_RowVector1.col; c++) {
		if (Complex::abs(t_RowVector2[c]) == 0) throw LTEV2X_Exception("Ԫ��ģֵΪ0��������Ϊ����");
		res[c] = t_RowVector1[c] / t_RowVector2[c];
	}
	return res;
}


default_random_engine Matrix::s_Engine((unsigned)time(NULL));


void Matrix::free() {
	Delete::safeDelete(matrix, true);
}


Matrix:: ~Matrix() {
	free();
}


Matrix::Matrix() :
	row(0), col(0) {
	matrix = nullptr;
}



Matrix::Matrix(int t_Row, int t_Col) :
	row(t_Row), col(t_Col) {
	if (t_Row < 0 || t_Col < 0) throw LTEV2X_Exception("�����ά�ȱ����ǷǸ���");
	matrix = new RowVector[row];
	for (int iter = 0; iter < row; iter++) {
		matrix[iter] = RowVector(col);
	}
}


Matrix::Matrix(const Matrix& t_Matrix) :
	row(t_Matrix.row), col(t_Matrix.col) {
	matrix = new RowVector[row];
	for (int iter = 0; iter < row; iter++) {
		/*-----------------------ATTENTION-----------------------
		* ����RowVector�Ŀ�����ֵ�����
		* ���ﲻ�ܵ���memcpy������matrix��Ա����ΪҪʵ�ֿ�����ֵ������
		* memcpy�Ὣt_Matrix.matrix�ڴ������ֵ��ȫ��ֵһ�ݸ�this->matrix
		* ���Ǹ��ڴ�������RowVector���󣬶��ö��������ָ��
		* �����ᵼ�¸���ǰ��RowVector�е�ָ��һ��
		*------------------------ATTENTION-----------------------*/
		matrix[iter] = t_Matrix[iter];
	}
}


Matrix::Matrix(Matrix&& t_Matrix) noexcept
	:row(t_Matrix.row), col(t_Matrix.col), matrix(t_Matrix.matrix) {
	//�ڳ�ʼ���б��нӹ���Դ
	t_Matrix.matrix = nullptr;//�ÿ�ָ��
}


Matrix::Matrix(const initializer_list<RowVector>& t_InitializeList) {
	if (t_InitializeList.size() == 0) {
		row = 0;
		col = 0;
		matrix = nullptr;
	}
	else {
		col = 0;
		row = static_cast<int>(t_InitializeList.size());
		matrix = new RowVector[row];
		for (const RowVector& rv : t_InitializeList) {
			if (col < rv.col) col = rv.col;
		}
		int iter = 0;
		for (const RowVector& rv : t_InitializeList) {
			RowVector rvTmp(rv);
			rvTmp.resize(col);
			matrix[iter++] = std::move(rvTmp);
		}
	}
}


void Matrix::randomFill(double t_RealLeft, double t_RealRight, double t_ImagLeft, double t_ImagRight) {
	uniform_real_distribution<double> urdReal(t_RealLeft, t_RealRight);
	uniform_real_distribution<double> urdImag(t_ImagLeft, t_ImagRight);
	for (int r = 0; r < row; r++) {
		for (int c = 0; c < col; c++) {
			matrix[r][c] = Complex(urdReal(Matrix::s_Engine), urdImag(Matrix::s_Engine));
		}
	}
}


Matrix Matrix::conjugate() {
	Matrix res(row, col);
	for (int r = 0; r < row; r++) {
		for (int c = 0; c < col; c++) {
			res[r][c] = matrix[r][c].conjugate();
		}
	}
	return res;
}


Matrix Matrix::transpose() {
	Matrix res(col, row);
	for (int r = 0; r < col; r++) {
		for (int c = 0; c < row; c++) {
			res[r][c] = matrix[c][r];
		}
	}
	return res;
}


Matrix Matrix::hermitian() {
	Matrix res = conjugate();
	res = res.transpose();
	return res;
}


Matrix Matrix::inverse(bool t_TryPseudoInverse) {
	if (row <= 0 || col <= 0 || row != col) throw LTEV2X_Exception("�þ����޷�����");

	if (row < 3) return inverseWhenDimlowerThan3(t_TryPseudoInverse);

	Matrix mergeMatrix = Matrix::verticalMerge(*this, Matrix::eye(row));

	//�ȱ任�������Ǿ���
	Complex zero(0, 0);
	int tmpRow = 0;
	RowVector tmpRV;
	for (int r = 0; r < row; r++) {
		if (mergeMatrix[r][r] == zero) {//����ǰ�еĶԽ��߲���Ϊ0�����벻Ϊ0�����л���
			tmpRow = r + 1;
			while (tmpRow < row&&mergeMatrix[tmpRow][r] == zero)
				tmpRow++;
			if (tmpRow == row) {
				if (t_TryPseudoInverse) {
					return this->pseudoInverse();
				}
				else {
					throw LTEV2X_Exception("�þ������ȣ��޷��������");
				}
			}

			//����r��tmp����
			tmpRV = mergeMatrix[r];
			mergeMatrix[r] = mergeMatrix[tmpRow];
			mergeMatrix[tmpRow] = tmpRV;
		}
		mergeMatrix[r] = mergeMatrix[r] / mergeMatrix[r][r];//���Խ��߲�����1

															//�����е��°벿����0
		for (tmpRow = r + 1; tmpRow < row; tmpRow++) {
			if (mergeMatrix[tmpRow][r] == zero)continue;
			Complex factor = -mergeMatrix[tmpRow][r];
			mergeMatrix[tmpRow] = mergeMatrix[r] * factor + mergeMatrix[tmpRow];
		}
	}


	//���ұ߼����任Ϊ������
	for (int r = row - 1; r >= 0; r--) {
		for (tmpRow = r - 1; tmpRow >= 0; tmpRow--) {
			if (mergeMatrix[tmpRow][r] == zero)continue;
			Complex factor = -mergeMatrix[tmpRow][r];
			mergeMatrix[tmpRow] = mergeMatrix[r] * factor + mergeMatrix[tmpRow];
		}
	}
	return Matrix::verticalSplit(mergeMatrix, col, col).second;
}


Matrix Matrix::inverseWhenDimlowerThan3(bool t_TryPseudoInverse) {
	if (row == 1) {
		if (Complex::abs(this->operator[](0)[0]) == 0) {
			if (t_TryPseudoInverse)
				return Matrix{ { 0,0 } };
			else
				throw LTEV2X_Exception("�þ����޷�����");
		}
		return Matrix{ { 1 / this->operator[](0)[0] } };
	}
	else {
		Matrix res(row, col);
		Complex denominator = this->operator[](0)[0] * this->operator[](1)[1] - this->operator[](0)[1] * this->operator[](1)[0];
		if (denominator == 0) {
			if (t_TryPseudoInverse) return pseudoInverse();
			else throw LTEV2X_Exception("�þ����޷�����");
		}
		res[0][0] = this->operator[](1)[1] / denominator;
		res[0][1] = -(this->operator[](0)[1]) / denominator;
		res[1][0] = -(this->operator[](1)[0]) / denominator;
		res[1][1] = this->operator[](0)[0] / denominator;
		return res;
	}
}


Matrix Matrix::diag() {
	if (row != col) throw LTEV2X_Exception("�ú���ֻ֧�ַ���");
	Matrix res(1, row);
	for (int r = 0; r < row; r++) {
		res[0][r] = this->operator[](r)[r];
	}
	return res;
}


pair<Matrix, Matrix>  Matrix::fullRankDecomposition() {
	//������ά��Ϊm*n����Ϊr��r����Ϊ1
	Matrix mergeMatrix = Matrix::verticalMerge(*this, Matrix::eye(row));

	//���Ƚ�����ת��Ϊ��׼�ͣ���������r*n�������Ⱦ���������(m-r)*n�������
	Complex zero(0, 0);
	int tmpRow = 0;
	RowVector tmpRV;
	int iterCol = 0;
	int rank = 0;
	for (int r = 0; r < row; r++) {
		if (iterCol == col) break;
		if (mergeMatrix[r][iterCol] == zero) {//����ǰ�еĶԽ��߲���Ϊ0�����벻Ϊ0�����л���
			tmpRow = r + 1;
			while (tmpRow < row&&mergeMatrix[tmpRow][iterCol] == zero)
				tmpRow++;
			if (tmpRow == row) { //������û�з���Ԫ�أ���������
				iterCol++;
				r--;
				continue;//�����ڵ�ǰ�е���һ��Ѱ�ҷ���Ԫ��
			}
			else {
				//����r��tmp����
				tmpRV = mergeMatrix[r];
				mergeMatrix[r] = mergeMatrix[tmpRow];
				mergeMatrix[tmpRow] = tmpRV;
			}
		}
		mergeMatrix[r] = mergeMatrix[r] / mergeMatrix[r][iterCol];//���Խ��߲�����1

																  //�����е��°벿����0
		for (tmpRow = r + 1; tmpRow < row; tmpRow++) {
			if (mergeMatrix[tmpRow][iterCol] == zero)continue;
			Complex factor = -mergeMatrix[tmpRow][iterCol];
			mergeMatrix[tmpRow] = mergeMatrix[iterCol] * factor + mergeMatrix[tmpRow];
		}
		iterCol++;
		rank++;
	}

	if (rank == 0) throw LTEV2X_Exception("�þ�����Ϊ0�����������ȷֽ������");

	pair<Matrix, Matrix> splitRes = verticalSplit(mergeMatrix, mergeMatrix.col - row, row);
	Matrix B = splitRes.first;
	Matrix P = splitRes.second;

	Matrix F = verticalSplit(P.inverse(), rank, P.col - rank).first;
	Matrix G = horizonSplit(B, rank, B.row - rank).first;

	return pair<Matrix, Matrix>(F, G);
}


Matrix Matrix::pseudoInverse() {
	pair<Matrix, Matrix> fullRankDecompositionRes = fullRankDecomposition();
	Matrix F = fullRankDecompositionRes.first;
	Matrix G = fullRankDecompositionRes.second;
	return G.hermitian()*(G*G.hermitian()).inverse()*(F.hermitian()*F).inverse()*F.hermitian();
}



Matrix& Matrix::operator=(const Matrix& t_Matrix) {
	if (matrix != t_Matrix.matrix) {//����Ը�ֵ����ȷ��
		free();
		row = t_Matrix.row;
		col = t_Matrix.col;
		matrix = new RowVector[row];
		for (int r = 0; r < row; r++) {
			/*-----------------------ATTENTION-----------------------
			* ����RowVector�Ŀ�����ֵ�����
			* ���ﲻ�ܵ���memcpy������matrix��Ա����ΪҪʵ�ֿ�����ֵ������
			* memcpy�Ὣt_Matrix.matrix�ڴ������ֵ��ȫ��ֵһ�ݸ�this->matrix
			* ���Ǹ��ڴ�������RowVector���󣬶��ö��������ָ��
			* �����ᵼ�¸���ǰ��RowVector�е�ָ��һ��
			*------------------------ATTENTION-----------------------*/
			matrix[r].operator=(t_Matrix.matrix[r]);
		}
	}
	return *this;
}


Matrix& Matrix::operator=(Matrix&& t_Matrix) noexcept {
	if (matrix != t_Matrix.matrix) {//����Ը�ֵ����ȷ��
		free();//������Դ
		row = t_Matrix.row;
		col = t_Matrix.col;
		matrix = t_Matrix.matrix;//�ӹ���Դ
		t_Matrix.matrix = nullptr;//�ÿ�ָ��
	}
	return *this;
}


RowVector& Matrix::operator[](int t_Pos) {
	return matrix[t_Pos];
}


const RowVector& Matrix::operator[](int t_Pos) const {
	return  matrix[t_Pos];
}


string Matrix::toString() {
	ostringstream ss;
	for (int r = 0; r < row; r++) {
		ss << this->operator[](r).toString() << endl;
	}
	return ss.str();
}


void Matrix::print(ostream&t_Out, int t_NumEnter) {
	t_Out << toString();
	for (int i = 0; i < t_NumEnter; i++)
		t_Out << endl;
}


Matrix Matrix::verticalMerge(const Matrix& t_Matrix1, const Matrix& t_Matrix2) {
	if (t_Matrix1.row != t_Matrix2.row) throw LTEV2X_Exception("�����������޷��ϲ�");
	Matrix res(t_Matrix1.row, t_Matrix1.col + t_Matrix2.col);
	for (int r = 0; r < t_Matrix1.row; r++) {
		for (int c = 0; c < t_Matrix1.col; c++) {
			res[r][c] = t_Matrix1[r][c];
		}
	}
	for (int r = 0; r < t_Matrix2.row; r++) {
		for (int c = 0; c < t_Matrix2.col; c++) {
			res[r][c + t_Matrix1.col] = t_Matrix2[r][c];
		}
	}
	return res;
}


pair<Matrix, Matrix> Matrix::verticalSplit(const Matrix& t_Matrix, int t_LeftCol, int t_RightCol) {
	if (t_Matrix.col != t_LeftCol + t_RightCol) throw LTEV2X_Exception("�þ����޷����ѳ�ָ��ά��");
	Matrix left(t_Matrix.row, t_LeftCol), right(t_Matrix.row, t_RightCol);
	for (int r = 0; r < left.row; r++) {
		for (int c = 0; c < left.col; c++) {
			left[r][c] = t_Matrix[r][c];
		}
	}

	for (int r = 0; r < right.row; r++) {
		for (int c = 0; c < right.col; c++) {
			right[r][c] = t_Matrix[r][c + left.col];
		}
	}
	return pair<Matrix, Matrix>(left, right);
}


pair<Matrix, Matrix> Matrix::horizonSplit(const Matrix& t_Matrix, int t_UpRow, int t_DownRow) {
	if (t_Matrix.row != t_UpRow + t_DownRow) throw LTEV2X_Exception("�þ����޷����ѳ�ָ��ά��");
	Matrix up(t_UpRow, t_Matrix.col), down(t_DownRow, t_Matrix.col);
	for (int r = 0; r < up.row; r++) {
		for (int c = 0; c < up.col; c++) {
			up[r][c] = t_Matrix[r][c];
		}
	}

	for (int r = 0; r < down.row; r++) {
		for (int c = 0; c < down.col; c++) {
			down[r][c] = t_Matrix[r + up.row][c];
		}
	}
	return pair<Matrix, Matrix>(up, down);
}

Matrix Matrix::eye(const int t_Dim) {
	if (t_Dim < 1) throw LTEV2X_Exception("��λ��ά������Ϊ1");
	Matrix res(t_Dim, t_Dim);
	for (int i = 0; i < t_Dim; i++) {
		res[i][i] = Complex(1, 0);
	}
	return res;
}


//��Ŀȡ�������
Matrix operator-(const Matrix& t_Matrix) {
	Matrix res(t_Matrix.row, t_Matrix.col);
	for (int r = 0; r < t_Matrix.row; r++) {
		res[r] = -t_Matrix[r];
	}
	return res;
}


//����������
Matrix operator+(const Matrix& t_Matrix1, const Matrix& t_Matrix2) {
	if (t_Matrix1.row != t_Matrix2.row || t_Matrix1.col != t_Matrix2.col) throw LTEV2X_Exception("����ά�Ȳ�ͬ���޷����");
	Matrix res(t_Matrix1.row, t_Matrix1.col);
	for (int r = 0; r < t_Matrix1.row; r++) {
		for (int c = 0; c < t_Matrix1.col; c++) {
			res[r][c] = t_Matrix1[r][c] + t_Matrix2[r][c];
		}
	}
	return res;
}


Matrix operator-(const Matrix& t_Matrix1, const Matrix& t_Matrix2) {
	if (t_Matrix1.row != t_Matrix2.row || t_Matrix1.col != t_Matrix2.col) throw LTEV2X_Exception("����ά�Ȳ�ͬ���޷����");
	Matrix res(t_Matrix1.row, t_Matrix1.col);
	for (int r = 0; r < t_Matrix1.row; r++) {
		for (int c = 0; c < t_Matrix1.col; c++) {
			res[r][c] = t_Matrix1[r][c] - t_Matrix2[r][c];
		}
	}
	return res;
}


Matrix operator*(const Matrix& t_Matrix1, const Matrix& t_Matrix2) {
	if (t_Matrix1.col != t_Matrix2.row) throw LTEV2X_Exception("����ά�Ȳ�ƥ�䣬�޷����");

	Matrix res(t_Matrix1.row, t_Matrix2.col);

	for (int r = 0; r < t_Matrix1.row; r++) {
		for (int c = 0; c < t_Matrix2.col; c++) {
			//����ÿһ��
			Complex tmp(0, 0);
			for (int idx = 0; idx < t_Matrix1.col; idx++) {
				tmp += t_Matrix1[r][idx] * t_Matrix2[idx][c];
			}
			res[r][c] = tmp;
		}
	}
	return res;
}


//�����븴��������
Matrix operator+(const Matrix& t_Matrix, const Complex& t_Complex) {
	Matrix res(t_Matrix.row, t_Matrix.col);
	for (int r = 0; r < t_Matrix.row; r++) {
		res[r] = t_Matrix[r] + t_Complex;
	}
	return res;
}
Matrix operator+(const Complex& t_Complex, const Matrix& t_Matrix) {
	return t_Matrix + t_Complex;
}


Matrix operator-(const Matrix& t_Matrix, const Complex& t_Complex) {
	Matrix res(t_Matrix.row, t_Matrix.col);
	for (int r = 0; r < t_Matrix.row; r++) {
		res[r] = t_Matrix[r] - t_Complex;
	}
	return res;
}
Matrix operator-(const Complex& t_Complex, const Matrix& t_Matrix) {
	return -t_Matrix + t_Complex;
}


Matrix operator*(const Matrix& t_Matrix, const Complex& t_Complex) {
	Matrix res(t_Matrix.row, t_Matrix.col);
	for (int r = 0; r < t_Matrix.row; r++) {
		res[r] = t_Matrix[r] * t_Complex;
	}
	return res;
}
Matrix operator*(const Complex& t_Complex, const Matrix& t_Matrix) {
	return t_Matrix*t_Complex;
}


Matrix operator/(const Matrix& t_Matrix, const Complex& t_Complex) {
	if (Complex::abs(t_Complex) == 0) throw LTEV2X_Exception("����ģֵΪ0���Ƿ���");
	Matrix res(t_Matrix.row, t_Matrix.col);
	for (int r = 0; r < t_Matrix.row; r++) {
		res[r] = t_Matrix[r] / t_Complex;
	}
	return res;
}
Matrix operator/(const Complex& t_Complex, const Matrix& t_Matrix) {
	Matrix res(t_Matrix.row, t_Matrix.col);
	for (int r = 0; r < t_Matrix.row; r++) {
		res[r] = t_Complex / t_Matrix[r];
	}
	return res;
}


Matrix elementProduct(const Matrix& t_Matrix1, const Matrix& t_Matrix2) {
	if (t_Matrix1.row != t_Matrix2.row ||
		t_Matrix1.col != t_Matrix2.col) throw LTEV2X_Exception("����ά�Ȳ�ƥ��");
	Matrix res(t_Matrix1.row, t_Matrix1.col);
	for (int r = 0; r < t_Matrix1.row; r++) {
		res[r] = elementProduct(t_Matrix1[r], t_Matrix2[r]);
	}
	return res;
}
Matrix elementDivide(const Matrix& t_Matrix1, const Matrix& t_Matrix2) {
	if (t_Matrix1.row != t_Matrix2.row ||
		t_Matrix1.col != t_Matrix2.col) throw LTEV2X_Exception("����ά�Ȳ�ƥ��");
	Matrix res(t_Matrix1.row, t_Matrix1.col);
	for (int r = 0; r < t_Matrix1.row; r++) {
		res[r] = elementDivide(t_Matrix1[r], t_Matrix2[r]);
	}
	return res;
}