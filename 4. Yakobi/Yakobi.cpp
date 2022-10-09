#include <iostream>
#include <vector>
#include <cmath>
#include <exception>
#include <iomanip>
#include <random>
#include <string>

using namespace std;

mt19937 mersenne(static_cast<unsigned int>(time(nullptr)));
uniform_int_distribution<> uid(-25, 25);

static int spectralIterCount;
static int yakobiIterCount;
static const double epsAcc = 0.0001;
static vector<double> eigenvalues;

void PrintMatrix(double **matrix, const int& mSize, const string& message)
{
    cout << message << " \n";
	
    for (int i = 0; i < mSize; i++)
    {
        if (i == 0) {
            cout << "/";
        }
        else if (i == mSize - 1) {
            cout << "\\";
        }
        else {
            cout << "|";
        }
        for (int j = 0; j < mSize; j++) {
            cout << fixed << setprecision(4) << setw(9) << matrix[i][j];
        }
        if (i == 0) {
            cout << " \\" << "\n";
        }
        else if (i == mSize - 1) {
            cout << " /" << "\n";
        }
        else {
            cout << " |" << "\n";
        }
    }

    cout << endl;
	
}

double **MatrixGen(const int& mSize)
{
    auto** matrix = new double* [mSize];

    for (int i = 0; i < mSize; i++)
    {
        matrix[i] = new double[mSize];
    }

    for (int i = 0; i < mSize; i++)
    {
        for (int j = 0; j < mSize; j++)
        {
            matrix[i][j] = uid(mersenne);
            matrix[j][i] = matrix[i][j];
        }
    }

    return matrix;
}

double **ManualMatrix(const int& mSize, const int& nStudent, const int& nGroup)
{
    auto** matrix = new double* [mSize];

	for (int i = 0; i < mSize; ++i)
	{
        matrix[i] = new double[mSize];
	}

    /*matrix[0][0] = nStudent + 10;
    matrix[1][0] = nGroup;
    matrix[2][0] = 1;
    matrix[2][1] = 3;

    matrix[2][2] = nGroup + 4;

    matrix[0][1] = matrix[1][0];
    matrix[1][1] = matrix[0][0];
    matrix[0][2] = matrix[2][0];
    matrix[1][2] = matrix[2][1];*/

    matrix[0][0] = 17;
    matrix[1][0] = 1;
    matrix[2][0] = matrix[1][0];
    matrix[0][1] = matrix[1][0];
    matrix[0][2] = matrix[1][0];
    matrix[1][1] = matrix[0][0];
    matrix[2][1] = 2;
    matrix[1][2] = matrix[2][1];
    matrix[2][2] = 4;

    return matrix;
}

double* MatrixMultiVector (double** matrix, const double* vec, const int& mSize)
{
    auto* resVec = new double[mSize];

	for (int i = 0; i < mSize; ++i)
	{
        resVec[i] = 0;
		
		for (int j = 0; j< mSize; ++j)
		{
            resVec[i] += matrix[i][j] * vec[j];
		}
	}

    return resVec;
	
}

double FindSpectralRadius (double **matrix, const int& mSize)
{
    auto* mOmega = new double [mSize];
	for (int i = 0; i < mSize; ++i)
	{
        mOmega[i] = 1;
	}
    delete[] mOmega;

    double oldNorm = 0, maxNewNorm = 0;
    double difNorm = 1;
	
    while (difNorm > epsAcc)
    {
        mOmega = MatrixMultiVector(matrix, mOmega, mSize);

        maxNewNorm = mOmega[0];                      
                                                     
    	for (int i = 0; i < mSize; ++i)              
    	{                                            
    		if (mOmega[i] > maxNewNorm)              
    		{                                        
                maxNewNorm = mOmega[i];              
    		}
    	}

    	for (int i = 0; i < mSize; ++i)
    	{
            mOmega[i] = mOmega[i] / maxNewNorm;
    	}


        difNorm = abs(maxNewNorm - oldNorm);
        oldNorm = maxNewNorm;
        ++spectralIterCount;

    	if (spectralIterCount > 15000)
    	{
            throw out_of_range("Нет решения, итераций совершенно - > 15000, last difNorm value is:  " + to_string(difNorm) + ' ' + to_string(spectralIterCount));
    	}
    }
	
    return abs(maxNewNorm);
}

void MakeIdentity (double** matrix, const int& mSize)
{
	for (int i = 0; i < mSize; ++i)
	{
		for (int j = 0; j < mSize; ++j)
		{
			if (i == j)
			{
                matrix[i][j] = 1;
			}
            else
            {
                matrix[i][j] = 0;
            }
		}
	}
}

double** MatrixMultiMatrix(double** matrixA, double** matrixB, const int& mSize)
{
    auto** resMatrix = new double* [mSize];
	for (int i = 0; i < mSize; ++i)
	{
        resMatrix[i] = new double[mSize];
	}

	for (int i = 0; i < mSize; ++i)
	{
		for (int j = 0; j < mSize; ++j)
		{
            resMatrix[i][j] = 0;

			for (int k = 0; k < mSize; ++k)
			{
                resMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
			}
		}
	}

    return resMatrix;
	
}

double** MakeTransposed(double** matrix, const int& mSize)
{
    auto** resMatrix = new double* [mSize];
    for (int i = 0; i < mSize; ++i)
    {
        resMatrix[i] = new double[mSize];
    }

	for (int i = 0; i < mSize; ++i)
	{
		for (int j = 0; j < mSize; ++j)
		{
            resMatrix[i][j] = matrix[j][i];
		}
	}

    return resMatrix;
	
}

double** FindYakobiRotation (double** matrix, const int& mSize)
{
    double maxNorm = 1;

    auto** matrixH = new double* [mSize];
    for (int i = 0; i < mSize; ++i)
    {
        matrixH[i] = new double[mSize];
    }

    auto** resMatrix = new double* [mSize];
    for (int i = 0; i < mSize; ++i)
    {
        resMatrix[i] = new double[mSize];
    }

    MakeIdentity(resMatrix, mSize);
	
	while (abs(maxNorm) > epsAcc)
	{
        int _i = 0, _j = 0;
        maxNorm = 0;
		for (int i = 0; i < mSize; ++i)
		{
			for (int j = 0; j < mSize; ++j)
			{
				if (abs(matrix[i][j]) >= abs(maxNorm) && i != j)
				{
                    maxNorm = matrix[i][j];
                    _i = i;
                    _j = j;
				}
			}
		}

        double rotationAngle = 0.5 * atan(2 * maxNorm / (matrix[_i][_i] - matrix[_j][_j]));

        MakeIdentity(matrixH, mSize);

        matrixH[_i][_i] = cos(rotationAngle);
        matrixH[_j][_i] = sin(rotationAngle);
		
        matrixH[_j][_j] = matrixH[_i][_i];
        matrixH[_i][_j] = -matrixH[_j][_i];

        resMatrix = MatrixMultiMatrix(matrixH, resMatrix, mSize);
        matrix = MatrixMultiMatrix(MatrixMultiMatrix(MakeTransposed(matrixH, mSize), matrix, mSize), matrixH, mSize);

        ++yakobiIterCount;

        if (yakobiIterCount > 15000)
        {
            throw out_of_range("Нет решения, итераций совершенно - > 15000, last maxNorm value is: " + to_string(maxNorm) + ' ' + to_string(yakobiIterCount));
        }
		
	}

    for (int i = 0; i < mSize; ++i)
    {
        for (int j = 0; j < mSize; ++j)
        {
            if (i == j)
            {
                eigenvalues.push_back(matrix[i][j]);
            }
        }
    }

    cout << endl;

    return resMatrix;
	
}

double** MakeMatrixNorm (double** matrix, const int& mSize)
{
    double maxElement = 0;

	for (int i = 0; i < mSize; ++i)
	{
        maxElement = matrix[i][i];
		for (int j = 0; j < mSize; ++j)
		{
            matrix[i][j] = matrix[i][j] / maxElement;
		}
	}

    return matrix;

}

int main()
{
    setlocale(LC_ALL, "rus");

    try
    {
        cout << "Нахождение спектрального радиуса и метод вращений Якоби для симм. матрицы" << endl;
        cout << "Для рандомной матрицы введите 0, для заданной - 1" << endl;

        int inputType, mSize, nStudent, nGroup;
        cin >> inputType;

        if (inputType == 0)
        {
            cout << "Введите размер матрицы: ";
            cin >> mSize;
        }

        else if (inputType == 1)
        {
            cout << "Введите номер группы: ";
            cin >> nGroup;

            cout << "Введите номер студента: ";
            cin >> nStudent;

            mSize = 3;

        }
        else
        {
            throw out_of_range("Неверный тип ввода.");
        }

        auto** matrix = new double* [mSize];

        for (int i = 0; i < mSize; ++i) 
        {
            matrix[i] = new double[mSize + 1];
        }

		if (inputType == 0)
		{
            matrix = MatrixGen(mSize);
		}
        else if (inputType == 1)
        {
            matrix = ManualMatrix(mSize, nStudent, nGroup);
        }

        cout << "\n";

        PrintMatrix(matrix, mSize, "Базовая матрица:");

        cout << "Спектральный радиус заданной матрицы: " << FindSpectralRadius(matrix, mSize) << endl << "Количество совершённых итераций: " << spectralIterCount << endl;

        matrix = FindYakobiRotation(matrix, mSize);

        PrintMatrix(matrix, mSize, "Матрица V, полученная перемножением матриц поворотов:");
        cout << "Количество совершённых итераций: " << yakobiIterCount << endl;

        cout << "Собственные числа матрицы: ";
    	for (size_t i = 0; i < eigenvalues.size(); ++i)
    	{
    		if (i != eigenvalues.size() - 1)
    		{
                cout << eigenvalues[i] << ", ";
    		}
            else
            {
                cout << eigenvalues[i] << endl << "\n";
            }
    	}

        matrix = MakeMatrixNorm(matrix, mSize);

        cout << "Собственные вектора матрицы: " << endl;
        for (int i = 0; i < mSize; i++) {
            cout << "V" + to_string(i + 1) << " = " << fixed << setprecision(4) << setw(3) << matrix[i][0];
            for (int j = 1; j < mSize; j++) {
                cout << ", " << fixed << setprecision(4) << matrix[i][j];
            }
            cout << "\n";
        }

    }

	catch (exception& ex)

    {
        cout << ex.what() << endl;
    }

    return 0;
}