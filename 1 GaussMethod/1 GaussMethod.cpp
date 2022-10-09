#include <iostream>
#include <iomanip>
#include <exception>
#include <ctime>
#include <cstdlib>
#include <random>

using namespace std;

mt19937 mersenne(static_cast<unsigned int>(time(nullptr)));
uniform_int_distribution<> uid(-50, 50);

double** inputMatrix(int matrixSize, bool outputType)
{
    setlocale(LC_ALL, "rus");
    srand(time(nullptr));

    auto** matrix = new double* [matrixSize];

    for (int i = 0; i < matrixSize; ++i)
    {
        matrix[i] = new double[matrixSize];
    }

    if (outputType == 1)
    {
        cout << "Введите матрицу А (числа вводятся через пробел)." << endl;
    }

    for (int i = 0; i < matrixSize; i++)
    {
        if (outputType == 1)
        {
            cout << "<" << i + 1 << " строчка>:" << endl;
        }

        for (int j = 0; j < matrixSize; j++)
        {
            if (outputType == 1)
            {
                cin >> matrix[i][j];
            }

            else if (outputType == 0)
            {
                matrix[i][j] = uid(mersenne);
            }
        }
    }

    if (outputType == 1)
    {
        cout << endl;
    }

    return matrix;
}

double* inputDecision(int matrixSize, bool outputType)
{
    setlocale(LC_ALL, "rus");

    auto* decision = new double[matrixSize];
    if (outputType == 1)
    {
        cout << "Введите матрицу решений (через пробел) " << endl;
    }
    for (int i = 0; i < matrixSize; i++)
    {
        if (outputType == 1)
        {
            cin >> decision[i];
        }
        else if (outputType == 0)
        {
            decision[i] = uid(mersenne);
        }
    }

    if (outputType == 1)
    {
        cout << endl;
    }

    return decision;
}

double* FindRoots(double** matrix, double* dec, int n)
{
    int iter, i, j;
    double* null_dia = new double[n + 1];
    double* roots = new double[n], summ = 0;
    for (iter = 0; iter < n; iter++) {
        for (i = iter; i < n; i++) {
            double temp = matrix[i][iter];
            if (temp != 0) {
                for (j = iter; j < n; j++) matrix[i][j] = matrix[i][j] / temp; //Нормирование элементов в строке.
                dec[i] = dec[i] / temp;
            }
        }
        for (i = iter + 1; i < n; i++) {
            if (matrix[iter][iter] == 0 && iter < n - 1) {
                null_dia = matrix[iter];
                matrix[iter] = matrix[iter + 1];
                matrix[iter + 1] = null_dia;
            }
            if (matrix[i][iter] != 0) {
                for (j = iter; j < n; j++) {
                    matrix[i][j] = matrix[i][j] - matrix[iter][j];//Функция вычитания для получения нуля.
                    summ = summ + abs(matrix[i][j]);//Поиск строки с нулевыми элементами.
                }
                dec[i] = dec[i] - dec[iter];
                if (summ == 0) {
                    exit(10);
                }
            }
        }
    }
    summ = 0;
    for (iter = n - 1; iter >= 0; iter--) {
        roots[iter] = dec[iter] - summ; //Подстановка корней обратной последовательностью.
        summ = 0;
        if (iter > 0) {
            for (i = n - 1; i >= iter; i--) summ = summ + matrix[iter - 1][i] * roots[i];
        }
    }
    return roots;
}

double** MinorMatrix(double** matrix, int matrixSize, int i, int j)
{
    int index_i{ 0 };

    auto** minor = new double* [matrixSize];

    for (int t = 0; t < matrixSize; t++)
    {
        minor[t] = new double[matrixSize];
    }

    for (int step_i = 0; step_i < matrixSize; step_i++)
    {
        if (step_i == i)
        {
            index_i = 1;
        }

        int index_j{ 0 };

        for (int step_j = 0; step_j < matrixSize; step_j++)
        {
            if (step_j == j)
            {
                index_j = 1;
            }

            minor[step_i][step_j] = matrix[step_i + index_i][step_j + index_j];
        }
    }
    return minor;
}

double CountDeterminant(double** matrix, int matrixSize)
{
    int i{ 0 };
    int minorSize{ matrixSize - 1 };

    if (matrixSize == 1)
    {
        double det = matrix[0][0];
        return det;
    }
    else if (matrixSize == 2)
    {
        double det = (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
        return det;
    }
    else if (matrixSize > 2)
    {
        double det{ 0 };
        for (int j = 0; j < matrixSize; j++)
        {
            det = det + pow(-1, j) * matrix[0][j] * CountDeterminant(MinorMatrix(matrix, minorSize, i, j), minorSize);
        }

        return(det);
    }
    else
    {
        return 0;
    }
}

void PrintMatrix(double** matrix, const double* dec, const int& n)
{
    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            cout << "/";
        }
        else if (i == n - 1)
        {
            cout << "\\";
        }
        else
        {
            cout << "|";
        }

        for (int j = 0; j < n; j++)
        {
            cout << setw(3) << matrix[i][j] << " ";
        }
        cout << " | " << setw(4) << dec[i];
        if (i == 0)
        {
            cout << " \\" << "\n";
        }
        else if (i == n - 1)
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
    try
    {
        setlocale(LC_ALL, "rus");

        cout << "Введите 0 для автоматического ввода и 1 - ручного. " << endl;
        int outputType;
        cin >> outputType;

        if (outputType != 0 && outputType != 1)
        {
            throw out_of_range("Введено неправильное число!");
        }

        cout << "Введите размер квадратичной матрицы" << endl;
        int matrixSize;
        cin >> matrixSize;

        if (matrixSize < 1)
        {
            throw out_of_range("Размер не может быть меньше 1.");
        }

        double** matrix = inputMatrix(matrixSize, outputType);
        double* decision = inputDecision(matrixSize, outputType);

        cout << "Ваша матрица: " << endl << endl;

        PrintMatrix(matrix, decision, matrixSize);

        if (CountDeterminant(matrix, matrixSize) == 0)
        {
            throw out_of_range("Детерминант матрицы равен 0, решения нет.");
        }
        else
        {
            double det = CountDeterminant(matrix, matrixSize);
            cout << "Детерминант матрицы равен: " << det << endl << endl;
        }

        auto* roots = new double[matrixSize];
        roots = FindRoots(matrix, decision, matrixSize);

        cout << "Корни уравнения: " << endl;
        for (int i = 0; i < matrixSize; i++)
        {
            cout << "x" << i + 1 << " = " << fixed << setprecision(4) << roots[i] << endl;
        }

        cout << endl;

    }
    catch (exception& ex)
    {
        cout << ex.what() << endl;
    }

    return 0;

}
