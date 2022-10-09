#include <iostream>
#include <ctime>
#include <random>

using namespace std;

double myFunction(const double& x)
//Function to integrate
{
	return x * x;
}

double monteCarloEstimate(const double& lowBound, const double& upBound, const int& iterations)
//Function to execute Monte Carlo integration on predefined function
{

	double totalSum = 0,
		randNum, functionVal;

	int iter = 0;

	while (iter < iterations - 1)
	{

		//Select a random number within the limits of integration
		randNum = lowBound + static_cast<double>(rand()) / RAND_MAX * (upBound - lowBound);

		//Sample the function's values
		functionVal = myFunction(randNum);

		//Add the f(x) value to the running sum
		totalSum += functionVal;

		iter++;
	}

	return (upBound - lowBound) * totalSum / iterations;
}

int main()
{
	setlocale(LC_ALL, "rus");

	cout << "Введите нижнюю границу, верхнюю границу и количество итераций." << endl;

	double lowerBound, upperBound;
	int iterations;
	cin >> lowerBound >> upperBound >> iterations;

	double estimate = monteCarloEstimate(lowerBound, upperBound, iterations);

	cout << "Для интервала [" << lowerBound << ", " << upperBound << "]: " << estimate
		<< endl << "Количество итераций: " << iterations << endl << "Затраченное время: " <<
		static_cast<double>(clock()) / 1000 << "с " << endl;

	return 0;
}