#include<random>
#include<thread>
#include<iomanip>
#include"WT_B.h"

using namespace std;

void testThread(int count) {
	default_random_engine e;
	uniform_real_distribution<double> u(1, 3);
	for (int iter1 = 0; iter1 <= count; iter1++) {

		for (int iter2 = 0; iter2 < 30; iter2++) {
			Matrix m_H{
				{ Complex(u(e),u(e)) },
				{ Complex(u(e),u(e)) },
			};

			/* ���濪ʼ����W */

			Matrix HHermit(1, 2);
			HHermit = m_H.hermitian();//���ŵ������hermitian

			Matrix inverseExpLeft = 2 * m_H * HHermit;//������ʽ�������

			//���������
			Matrix Interference1(2, 2);
			Matrix m_HInterference{
				{ Complex(u(e),u(e)) },
				{ Complex(u(e),u(e)) },
			};
			for (int i = 0; i < 5; i++) {
				Interference1 = Interference1 + 3 * m_HInterference * m_HInterference.hermitian();
			}

			//������ʽ�ұ�����
			Matrix inverseExpRight = 2 * Matrix::eye(2) + Interference1;//sigma�ϴ�����

			Matrix W = (inverseExpLeft + inverseExpRight).inverse(true)*sqrt(2)*m_H;//Ȩ�ؾ���


			/* ���濪ʼ����D */
			//�ȼ������
			Matrix WHer = W.hermitian();
			Matrix D = sqrt(2)*WHer*m_H;
			Matrix DHer = D.hermitian();
			Matrix molecular = D*DHer;//SINR����ķ��ӣ�1*1�ľ���


			//Ȼ������ĸ
			Matrix denominatorLeft = WHer*W * 2;//SINR����ķ�ĸ�еĵ�һ��
			Matrix Iself = WHer*m_H*sqrt(2) - D;
			Matrix IselfHer = Iself.hermitian();
			Matrix denominatorMiddle = Iself*IselfHer; //SINR����ķ�ĸ�еĵڶ���

			/*���¼��㹫ʽ�еĸ�����,����ʽ�еĵ�����*/
			Matrix denominatorRight(1, 1);
			for (int i = 0; i < 5; i++) {
				denominatorRight = denominatorRight + 2 * WHer*m_HInterference * m_HInterference.hermitian()*W;
			}

			Matrix denominator = denominatorLeft + denominatorMiddle + denominatorRight;//SINR����ķ�ĸ

		}

	}
}



int testMain1() {
	long double start = clock();
	testThread(2500);
	long double end = clock();

	cout.setf(ios::fixed);
	cout << "\nRunning Time :" << setprecision(1) << (end - start) / 1000.0L << " s\n" << endl;
	cout.unsetf(ios::fixed);

	start = clock();
	int threadNum = 4;
	thread* threads = new thread[threadNum];
	for (int i = 0; i < threadNum; i++) {
		threads[i] = thread(testThread, 2500 / threadNum);
	}

	for (int i = 0; i < threadNum; i++) {
		threads[i].join();
	}
	end = clock();

	cout.setf(ios::fixed);
	cout << "\nRunning Time :" << setprecision(1) << (end - start) / 1000.0L << " s\n" << endl;
	cout.unsetf(ios::fixed);

	system("pause");
	return 1;
}