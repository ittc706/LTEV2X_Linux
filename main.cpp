#include<iostream>
#include<fstream>
#include<iomanip>
#include"Enumeration.h"
#include"System.h"
#include"Matrix.h"
#include"Function.h"

using namespace std;

int main() {
	System* _system = new System();;
	_system->process();

	Delete::safeDelete(_system);
	system("pause");
	return 1;
}


//int main() {
//	try {
//		Matrix m{
//			{{1,2},{2,2},{3,3}},
//			{{4,4},{5,3},{6,6}},
//			{{7,9},{8,8},{9,3}},
//		};
//
//		RowVector rv{
//			{ 1,2 },{ 2,2 },{ 3,3 }
//		};
//
//
//		cout << "\n\n-------------RowVector(������ֵ)-------------" << endl;
//		cout << "=====RowVector(���Ը�ֵ)=====" << endl;
//		RowVector rvCopyAssign;
//		rvCopyAssign = rv;
//		rv.print(); cout << endl;
//		rv[1] = Complex{ 0,0 };
//		rv.print(); cout << endl;
//		rvCopyAssign.print(); cout << endl;
//		cout << "=====RowVector(�Ը�ֵ)=====" << endl;
//		rvCopyAssign = rvCopyAssign;
//		rvCopyAssign.print(); cout << endl;
//
//		cout << "\n\n-------------RowVector(�ƶ���ֵ)-------------" << endl;
//		rv=RowVector{
//			{ 1,2 },{ 2,2 },{ 3,3 }
//		};
//		cout << "=====RowVector(���Ը�ֵ)=====" << endl;
//		RowVector rvMoveAssign;
//		rvMoveAssign = std::move(rv);
//		rvMoveAssign.print(); cout << endl;
//		cout << "=====RowVector(�Ը�ֵ)=====" << endl;
//		rvMoveAssign = std::move(rvMoveAssign);
//		rvMoveAssign.print(); cout << endl;
//
//
//		//���ȼ��Matrix������ֵǰ���Ƿ��й�ϵ
//		cout << "\n\n-------------Matrix(������ֵ)-------------" << endl;
//		cout << "=====Matrix(���Ը�ֵ)=====" << endl;
//		m = Matrix{
//			{ { 1,2 },{ 2,2 },{ 3,3 } },
//			{ { 4,4 },{ 5,3 },{ 6,6 } },
//			{ { 7,9 },{ 8,8 },{ 9,3 } },
//		};
//		Matrix mCopyAssign;
//		mCopyAssign = m;
//		m.print(); cout << endl;
//		m[1][1] = Complex{ 0,0 };
//		m.print(); cout << endl;
//		mCopyAssign.print(); cout << endl;
//		cout << "=====Matrix(�Ը�ֵ)=====" << endl;
//		mCopyAssign = mCopyAssign;
//		mCopyAssign.print(); cout << endl;
//
//		cout << "\n\n-------------Matrix(�ƶ���ֵ)-------------" << endl;
//		cout << "=====Matrix(���Ը�ֵ)=====" << endl;
//		m = Matrix{
//			{ { 1,2 },{ 2,2 },{ 3,3 } },
//			{ { 4,4 },{ 5,3 },{ 6,6 } },
//			{ { 7,9 },{ 8,8 },{ 9,3 } },
//		};
//		Matrix mMoveAssign;
//		mMoveAssign = std::move(m);
//		mMoveAssign.print(); cout << endl;
//		cout << "=====Matrix(�Ը�ֵ)=====" << endl;
//		mMoveAssign = std::move(mMoveAssign);
//		mMoveAssign.print(); cout << endl;
//
//
//
//		cout << "\n\n-------------Matrix(��������)-------------" << endl;
//		m = Matrix{
//			{ { 1,2 },{ 2,2 },{ 3,3 } },
//			{ { 4,4 },{ 5,3 },{ 6,6 } },
//			{ { 7,9 },{ 8,8 },{ 9,3 } },
//		};
//		Matrix mCopyConstruct(m);
//		mCopyConstruct.print(); cout << endl;
//		m[1][1] = Complex(-100, -100);
//		mCopyConstruct.print(); cout << endl;
//
//	}
//	catch (Exp e) {
//		cout << e.what() << endl;
//	}
//
//	system("pause");
//	return 1;
//}



//int main() {
//	WT_B* p = new WT_B(nullptr);
//	p->testCloest();
//	system("pause");
//	return 1;
//}



//int main() {
//	int testMain1();
//	testMain1();
//	system("pause");
//	return 1;
//}


//int main() {
//	WT_B *w = new WT_B(Configure(), nullptr, nullptr);
//	w->testSINR();
//	system("pause");
//	return 1;
//}