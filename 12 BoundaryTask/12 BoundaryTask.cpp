#define _USE_MATH_DEFINES

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

using namespace std;

#define StudentData nGroup, nStudent

constexpr double
a = 0, b = 4,
R = 0, S = 1, T = 1,
V = 2, W = 4, Z = 9;

double K_x(const double& x)
{
    return 2;
    //return 1;
}
double L_x(const double& x)
{
    return 2;
    //return 1;
}
double M_x(const double& x)
{
    return -4;
    //return 1;
}
double F_x(const double& x, const int& nGroup, const int& nStudent)
{
    return 1 - 2 * x;
    //return nStudent * (x + 1) + nGroup * cos(x);
}

double* Run_through_method(double** matrix, const int& n)
{
    auto* X = new double[n];
    auto* P = new double[n];
    auto* Q = new double[n];
    P[0] = -matrix[0][1] / matrix[0][0];
    Q[0] = matrix[0][n] / matrix[0][0];

    for (int i = 1; i < n; i++) 
    {
        P[i] = -matrix[i][i + 1] / (matrix[i][i] + matrix[i][i - 1] * P[i - 1]);
        Q[i] = (matrix[i][n] - matrix[i][i - 1] * Q[i - 1]) / (matrix[i][i] + matrix[i][i - 1] * P[i - 1]);
    }

    X[n - 1] = Q[n - 1];

    for (int i = n - 2; i >= 0; i--) 
    {
        X[i] = P[i] * X[i + 1] + Q[i];
    }

    return X;
}

void Output(double** _matrix, const int& node)
{
    cout << "---------------" << " Полученная матрица " << "---------------" << endl << endl;
    for (size_t i = 0; i < node; ++i)
    {
        for (size_t j = 0; j < node; ++j)
        {
            cout << fixed << setprecision(4) << setw(10) << _matrix[i][j] << ' ';
        }
        cout << fixed << setprecision(4) << setw(10) << _matrix[i][node] << ' ' << endl;
    }
    cout << endl << "---------------" << "                    " << "---------------" << endl << endl;

}

void PrintPoints(double** _matrix, const int& node)
{
    for (int i = 0; i < node; ++i)
    {
        cout << _matrix[0][i] << ',' << _matrix[1][i] << endl;
    }
}

void DifferenceMethod(const int& n, const int& nGroup, const int& nStudent)
{
    double x = a,
        h = (b - a) / (n - 1);

    auto** matrix = new double* [n];

    for (int i = 0; i < n; i++) 
    {
        matrix[i] = new double[n + 1];
    }

    auto** points = new double* [2];

    points[0] = new double[n];
    points[1] = new double[n];
    points[0][0] = a;

    for (int i = 1; i <= n; i++) 
    {
        points[0][i] = points[0][i - 1] + h;
    }

    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            matrix[i][j] = 0;
        }
    }

    matrix[0][0] = -(R / h - S);
    matrix[0][1] = R / h;
    matrix[0][n] = T;
    for (int i = 1; i < n - 1; i++) 
    {
        x = points[0][i];
        matrix[i][i - 1] = K_x(x) / (h * h) - L_x(x) / (2 * h); //a
        matrix[i][i] = -(2 * K_x(x) / (h * h) - M_x(x));//b
        matrix[i][i + 1] = K_x(x) / (h * h) + L_x(x) / (2 * h);//c
        matrix[i][n] = F_x(x, StudentData);//d
    }

    matrix[n - 1][n - 2] = V / h;//-
    matrix[n - 1][n - 1] = -(V / h + W);//-
    matrix[n - 1][n] = -Z;//-

    Output(matrix, n);

    points[1] = Run_through_method(matrix, n);
    cout << "Полученные координаты решения численного решения дифференциального уравнения с количеством узлов, равным " + to_string(n + 1) << endl;
    PrintPoints(points, n);

}

int main()
{
    setlocale(LC_ALL, "rus");

    cout << "Решение краевой задачи." << endl << "Введите номер группы, номер студента и количество интервалов." << endl;

    int nGroup, nStudent, n;
    cin >> nGroup >> nStudent >> n;

    DifferenceMethod(++n, StudentData);

    return 0;
}