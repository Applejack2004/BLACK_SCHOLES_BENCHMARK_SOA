#pragma once
#include <cmath>
#include <random>
#pragma once
#include <cmath>
#include <random>
#include <vector>
#include <omp.h>
#include <iostream>
#include <riscv-vector.h>
#include <fstream>
#include <string>
class Option
{
private:
	int N;
	float* T;//�����
	float* S0;//��������� ���� �����
	float* K;// ���� ���������� ��� ������
	float* C;//���� �������
public:

	const float Volatility = 0.2f;
	const float Interest_rest = 0.05f;
	Option()
	{
		N = 10;
		T = new float[N];
		K = new float[N];
		C = new float[N];
		S0 = new float[N];
#pragma omp parallel 
		{

			for (int i = 0; i < N; i++)
			{
				T[i] = 0.0f;
				K[i] = 0.0f;
				C[i] = 0.0f;
				S0[i] = 0.0f;
			}
		}

	}
	~Option()
	{
		delete[] T;
		delete[] K;
		delete[] C;
		delete[] S0;
	}
	Option(int _N)
	{
		N = _N;
		T = new float[N];
		K = new float[N];
		C = new float[N];
		S0 = new float[N];
#pragma omp parallel
		{

			for (int i = 0; i < N; i++)
			{
				T[i] = 0.0f;
				K[i] = 0.0f;
				C[i] = 0.0f;
				S0[i] = 0.0f;
			}
		}
	}
	void random_datas()
	{

		//std::default_random_engine rd(0);//��������� ��������� �����
		//std::uniform_real_distribution<float> dist1(10.0f, 100.0f);
		//std::uniform_real_distribution<float> dist2(5.0f, 30.0f);
//#pragma omp parallel
		{
			std::default_random_engine rd(0);//��������� ��������� �����
			std::uniform_real_distribution<float> dist1(10.0f, 100.0f);
			std::uniform_real_distribution<float> dist2(5.0f, 30.0f);
			//#pragma omp for simd
			for (int i = 0; i < N; i++)
			{

				S0[i] = dist1(rd);
				K[i] = S0[i] + dist2(rd);
				T[i] = dist2(rd);


			}
		}
	}
	void Get_option_price();
	float Get_out_price()
	{
		float price = 0.0f;
		//#pragma omp parallel 
		{
			//#pragma omp for simd reduction(+:price)//������ ������ �� �����, ������ ����� �������� �� ����� ������ price

			for (int i = 0; i < N; i++)
			{
				/*  #pragma omp atomic*/
				price += C[i];
			}
		}

		return price;



	}
	void print()
	{
		for (int i = 0; i < N; i++)
		{
			std::cout << C[i] << ' ' << T[i] << ' ' << S0[i] << ' ' << K[i] << std::endl;
		}

	}
	float Get_S(int i)
	{
		return S0[i];
	}
	float Get_K(int i)
	{
		return K[i];
	}
	float Get_T(int i)
	{
		return T[i];
	}
	void my_datas()
	{
		for (int i = 0; i < N; i++)
		{

			S0[i] = 100.0f;
			K[i] = 100.0f;
			T[i] = 3.0f;


		}

	}

	float compensated_sum() {
		float sum = 0.0;
		float c = 0.0; // ��������������� ��������

		for (int i = 0; i < N; i++) {
			float y = C[i] - c;  // ��������� �������� �����
			float t = sum + y;          // ��������� �����
			c = (t - sum) - y;           // ��������� ���������������� ��������
			sum = t;                     // ���������� �����
		}

		return sum;
	}


};



