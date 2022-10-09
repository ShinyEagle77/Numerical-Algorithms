#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

#define StudentData nGroup, nStudent
#define IntervalData a, b

static constexpr double epsOrd = 4;

//11, 21, 41
//0.1, 0.05, 0.025

double GivenFunction(const double& value, const int& nGroup, const int& nStudent)
{
	return pow(value, nGroup) * sin(value * nStudent);
}

//double GivenFunction(const double& value, const int& nGroup, const int& nStudent)
//{
//	return pow(sin(value), 2 * nGroup) * pow(cos(value), 2 * nStudent);
//}

double** FindMinor(double** matrix, const int& n, const int& i, const int& j)
{
	int index_i = 0;
	auto** minor = new double* [n];
	for (int t = 0; t < n; t++)
		minor[t] = new double[n];
	for (int step_i = 0; step_i < n; step_i++) {
		if (step_i == i) index_i = 1;
		int index_j = 0;
		for (int step_j = 0; step_j < n; step_j++) {
			if (step_j == j) index_j = 1;
			minor[step_i][step_j] = matrix[step_i + index_i][step_j + index_j];
		}
	}
	return(minor);
}

double FindDet(double** matrix, int n)
{
	double det = 0;
	int k = n - 1,
		i = 0;
	if (n == 1)
	{
		det = matrix[0][0];
		return det;
	}
	if (n == 2)
	{
		det = (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
		return det;
	}
	if (n > 2)
	{
		for (int j = 0; j < n; j++) det = det + pow(-1, j) * matrix[0][j] * FindDet(FindMinor(matrix, k, i, j), k);
		return det;
	}
	return det;
}

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

void TrapeziMethod(const double& a, const double& b, const int& itCount, const int& nGroup, const int& nStudent)
{
	auto** _mD1 = new double* [itCount];
	for (int i = 0; i < itCount; ++i)
	{
		_mD1[i] = new double[itCount];
	}

	auto** _mD2 = new double* [itCount];
	for (int i = 0; i < itCount; ++i)
	{
		_mD2[i] = new double[itCount];
	}

	vector<double> resultT;
	int node = 0;
	for (int i = 0; i < itCount; ++i)
	{

		node += 11 + i * 1;
		double _h = (b - a) / node,
			xOld = a,
			yOld = GivenFunction(xOld, StudentData);

		for (int j = 1; j < itCount; j++) 
		{
			_mD1[i][j] = pow(_h, epsOrd + j - 1);
			_mD2[i][j] = _mD1[i][j];
		}

		cout << "---------------- Текущий шаг итерации: " + to_string(i + 1) << " ----------------" << " Количество узлов: " + to_string(node) << endl;
		cout << setw(10) << fixed << setprecision(4) << 'X' << setw(17) << fixed << setprecision(6) << 'Y' << endl;
		cout << setw(12) << fixed << setprecision(4) << xOld << setw(18) << fixed << setprecision(6) << yOld << endl;

		vector<double> resultY;
		resultY.push_back(yOld);
		for (int k = 0; k < node; ++k)
		{
			double xNew = xOld + _h,
				yNew = GivenFunction(xNew, StudentData);
			resultY.push_back(yNew);

			xOld = xNew;
			cout << setw(12) << fixed << setprecision(4) << xNew << setw(18) << fixed << setprecision(6) << yNew << endl;
		}

		double midValue = 0;

		for (int k = 1; k < resultY.size() - 1; ++k)
		{
			midValue += resultY[k];
		}

		double valueT = _h / 2 * (*resultY.begin() + 2 * midValue + *prev(resultY.end()));

		resultT.push_back(valueT);

		_mD1[i][0] = valueT;
		_mD2[i][0] = 1;
		
		cout << endl;
	}

	PrintTValues(resultT, itCount);
	cout << "Уточнённое значение методом Рунге: " << fixed << setprecision(6) << FindDet(_mD1, itCount) / FindDet(_mD2, itCount) << endl;
}

int main()
{
	setlocale(LC_ALL, "rus");

	cout << "Вычисление значения определённого интеграла методом трапеций. " << endl;
	cout << "Введите номер группы, номер студента и количество итераций." << endl;

	int nGroup, nStudent, itCount;
	cin >> nGroup >> nStudent >> itCount;

	cout << "Введите значения интервала для интеграла." << endl;
	double a, b;
	cin >> a >> b;

	if (a > b)
	{
		double temp = a;
		b = a;
		a = temp;
	}

	TrapeziMethod(IntervalData, itCount, StudentData);

	return 0;
}