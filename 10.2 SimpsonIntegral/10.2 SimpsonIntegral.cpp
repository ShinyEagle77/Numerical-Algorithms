#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
#include <vector>
#include <exception>

using namespace std;

#define StudentData nGroup, nStudent
#define IntervalData a, b

static constexpr double epsOrd = 4;
static double zOne, zSecond, hOne, hSecond;

//11, 21, 41
//0.1, 0.05, 0.025

double GivenFunction(const double& value, const int& nGroup, const int& nStudent)
{
	return pow(sin(value), 2 * nGroup) * pow(cos(value), 2 * nStudent);
}

//double GivenFunction(const double& value, const int& nGroup, const int& nStudent)
//{
//	return pow(value, nGroup) * sin(value * nStudent);
//}

void PrintTValues(const vector<double>& resultT, const int& itCount)
{
	setlocale(LC_ALL, "rus");

	double maxT = *resultT.begin();
	for (int k = 0; k < itCount; ++k)
	{
		cout << setw(10 + k) << "T" + to_string(k + 1);
	}
	cout << endl;

	for (const auto& x : resultT)
	{
		cout << setw(12 + x * 0.5) << fixed << setprecision(4) << x;
		if (x > maxT)
		{
			maxT = x;
		}
	}
	cout << endl << endl;

	cout << "Конечное значение: " + to_string(maxT) << endl;

}

void SimpsonMethod(const double& a, const double& b, const int& itCount, const int& nGroup, const int& nStudent)
{

	vector<double> resultT;
	int node = 0;
	for (int i = 0; i < itCount; ++i)
	{

		node += 10 + i * 1;

		if(node % 2 != 0)
		{
			++node;
			cout << "Размер был увеличен, так как метод Симпсона работает только с чётным количеством узлов, текущее значение: " + to_string(node) << endl;
		}

		double _h = (b - a) / node;

		double xOld = a,
			yOld = GivenFunction(xOld, StudentData);

		cout << "---------------- Текущий шаг итерации: " + to_string(i + 1) << " ----------------" << " Количество узлов: " + to_string(node) << endl;
		cout << setw(10) << fixed << setprecision(4) << 'X' << setw(17) << fixed << setprecision(6) << 'Y' << endl;
		cout << setw(12) << fixed << setprecision(4) << xOld << setw(18) << fixed << setprecision(6) << yOld << endl;

		vector<double> resultOdd, resultEven;
		resultOdd.push_back(yOld);
		for (int k = 0; k < node; ++k)
		{
			double xNew = xOld + _h,
				yNew = GivenFunction(xNew, StudentData);

			if (k % 2 == 0)
			{
				resultEven.push_back(yNew);
			} else
			{
				resultOdd.push_back(yNew);
			}

			xOld = xNew;

			if (itCount == 2 && i == 0)
			{
				zOne = yNew;
				hOne = _h;
			}
			else if (itCount == 2 && i == 1)
			{
				zSecond = yNew;
				hSecond = _h;
			}

			cout << setw(12) << fixed << setprecision(4) << xNew << setw(18) << fixed << setprecision(6) << yNew << endl;
		}

		double midValueOdd = 0, midValueEven = 0;

		for (int k = 1; k < resultOdd.size() - 1; ++k)
		{
			midValueOdd += resultOdd[k];
		}

		for (int k = 1; k < resultEven.size() - 1; ++k)
		{
			midValueEven += resultOdd[k];
		}

		double valueT = _h / 3 * (*resultOdd.begin() + 4 * midValueOdd + 2 * midValueEven + *prev(resultEven.end()));

		resultT.push_back(valueT);


		cout << endl;
	}

	PrintTValues(resultT, itCount);

	cout << "Количество совершённых итераций: " + to_string(itCount) << ", где уточнённый корень равен: "
	<< setprecision(6) << fixed << zOne + (zOne - zSecond) / (pow(hSecond / hOne, epsOrd) - 1) << endl;

}

int main()
{
	setlocale(LC_ALL, "rus");

	cout << "Вычисление значения определённого интеграла методом Симпсона. " << endl;
	cout << "Введите номер группы, номер студента и количество итераций." << endl;

	int nGroup, nStudent, itCount;
	cin >> nGroup >> nStudent >> itCount;

	cout << "Введите значения интервала для интеграла." << endl;
	double a, b;
	string tempB;
	cin >> a >> tempB;

	if (tempB == "pi/2")
	{
		b = M_PI / 2;
	}
	else
	{
		try
		{
			b = stod(tempB);
		} catch (exception& ex)
		{
			cout << ex.what() << endl << "Введите значение b" << endl;
			cin >> b;
		}
		
	}

	if (a > b)
	{
		double temp = a;
		b = a;
		a = temp;
	}

	SimpsonMethod(IntervalData, itCount, StudentData);

	return 0;
}