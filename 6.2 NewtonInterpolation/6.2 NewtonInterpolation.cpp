#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include <exception>
#include <map>
#include <random>

using namespace std;

#define StudentData nGroup, nStudent
#define MatrixData _matrix, node

static constexpr int systemOfEquationSize = 3;

uniform_real_distribution<> uid(-1, 1);


void PrintPoints(double** _matrix, const int& node)
{
	cout << "X: ";
	for (int i = 0; i < node; ++i)
	{
		cout << setw(7) << setprecision(2) << fixed << _matrix[0][i] << ' ';
	}
	cout << endl << "Y: ";
	for (int i = 0; i < node; ++i)
	{
		cout << setw(7) << setprecision(2) << fixed << _matrix[1][i] << ' ';
	}
	cout << endl << endl;
}

double** CreateMatrix(const int& node, const int& nGroup, const int& nStudent, const int& inputType)
{

	setlocale(LC_ALL, "rus");

	random_device dev;
	mt19937 _randengine(dev());

	auto** _matrix = new double* [2];
	_matrix[0] = new double[node];
	_matrix[1] = new double[node];

	if (inputType == 0) // studentParameters
	{
		if (node > 5 || node < 1)
		{
			cout << "Неверное количество точек" << endl;
			exit(10);
		}
		else
		{
			auto a = static_cast<double>(nStudent) + static_cast<double>(nGroup);
			if (node >= 3 && node <= 5)
			{
				_matrix[0][0] = -2.5 * a;
				_matrix[1][0] = 84 - static_cast<double>(nGroup);
			}
			if (node >= 3 && node <= 5)
			{
				_matrix[0][1] = -1.5 * a;
				_matrix[1][1] = 73 - static_cast<double>(nGroup) * 2;
			}
			if (node >= 3 && node <= 5)
			{
				_matrix[0][2] = -0.5 * a;
				_matrix[1][2] = 63 - static_cast<double>(nGroup) * 3;
			}
			if (node >= 4 && node <= 5)
			{
				_matrix[0][3] = 0.5 * a;
				_matrix[1][3] = 55 - static_cast<double>(nGroup) * 4;
			}
			if (node == 5)
			{
				_matrix[0][4] = 1.5 * a;
				_matrix[1][4] = 47 - static_cast<double>(nStudent) * 5;
			}
		}
	}
	else if (inputType == 1) // Random
	{
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < node; ++j)
			{
				_matrix[i][j] = uid(_randengine);
			}
			_matrix[i][node] = uid(_randengine);
		}
	}
	PrintPoints(MatrixData);
	return _matrix;

}

double NewtonMethod(double** points, const double& x, const double& node)
{
	double summ = points[1][0];
	for (int i = 1; i < node; ++i) 
	{
		double F = 0;
		for (int j = 0; j <= i; ++j) 
		{
			double den = 1;
			for (int k = 0; k <= i; ++k)
				if (k != j)
					den *= (points[0][j] - points[0][k]);
			F += points[1][j] / den;
		}
		for (int k = 0; k < i; ++k)
			F *= x - points[0][k];
		summ += F;
	}
	return summ;
}

int main()
{
	try
	{
		setlocale(LC_ALL, "rus");

		cout << "Построение многочлена методом Ньютона" << endl;
		cout << "Введите размер таблицы, номер группы и номер студента, тип ввода: 0 - параметры студента, 1 - рандом (через пробел)" << endl;

		int node, nGroup, nStudent, inputType; // nStudent & nGroup defined as StudentData;
		cin >> node >> nGroup >> nStudent >> inputType;
		cout << "\n";

		cout << "Введите интервал a, b и шаг" << endl;
		double a, b, step;
		cin >> a >> b >> step;

		if ((inputType != 0 && inputType != 1) || (node < 3) && (node > 5))
		{
			throw out_of_range("Неверный тип вводимых данных");
		}

		auto** _matrix = CreateMatrix(node, StudentData, inputType);

		vector<double> first, second;

		for (int i = static_cast<int>(a); i <= static_cast<int>(b); i += step)
		{
			first.push_back(i);
			second.push_back(NewtonMethod(_matrix, i, node));
		}

		cout << "Полученные значения: " << endl;

		map< double, double > result;
		for (int i = 0; i < first.size(); ++i)
		{
			result[first[i]] = second[i];
		}

		for (const auto& x : result)
		{
			cout << x.first << ',' << x.second << endl;
		}

	}
	catch (exception& ex)
	{
		cout << ex.what() << endl;
	}
}