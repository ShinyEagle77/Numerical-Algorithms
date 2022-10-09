#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>

using namespace std;

#define StudentData nGroup, nStudent

static constexpr int epsOrd = 2;

double GivenFunction(const double& a, const double& x, const double& y)
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

void EilerMethod(const double& a, const double& b, const double& _firstBorder, const double& _secondBorder, const int& itCount)
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
	
	int _node = 0;
	for (int i = 0; i < itCount; ++i)
	{
		_node += 5;
		double _h = (_secondBorder - _firstBorder) / _node,
			xOld = _firstBorder,
			yOld = 0,
			zOld = a + b;

		double xNew = xOld + _h,
			yNew = yOld + _h * zOld;
			
		cout << "---------------- Текущий шаг итерации: " + to_string(i + 1) << " ----------------" << " Количество узлов: " + to_string(_node) << endl;
		cout << setw(6) << 'i' << setw(10) << fixed << setprecision(4) << 'X' << setw(17) << fixed << setprecision(6) << 'Y' << endl;
		cout << setw(6) << '0' << setw(12) << fixed << setprecision(4) << xOld << setw(18) << fixed << setprecision(6) << yOld << endl;

		while (static_cast<int>(xNew) != static_cast<int>(_secondBorder))
		{
			for (int k = 0; k < _node; ++k)
			{
				xNew = xOld + _h;
				yNew = yOld + _h * zOld;
				double zNew = zOld + _h * GivenFunction(a, xOld, yOld);

				xOld = xNew;
				yOld = yNew;
				zOld = zNew;

				cout << setw(12) << fixed << setprecision(4) << xNew << ',' <<setw(18) << fixed << setprecision(6) << yNew << endl;

			}
		}
		for (int j = 1; j < itCount; j++) 
		{
			_mD1[i][j] = pow(_h, epsOrd  + j - 1);
			_mD2[i][j] = _mD1[i][j];
		}
		_mD1[i][0] = yNew;
		_mD2[i][0] = 1;
		cout << endl;
	}
	cout << "Количество совершённых итераций: " + to_string(itCount) << ", где уточнённый корень равен: " << setprecision(6) << fixed << FindDet(_mD1, itCount) / FindDet(_mD2, itCount) << endl;
}

int main()
{
	setlocale(LC_ALL, "rus");

	cout << "Решение задачи Коши методом Эйлера. Введите номер группы и номер студента, и количество итераций" << endl;
	int nGroup, nStudent, itCount;
	cin >> nGroup >> nStudent >> itCount;

	double a = sqrt(nStudent),
		b = nGroup,
		_firstBorder = 0,
		_secondBorder = M_PI;

	EilerMethod(a, b, _firstBorder, _secondBorder, itCount);

	return 0;

}