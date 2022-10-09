#include <iostream>
#include <cmath>
#include <exception>
#include <iomanip>

using namespace std;

static int newtonIterationCount = 0;
static const double epsAcc = 0.00000001;


double PutInMain (const double& value, 
	const int& nStudent, const int& nGroup)
{
	return pow(value, 3) / 10 + value / nStudent // x^3 / 10 + x / Nc + Nc * Ng = 0;
	+ static_cast<uint8_t>(nStudent * nGroup);
}

double InFirstDerivative (const double& value,
	const int& nStudent)
{
	return 3 * pow(value, 2) / 10 + 1 / nStudent; // 3 * x^2 / 10 + 1 / Nc;
}

double InSecondDerivative (const double& value)
{
	return 3 * value / 5;						  // 3 * x / 5;
}

bool CheckInterval (const double& value,
	const int& nStudent, const int& nGroup)
{
		if (PutInMain(value, nStudent, nGroup) * InSecondDerivative(value) > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
}

void NewtonMethod (const double& value, 
	const int& nStudent, const int& nGroup)
{
	double a_ = value,
		_firstpoint = a_;


	while ((abs(PutInMain(_firstpoint, nStudent, nGroup)) > epsAcc))
	{
		a_ = _firstpoint - (PutInMain(_firstpoint, nStudent, nGroup)
			/ InFirstDerivative(_firstpoint, nStudent));
		_firstpoint = a_;
		a_ = PutInMain(_firstpoint, nStudent, nGroup);
		++newtonIterationCount;
	}

	double _root = _firstpoint;

	cout << "Корень уравнения: " << setprecision(9) << fixed
			<< _root << endl << "Количество итераций: "
				<< newtonIterationCount << endl;
}

int main ()
{
	setlocale(LC_ALL, "rus");
	
	double a, b;
	cout << "Нахождение корня методом Ньютона. " << endl
		<< "Введите интервал a и b (через пробел): " << endl;
	cin >> a >> b;

	try
	{

		cout << "Введите номер группы и номер студента через пробел: " << endl;
		int nGroup, nStudent;
		cin >> nGroup >> nStudent;

		double _a = PutInMain(a, nStudent, nGroup),
			_b = PutInMain(b, nStudent, nGroup);

		/*if (!(_a > 0 && _b < 0) || (_a < 0 && _b > 0))
		{
			throw out_of_range("Значения функции на границах интервала имеют одинаковые знаки.");
		}*/

		cout << "\n";
		cout << "Значения функции в точках интервала: " << endl << "f(" << a << ") = "
			<< _a << endl << "f(" << b << ") = " << _b << endl << "\n";

		if (CheckInterval(a, nStudent, nGroup))
		{
			NewtonMethod(a, nStudent, nGroup);
			return 0;
		}
		else if (CheckInterval(b, nStudent, nGroup))
		{
			NewtonMethod(b, nStudent, nGroup);
			return 0;
		}
		else
		{
			throw out_of_range("Для обоих интервалов f() * f''(a) < 0, условие не выполняется");
		}
	} catch (exception& ex)
	{
		cout << ex.what() << endl;
	}

	return 0;
		
}