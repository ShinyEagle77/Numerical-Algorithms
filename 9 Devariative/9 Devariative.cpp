#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include <string>

using namespace std;

uniform_real_distribution<> uid(-10, 10);

#define StudentData nGroup, nStudent
#define MatrixData _matrix, node

void PrintMatrix(double** _matrix, const int& node)
{
    for (int i = 0; i < node; ++i)
    {
        for (int j = 0; j < node; ++j)
        {
            cout << setw(12) << setprecision(4) << fixed << _matrix[i][j] << " * x ^ " + to_string(node - j) << ' ';
        }
        cout << setw(7) << setprecision(4) << fixed << _matrix[i][node] << ' ' << endl;
    }
    cout << "\n";
}

void PrintPoints(double** _matrix, const int& node)
{
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
            if (node >= 2 && node <= 5) {
                for (int i = 0; i < node; i++) {
                    _matrix[0][i] = -2;
                }
                _matrix[0][node] = nStudent;
            }
            if (node >= 2 && node <= 5) {
                for (int i = 0; i < node; i++) {
                    _matrix[1][i] = -1;
                }
                _matrix[1][node] = nGroup;
            }
            if (node >= 3 && node <= 5) {
                for (int i = 0; i < node; i++) {
                    _matrix[2][i] = 0;
                }
                _matrix[2][node] = -1;
            }
            if (node >= 4 && node <= 5) {
                for (int i = 0; i < node; i++) {
                    _matrix[3][i] = 1;
                }
                _matrix[3][node] = nStudent;
            }
            if (node == 5) {
                for (int i = 0; i < node; i++) {
                    _matrix[4][i] = 2;
                }
                _matrix[4][node] = nGroup;
            }
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
        
    }
    PrintPoints(MatrixData);

    for (int i = 0; i < node; i++) {
        for (int j = 0; j < node; j++) {
            _matrix[i][j] = pow(_matrix[i][j], node - 1 - j);
        }
    }
    PrintMatrix(MatrixData);

    return _matrix;
}

double* FindRoots(double** matrix, int n)
{
    int iter, i, j;
    double* roots = new double[n], summ = 0;
    for (iter = 0; iter < n; iter++) {
        for (i = iter; i < n; i++) {
            double temp = matrix[i][iter];
            if (temp != 0) {
                for (j = iter; j < n; j++) {
                    matrix[i][j] = matrix[i][j] / temp;
                }//Нормирование элементов в строке.
                matrix[i][n] = matrix[i][n] / temp;
            }
        }
        for (i = iter + 1; i < n; i++) {
            if (matrix[i][iter] != 0) {
                for (j = iter; j < n; j++) {
                    matrix[i][j] = matrix[i][j] - matrix[iter][j];//Функция вычитания для получения нуля.
                    summ = summ + abs(matrix[i][j]);//Поиск строки с нулевыми элементами.
                }
                matrix[i][n] = matrix[i][n] - matrix[iter][n];
                if (summ == 0) {
                    cout << "Error, null line!";
                    exit(1);
                }
            }
        }
    }
    summ = 0;
    for (iter = n - 1; iter >= 0; iter--) {
        roots[iter] = matrix[iter][n] - summ; //Подстановка корней обратной последовательностью.
        summ = 0;
        if (iter > 0) {
            for (i = n - 1; i >= iter; i--) {
                summ = summ + matrix[iter - 1][i] * roots[i];
            }
        }
    }
    return roots;
}

void PrintRootsFinal(const double* roots, const int& der_lvl, const int& node)
{
    int i;
    cout << "P";
    if (der_lvl < 4) 
    {
        for (i = 0; i < der_lvl; i++) 
        {
            cout << "'";
        }
    }
    else 
    {
        cout << "^(" << der_lvl << ")";
    }
    cout << "(x) = ";
    if (der_lvl < node) 
    {
        for (i = 0; i < node - der_lvl; i++) 
        {
            if (i != 0) 
            {
                cout << "+";
            }
            cout << "(" << setw(7) << setprecision(4) << fixed << roots[i] << " )*x^(" << node - 1 - i - der_lvl << ")";
        }
    }
    else 
    {
        cout << "0";
    }
    cout << "\n";
}

void Root_output(const double* roots, const int& der_lvl, const int& n)
{
    int i;
    cout << "P";
    if (der_lvl < 4) {
        for (i = 0; i < der_lvl; i++) {
            cout << "'";
        }
    }
    else {
        cout << "^(" << der_lvl << ")";
    }
    cout << "(x) = ";
    if (der_lvl < n) {
        for (i = 0; i < n - der_lvl; i++) {
            if (i != 0) {
                cout << ' ';
            }
            cout << ' ' << setw(8) << setprecision(3) << fixed << roots[i] << "*x^" << n - 1 - i - der_lvl << "";
        }
    }
    else {
        cout << "0";
    }
    cout << "\n";
}

void Derivative(const double* roots, const int& n) // TO DO
{
    int der_lvl = n;

    auto** der_cf = new double* [der_lvl + 1];
    for (int i = 0; i < der_lvl + 1; i++) 
    {
        der_cf[i] = new double[n];
    }

    for (int i = 0; i < n; i++) 
    {
        der_cf[0][i] = roots[i];
    }

    for (int lvl = 1; lvl <= der_lvl; lvl++) 
    {
        for (int i = 0; i < n; i++) 
        {
            if (i < n - lvl) 
            {
                der_cf[lvl][i] = der_cf[lvl - 1][i] * (n - 1 - i);
            }
            else 
            {
                der_cf[lvl][i] = 0;
            }
        }
    }
    for (int i = 0; i < der_lvl; i++) 
    {
        Root_output(der_cf[i], i, n);
    }
}

int main()
{
    setlocale(LC_ALL, "rus");

    cout << "Решение производной." << endl;
    cout << "Введите размер таблицы, номер группы и номер студента, тип ввода: 0 - параметры студента, 1 - рандом (через пробел)" << endl;

    int node, nGroup, nStudent, inputType; // nStudent & nGroup defined as StudentData;
    cin >> node >> nGroup >> nStudent >> inputType;

    auto** _matrix = CreateMatrix(node, StudentData, inputType);
    auto* _roots = FindRoots(MatrixData);

    cout << "Корни системы: ";
    for (int i = 0; i < node; ++i)
    {
        cout << setw(5) << _roots[i] << ' ';
    }

    cout << endl << endl;

    Derivative(_roots, node);

    return 0;
}


