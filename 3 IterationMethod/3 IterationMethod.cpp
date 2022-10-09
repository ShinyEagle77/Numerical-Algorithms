#include <iostream>
#include <iomanip>
#include <cmath>
#include <exception>
#include <random>

using namespace std;

mt19937 mersenne(static_cast<unsigned int>(time(nullptr)));
uniform_int_distribution<> uid(-50, 50);

double** MatrixGen(int dimension)
{
    auto** matrix = new double* [dimension];

    for (int i = 0; i < dimension; i++)
    {
        matrix[i] = new double[dimension + 1];
    }

    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension + 1; j++)
        {
            matrix[i][j] = uid(mersenne);
        }
    }

    return matrix;
}

double** MatrixManual(int dimension, int groupNumber, int studentNumber) {

    auto** matrix = new double* [dimension];

    for (int i = 0; i < dimension; i++)
    {
        matrix[i] = new double[dimension + 1];
    }

    matrix[0][0] = studentNumber + 10;
    matrix[0][1] = groupNumber;
    matrix[0][2] = 1;
    matrix[0][3] = groupNumber;
    matrix[1][0] = groupNumber;
    matrix[1][1] = studentNumber + 10;
    matrix[1][2] = 3;
    matrix[1][3] = studentNumber + 10;
    matrix[2][0] = 1;
    matrix[2][1] = 3;
    matrix[2][2] = groupNumber + 4;
    matrix[2][3] = 0;

    return matrix;
}

double* IterationMethod(double** matrix, int n)
{

    setlocale(LC_ALL, "rus");

    for (int i = 0; i < n; i++)
    {
        if (matrix[i][i] == 0)
        {
            throw out_of_range("\nНулевой элемент в главной диагонали.\n");
        }
    }

    int i, j;
    for (i = 0; i < n; i++) {
        if (matrix[i][i] == 0) {
            cout << "\nError, null element in main dioganal!\n";
            exit(0);
        }
    }
    double temp;
    for (i = 0; i < n; i++) {
        temp = matrix[i][i];
        for (j = 0; j < n; j++) {
            matrix[i][j] = -matrix[i][j] / temp;
        }
        matrix[i][n] = matrix[i][n] / temp;
        matrix[i][i] = 0;
    }
    double summ = 0, max_summ = 0, a1, a2, a3;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            summ = summ + abs(matrix[i][j]);
        }
        if (summ > max_summ) {
            max_summ = summ;
        }
        summ = 0;
    }
    a1 = max_summ;
    max_summ = 0;
    for (j = 0; j < n; j++) {
        for (i = 0; i < n; i++) {
            summ = summ + abs(matrix[i][j]);
        }
        if (summ > max_summ) {
            max_summ = summ;
        }
        summ = 0;
    }
    a2 = max_summ;
    max_summ = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            summ = summ + pow(abs(matrix[i][j]), 2);
        }
    }
    max_summ = sqrt(summ);
    a3 = max_summ;
    double beta = 0, alpha;
    if (a1 < a2 && a1 < a3) {
        for (i = 0; i < n; i++) {
            if (abs(matrix[i][n]) > beta) {
                beta = abs(matrix[i][n]);
            }
        }
        alpha = a1;
    }
    else if (a2 < a1 && a2 < a3) {
        for (i = 0; i < n; i++) {
            beta = beta + abs(matrix[i][n]);
        }
        alpha = a2;
    }
    else {
        for (i = 0; i < n; i++) {
            beta = beta + pow(abs(matrix[i][n]), 2);
        }
        beta = sqrt(beta);
        alpha = a3;
    }
    int k = 1, S_err = 0;
    double eps = 0.00000001,
        eps_n = 1,
        eps_S = 1000;
    auto* X = new double[n];
	auto* X_n = new double[n];
    for (i = 0; i < n; i++) {
        X[i] = matrix[i][n];
    }
    while (eps_n > eps) {
        for (i = 0; i < n; i++) {
            X_n[i] = 0;
            for (j = 0; j < n; j++) {
                X_n[i] = X_n[i] + X[j] * matrix[i][j];
            }
            X_n[i] = X_n[i] + matrix[i][n];
        }
        if (alpha < 1) {
            eps_n = pow(alpha, k) / (1 - alpha) * beta;
            k++;
        }
        else {
            eps_n = 0;
            for (i = 0; i < n; i++) {
                eps_n = eps_n + pow(X_n[i] - X[i], 2);
            }
            eps_n = sqrt(eps_n);
        }
        if (eps_S < eps_n) {
            S_err++;
        }
        eps_S = eps_n;
        if (S_err > 5) {
            cout << "\nError, the solution diverges!\n";
            exit(0);
        }

        for (i = 0; i < n; i++) {
            X[i] = X_n[i];
        }
    }
    return X;
}

void PrintMatrix(double** matrix, int dimension)
{

    for (int i = 0; i < dimension; i++)
    {
        if (i == 0)
        {
            cout << "/";
        }

        else if (i == dimension - 1)
        {
            cout << "\\";
        }

        else
        {
            cout << "|";
        }

        for (int j = 0; j < dimension; j++)
        {
            cout << fixed << setprecision(4) << setw(9) << matrix[i][j] << " ";
        }

        cout << " | " << fixed << setprecision(4) << setw(9) << matrix[i][dimension];

        if (i == 0)
        {
            cout << " \\" << "\n";
        }

        else if (i == dimension - 1)
        {
            cout << " /" << "\n";
        }

        else
        {
            cout << " |" << "\n";
        }
    }
}

int main()
{

    setlocale(LC_ALL, "rus");

    try
    {

        int dimension, groupNumber, studentNumber;

        cout << "Решение СЛАУ методом Итераций.\n";
        cout << "0 - рандомная матрица, 1 - ручная: ";
        bool inputType;
        cin >> inputType;
        if (inputType == 0)
        {
            cout << "Введите размер матрицы: ";
            cin >> dimension;
        }
        else if (inputType == 1)
        {
            cout << "Введите номер группы: ";
            cin >> groupNumber;

            cout << "Введите номер студента: ";
            cin >> studentNumber;

            dimension = 3;
        }
        else
        {
            dimension = 3;
        }

        cout << "\n";

        auto** matrix = new double* [dimension];

        for (int i = 0; i < dimension; ++i) {
            matrix[i] = new double[dimension + 1];
        }
        auto* X = new double[dimension];

        if (inputType == 0)
        {
            matrix = MatrixGen(dimension);
        }

        if (inputType == 1)
        {
            matrix = MatrixManual(dimension, groupNumber, studentNumber);
        }

        PrintMatrix(matrix, dimension);

        X = IterationMethod(matrix, dimension);

        cout << "\nКорни уравнения: \n";

        for (int i = 0; i < dimension; i++)
        {
            cout << "X" << i + 1 << "=" << fixed << setprecision(4) << X[i] << "\n";
        }

    }
    catch (exception& ex) {
        cout << ex.what() << endl;
    }
}