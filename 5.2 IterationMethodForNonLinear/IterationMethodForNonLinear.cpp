#include <iostream>
#include <cmath>
#include <exception>
#include <iomanip>
#include <string>

using namespace std;

static const double epsAcc = 0.00000001;
static int simpleIterCount;

#define StudentParameters nStudent, nGroup
#define Borders _firstBorder, _secondBorder
#define SignMax  equationSign, maxBasicValue

//double PutInMain(const double& value, 
//	const int& nStudent, const int& nGroup)
//{
//	return (pow(value, 3) / (2 * nStudent * nGroup))
//		+ ((nStudent + nGroup) / 2);
//}
//
//double InFirstDevariative (const double& value,
//	const int& nStudent, const int& nGroup)
//{
//	return 3 * pow(value, 2) / (2 * nStudent * nGroup);
//}

double BasicFunction (const double& value,
	const int& nStudent, const int& nGroup)
{
	return (pow(value, 3) / (nStudent * nGroup)) +
		2 * value - (nStudent + nGroup);
}

double BasicFunctionDevariative(const double& value,
	const int& nStudent, const int& nGroup)
{
	return 3 * pow(value, 2) / (nStudent * nGroup) + 2;
}

int CheckSign(const double& firstBorder, const double& secondBorder,
	const int& nStudent, const int& nGroup)
{
	return (BasicFunction(secondBorder, StudentParameters) -
		BasicFunction(firstBorder, StudentParameters) > 0) ? -1 : 1;
}

double FindMaxValue(const double& firstBorder, const double& secondBorder,
	const int& nStudent, const int& nGroup)
{
	return (abs(BasicFunctionDevariative(firstBorder, StudentParameters) >
		abs(BasicFunctionDevariative(secondBorder, StudentParameters)))) ?
			abs(BasicFunctionDevariative(firstBorder, StudentParameters)) :
				abs(BasicFunctionDevariative(secondBorder, StudentParameters));
}

double EquivalentFuction(const double& value, const int& nStudent, 
	const int& nGroup, const int& equationSign, const double& maxValue)
{
	return value + equationSign * 
		BasicFunction(value, StudentParameters) / maxValue;
}

double EquivalentDevariative(const double& value, const int& nStudent,
	const int& nGroup, const int& equationSign, const double& maxValue)
{
	return 1 + equationSign * BasicFunctionDevariative(value, StudentParameters) /
		maxValue;
}

//double EquivalentDevariative(const double& value, const int& _firstBorder, const int& _secondBorder,
//	const int& nStudent, const int& nGroup, const int& equationSign)
//{
//	return 1 + equationSign * BasicFunctionDevariative(value, StudentParameters) /
//		FindMaxValue(Borders, StudentParameters);
//}

//double FindMaxValueForEquivalent()
//{
//	
//}

void IterationMethod(const double& _firstBorder, const double& _secondBorder, 
	const int& nStudent, const int& nGroup)
{
	double maxBasicValue = FindMaxValue(Borders, StudentParameters); // FOR BETTER CALC
	int equationSign = CheckSign(Borders, StudentParameters);	  // FOR BETTER CALC

	cout << "\n";

	double maxEqualValue;

	if (abs(EquivalentDevariative(_firstBorder, StudentParameters, SignMax))
		> abs(EquivalentDevariative(_secondBorder, StudentParameters, SignMax)))
	{
		maxEqualValue = abs(EquivalentDevariative(_firstBorder, StudentParameters, SignMax));
	}
	else
	{
		maxEqualValue = abs(EquivalentDevariative(_secondBorder, StudentParameters, SignMax));
	}

	/*try
	{
		if (!(maxEqualValue < 1) || (maxEqualValue == 0))
		{
			throw out_of_range("Q имеет неверное значение, Q = " + to_string(maxEqualValue));
		}
	} catch (exception& ex)
	{
		cout << ex.what() << endl;
		exit(9);
	}*/

	static double validEps;

	if (maxEqualValue == 0)
	{
		validEps = epsAcc;
	}
	else
	{
		validEps = (1 - maxEqualValue) * epsAcc / maxEqualValue;		// IN CASE, WHEN EPS IS KNOWN
		cout << "Q = " << maxEqualValue << endl;
	}

	double _xO = (_firstBorder - _secondBorder) / 2,
		_xN = EquivalentFuction(_xO, StudentParameters, SignMax),
		absDiff = fabs(_xN - _xO);

	while (absDiff >= validEps) 
	{
		_xO = _xN;
		_xN = EquivalentFuction(_xO, StudentParameters, SignMax);
		absDiff = fabs(_xN - _xO); // ** //
		++simpleIterCount;
	}

	cout << "\n";
	double _root = _xN;

	cout << "Корень " << setprecision(7) << fixed << _root << endl << "Количество итераций: "
		<< simpleIterCount << endl;

	cout << "\n";
}

int main()
{
	setlocale(LC_ALL, "rus");

	double _firstBorder, _secondBorder;
	cout << "Нахождение корня методом простых итераций" << endl
		<< "Введите интервал a и b (через пробел): " << endl;
	cin >> _firstBorder >> _secondBorder;

	cout << "Введите номер группы и номер студента через пробел: " << endl;
	int nGroup, nStudent;
	cin >> nGroup >> nStudent;

	IterationMethod(Borders, StudentParameters);
	
	return 0;
	
}