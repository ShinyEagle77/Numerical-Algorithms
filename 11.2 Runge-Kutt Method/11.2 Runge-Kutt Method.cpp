#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>

using namespace std;

#define StudentData nGroup, nStudent

static constexpr int epsOrd = 4;
static double zOne, zSecond, hOne, hSecond;

double GivenFunction(const double& a, const double& x, const double& y, const double& z)
{
	return a * x - y;
}

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

void RungeMethod(const double& a, const double& b, const double& _firstBorder, const double& _secondBorder, const int& itCount)
{
	
	int _node = 0;
	for (int i = 0; i < itCount; ++i)
	{
		_node += 5;
		double _h = (_secondBorder - _firstBorder) / _node,
			xOld = _firstBorder,
			yOld = 0,
			zOld = a + b;

		double xNew = xOld + _h;

		cout << "---------------- Текущий шаг итерации: " + to_string(i + 1) << " ----------------" << " Количество узлов: " + to_string(_node) << endl;
		cout << setw(6) << 'i' << setw(10) << 'X' << setw(17) << 'Y' << endl;
		cout << setw(6) << '0' << setw(12) << fixed << setprecision(4) << xOld << setw(18) << fixed << setprecision(6) << yOld << endl;

		while (static_cast<int>(xNew) != static_cast<int>(_secondBorder))
		{
			for (int k = 0; k < _node; ++k)
			{
				double K1_Y = zOld,
					K1_Z = GivenFunction(a, xOld, yOld, zOld),

					K2_Y = zOld + _h / 2 * K1_Z,
					K2_Z = GivenFunction(a, xOld + _h / 2, yOld + _h / 2 * K1_Y, zOld + _h / 2 * K1_Z),

					K3_Y = zOld + _h / 2 * K2_Z,
					K3_Z = GivenFunction(a, xOld + _h / 2, yOld + _h / 2 * K2_Y, zOld + _h / 2 * K2_Z),

					K4_Y = zOld + _h * K3_Z,
					K4_Z = GivenFunction(a, xOld + _h, yOld + _h * K3_Y, zOld + _h * K3_Z);

				xNew = xOld + _h;

				double yNew = yOld + _h / 6 * (K1_Y + 2 * K2_Y + 2 * K3_Y + K4_Y),
					zNew = zOld + _h / 6 * (K1_Z + 2 * K2_Z + 2 * K3_Z + K4_Z);

				xOld = xNew;
				yOld = yNew;
				zOld = zNew;

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

				cout << setw(12) << fixed << setprecision(4) << xNew << ',' << setw(18) << fixed << setprecision(6) << yNew << endl;

			}
		}
		
		cout << endl;
	}
	cout << "Количество совершённых итераций: " + to_string(itCount) << ", где уточнённый корень равен: " << setprecision(6) << fixed << zOne + (zOne - zSecond)/(pow(hSecond/hOne, epsOrd) - 1) << endl;
}

int main()
{
	setlocale(LC_ALL, "rus");

	cout << "Решение задачи Коши методом Рунге-Кутты. Введите номер группы и номер студента, и количество итераций" << endl;
	int nGroup, nStudent, itCount;
	cin >> nGroup >> nStudent >> itCount;

	double a = sqrt(nStudent),
		b = nGroup,
		_firstBorder = 0,
		_secondBorder = M_PI;

	RungeMethod(a, b, _firstBorder, _secondBorder, itCount);

	return 0;

}