#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
#include <random>

using namespace std;

#define StudentData nGroup, nStudent

uniform_int_distribution<> uid(-10, 10);

// x = -1 , 1, 5
// y =  4, -2, 10

double** ThrowInPower(double** _matrix, const int& node)
{
	for (int i = 0; i < node; ++i)
	{
		for (int j = 0; j < node; ++j)
		{
			int _power = node - j - 1;
			_matrix[i][j] = pow(_matrix[i][j], _power);
		}
	}

	return _matrix;
}

double** CreateMatrix(const int& node, const int& nGroup, const int& nStudent, const int& inputType)
{
	setlocale(LC_ALL, "rus");

	random_device dev;
	mt19937 _randengine(dev());
	
	auto** _matrix = new double* [node];

	for (int i = 0; i < node; ++i)
	{
		_matrix[i] = new double[node];
	}

		if (inputType == 0) // studentParameters
		{
			if (node > 5 || node < 1)
			{
				cout << "Неверное количество точек" << endl;
				exit(10);
			}
			else
			{
				if (node >= 2 && node <= 5) 
				{
					_matrix[0][0] = -2;
					_matrix[0][node] = nStudent;
				}
				if (node >= 2 && node <= 5) 
				{
					_matrix[1][0] = -1;
					_matrix[1][node] = nGroup;
				}
				if (node >= 3 && node <= 5) 
				{
					_matrix[2][0] = 0;
					_matrix[2][node] = -1;
				}
				if (node >= 4 && node <= 5) 
				{
					_matrix[3][0] = 1;
					_matrix[3][node] = nStudent;
				}
				if (node == 5) 
				{
					_matrix[4][0] = 2;
					_matrix[4][node] = nGroup;
				}
				for (int i = 0; i < node; ++i)
				{
					for (int j = 0; j < node; ++j)
					{
						_matrix[i][j] = _matrix[i][0];
					}
				}
				cout << "X: ";
				for (int i = 0; i < node; ++i)
				{
					cout << setw(7) << setprecision(2) << fixed << _matrix[i][0] << ' ';
				}
				cout << endl << "Y: ";
				for (int i = 0; i < node; ++i)
				{
					cout << setw(7) << setprecision(2) << fixed << _matrix[i][node] << ' ';
				}
				cout << endl << endl;
			}
		}
		else if (inputType == 1) // Random
		{
			for (int i = 0; i < node; ++i)
			{
				for (int j = 0; j < node; ++j)
				{
					_matrix[i][j] = uid(_randengine);
				}
				_matrix[i][node] = uid(_randengine);
			}
			cout << "X: ";
			for (int i = 0; i < node; ++i)
			{
				cout << setw(7) << setprecision(2) << fixed << _matrix[i][0] << ' ';
			}
			cout << endl << "Y: ";
			for (int i = 0; i < node; ++i)
			{
				cout << setw(7) << setprecision(2) << fixed << _matrix[i][node] << ' ';
			}
			cout << endl << endl;
		}

		return ThrowInPower(_matrix, node);

}

//double* DecisionMatrix(const int& node, const int& nStudent, const int& nGroup)
//{
//	setlocale(LC_ALL, "rus");
//
//	random_device dev;
//	mt19937 _randengine(dev());
//	
//	auto* _decMatrix = new double[node];
//
//	_decMatrix[0] = 4;
//	_decMatrix[1] = -2;
//	_decMatrix[2] = 10;
//
//	return _decMatrix;
//}

void PrintMatrix (double** _matrix, const int& node)
{
	for (size_t i = 0; i < node; ++i)
	{
		for (size_t j = 0 ; j < node; ++j)
		{
			cout << setw(4 + node - j) << _matrix[i][j] << " * x ^ " + to_string(node-j) << ' ';
		}
		cout << setw(node + 1) << _matrix[i][node] << ' ' << endl;
	}
	cout << "\n";
}

double* FindRoots(double** _matrix, const int& node)
{
	setlocale(LC_ALL, "rus");

	double* roots = new double[node], summ = 0;
	for (int iter = 0; iter < node; iter++)
	{
		for (int i = iter; i < node; i++)
		{
			double temp = _matrix[i][iter];

			if (temp != 0)
			{
				for (int j = iter; j < node; j++) _matrix[i][j] = _matrix[i][j] / temp;
				_matrix[i][node] = _matrix[i][node] / temp;
			}
		}
		for (int i = iter + 1; i < node; i++)
		{
			if (_matrix[i][iter] != 0)
			{
				for (int j = iter; j < node; j++)
				{
					_matrix[i][j] = _matrix[i][j] - _matrix[iter][j];
					summ = summ + abs(_matrix[i][j]);
				}

				_matrix[i][node] = _matrix[i][node] - _matrix[iter][node];

				/*if (summ == 0)
				{
					cout << "exception";
					exit(9);
				}*/
			}
		}
	}

	summ = 0;

	for (int iter = node - 1; iter >= 0; iter--) {
		roots[iter] = _matrix[iter][node] - summ;
		if (iter > 0) {
			for (int i = node - 1; i >= iter; i--) summ = _matrix[iter - 1][i] * roots[i];
		}
	}
	return roots;
}

//void InterpolationMethod(double** _matrix, double* _roots, const int& node)
//{
//	for (int i = 0; i < node; ++i)
//	{
//		for (int j = 0; j < node; ++j)
//		{
//			_matrix[i][j] = _roots[i];
//		}
//	}
//
//	PrintMatrix(_matrix, _roots, node);
//}

int main()
{
	setlocale(LC_ALL, "rus");

	cout << "Составление интерполяционного многочлена по n - узлам." << endl;
	cout << "Введите количество узлов, номер группы и номер студента, тип ввода(через пробел)" << endl;

	int node, nGroup, nStudent, inputType; // nStudent & nGroup defined as StudentData;
	cin >> node >> nGroup >> nStudent >> inputType;
	cout << "\n";

	if (inputType != 0 && inputType != 1)
	{
		cout << "Неверный тип ввода" << endl;
		exit(10);
	}

	auto** _matrix = CreateMatrix(node, StudentData, inputType);

	PrintMatrix(_matrix, node);

	auto* roots = FindRoots(_matrix, node);

	cout << "Корни системы: ";
	for (int i = 0; i < node; ++i)
	{
		cout << setw(5) << roots[i] << ' ';
	}

	cout << endl << "Полученный многочлен: P(x) = ";

	for (int i = 0; i < node; ++i)																									// TO REWORK COUT
	{																																
		if (node - i != 1)																											
		{																															
			if(roots[i] > 1)																										
			{																														
				cout << setw(node - i) << setprecision(3) << fixed << roots[i] << " * x ^ " + to_string(node - i) << ' ';			
			}																														
			else if(roots[i] < 1)																									
			{																														
				cout << setw(node - i) << setprecision(3) << fixed << roots[i] << " * x ^ " + to_string(node - i) << ' ';			
			}																														
		}																															
		else																														
		{																															
			if(roots[i] > 1)																										
			{																														
				cout << setw(node - i) << setprecision(3) << fixed << roots[i] << " * x";											
			}																														
			else if (roots[i] < 1)																									
			{																														
				cout << setw(node - i) << setprecision(3) << fixed << roots[i] << " * x";											
			}																														
		}																															
	}																																// TO REWORK COUT
																																	
	cout << endl;
	
	return 0;
}