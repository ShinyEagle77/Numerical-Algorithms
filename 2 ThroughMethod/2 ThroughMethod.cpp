#include <iostream>
#include <iomanip>

using namespace std;

typedef long long ll;

double** MatrixGenerator(int dimension, int groupNumber, ll studentNumber)
{
    auto** matrix = new double* [dimension];

    for (int i = 0; i < dimension; i++)
    {
        matrix[i] = new double[dimension + 1];
    }

    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            matrix[i][j] = 0;
        }
    }

    for (int i = 0; i < dimension; i++)
    {
        if (i > 0)
        {
            matrix[i][i - 1] = static_cast<double>((i + 1) * studentNumber + groupNumber);           //A
        }

        matrix[i][i] = static_cast<double>(-((i + 1) * (i + 1) * studentNumber + groupNumber));      //B

        if (i < dimension - 1)
        {
            matrix[i][i + 1] = static_cast<double>(groupNumber - (i + 1) * studentNumber);           //C
        }

        matrix[i][dimension] = static_cast<double>(studentNumber + groupNumber * (i + 1));           //D
    }
    return matrix;
}

double* RunThrough(double** matrix, int dimension)
{
    auto* X = new double[dimension];
    auto* P = new double[dimension];
    auto* Q = new double[dimension];

    P[0] = -matrix[0][1] / matrix[0][0];
    Q[0] = matrix[0][dimension] / matrix[0][0];

    for (int i = 1; i < dimension; i++)
    {
        P[i] = -matrix[i][i + 1] / (matrix[i][i] + matrix[i][i - 1] * P[i - 1]);
        Q[i] = (matrix[i][dimension] - matrix[i][i - 1] * Q[i - 1]) /
            (matrix[i][i] + matrix[i][i - 1] * P[i - 1]);
    }

    X[dimension - 1] = Q[dimension - 1];

    for (int i = dimension - 2; i >= 0; i--)
    {
        X[i] = P[i] * X[i + 1] + Q[i];
    }

    return X;
}

void PrintMatrix(double** matrix, int dimension)
{

    cout << "---------- Полученная матрица ---------- " << endl;
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
            cout << setw(4) << matrix[i][j] << " ";
        }

        cout << " | " << setw(4) << matrix[i][dimension];

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

    cout << "Решение матрицы методом прогонки. Введите размер, номер группы и номер студента. " << endl;
    int dimension, groupNumber, studentNumber;
    cin >> dimension >> groupNumber >> studentNumber;

    auto** matrix = new double* [dimension];

    for (int i = 0; i < dimension; ++i)
    {
        matrix[i] = new double[dimension + 1];
    }

    auto* X = new double[dimension];

    matrix = MatrixGenerator(dimension, groupNumber, studentNumber);

    PrintMatrix(matrix, dimension);

    X = RunThrough(matrix, dimension);
    cout << endl;

    cout << "---------- Корни уравнения ----------" << endl;

    for (int i = 0; i < dimension; i++)
    {
        cout << "X" << i + 1 << " = " << fixed << setprecision(4) << X[i] << "\n";
    }
    cout << endl;

    return 0;

}