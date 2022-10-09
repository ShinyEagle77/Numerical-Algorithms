#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>
#include <exception>
#include <map>
#include <random>
#include <vector>

using namespace std;

uniform_int_distribution<> uid(-10, 10);

static int nValue = 4;
static int nStep = 1;
static int iterationCount = 0;

double S(const double& _h, const double& _m, const double& _mPrev, const double& _x, 
    const double& _xPrev, const double& _y, const double& _yPrev)
{
    return (_mPrev * pow(_x, 3) - _m * pow(_xPrev, 3) + 6 * _yPrev * _x - 6 * _y * _xPrev + _m * pow(_h, 2) * _xPrev - _mPrev * pow(_h, 2) * _x) / (6 * _h);
}

double S1(const double& _h, const double& _m, const double& _mPrev, const double& _x,
    const double& _xPrev, const double& _y, const double& _yPrev)
{
    return (_m * pow(_xPrev, 2) - _mPrev * pow(_x, 2) + 2 * _y - 2 * _yPrev + _mPrev * pow(_h, 2) / 3 - _m * pow(_h, 2) / 3) / (2 * _h);
}

double S2(const double& _h, const double& _m, const double& _mPrev, const double& _x, const double& _xPrev)
{
    return (_mPrev * _x - _m * _xPrev) / (2 * _h);
}

double S3(const double& _h, const double& _m, const double& _mPrev)
{
    return (_m - _mPrev) / (6 * _h);
}

double P(const double& h_i, const double& h_i_p, const double& P_i)
{
    return -h_i_p / (2 * (h_i + h_i_p) + P_i * h_i);
}

double Q(const double& h_i, const double& h_i_p, const double& P_i, const double& Q_i, const double& Y_i) 
{
    return (6 * Y_i - h_i * Q_i) / (2 * (h_i + h_i_p) + P_i * h_i);
}


//void PrintMatrix(double** _matrix, const int& node)
//{
//	for (int i = 0; i < node; ++i)
//	{
//		for (int j = 0; j < node; ++j)
//		{
//			cout << setw(static_cast<streamsize>(4)) << setprecision(4) << fixed << _matrix[i][j] << ' ';
//		}
//		cout << setw(static_cast<streamsize>(4)) << setprecision(4) << fixed << _matrix[i][node] << ' ' << endl;
//        cout << endl;
//	}
//	cout << endl;
//}

void PrintMatrix(double** _final, const int& node)
{
    for (int i = 0; i < node - 1; i++) {
        cout << setprecision(6) << fixed << "S" << i + 1 << "(x)=( " << _final[i][2] << " )*x^3+( " << _final[i][3]
            << " )*x^2+( " << _final[i][4] << " )*x+( " << _final[i][5] << " ); {"
            << setprecision(2) << fixed << _final[i][0] << "<=x<=" << _final[i][1] << "}\n";
    }
}

void PrintPoints(double** _matrix, const int& node)
{
    cout << "i: ";
    for (int i = 1; i <= node; ++i)
    {
        cout << setw(7) << i << ' ';
    }
    cout << endl << "X: ";
    for (int i = 0; i < node; ++i)
    {
        cout << setw(7) << _matrix[0][i] << ' ';
    }
    cout << endl << "Y: ";
    for (int i = 0; i < node; ++i)
    {
        cout << setw(7) << setprecision(4) << fixed << _matrix[1][i] << ' ';
    }
    cout << endl << endl;
}

//vector<double> CountSplineLength (double** _result, const int& node)
//{
//    vector<double> _h;
//    for (int i = 0; i < node - 1; ++i)
//    {
//        _h.push_back(_result[0][i + 1] - _result[0][i]);
//    }
//
//    return _h;
//}
//
//vector<double> CountY (double **_result, const vector<double>& _h, const int& node)
//{
//    vector<double> _y;
//    for (size_t i = 0; i < node - 2; ++i)
//    {
//        double pushValue = (_result[1][i + 2] - _result[1][i + 1]) / _h[i + 1] - (_result[1][i + 1] - _result[1][i]) / _h[i];
//        _y.push_back(pushValue);
//    }
//
//    return _y;
//}
//
//vector<double> CountP (const vector<double>& _h, const int& node)
//{
//    vector<double> _p;
//    _p.push_back(0);
//    for (size_t i = 1; i < node - 2; ++i)
//    {
//        double pushValue = -_h[i] / (2 * (_h[i - 1] + _h[i]) + _p[i - 1] * _h[i - 1]);
//        _p.push_back(pushValue);
//    }
//
//    return _p;
//}
//
//vector<double> CountQ (const vector<double>& _y, const vector<double>& _p, const vector<double>& _h, const int& node)
//{
//    vector<double> _q;
//    _q.push_back(0);
//    for (size_t i = 1; i < _h.size(); ++i)
//    {
//        double pushValue = (6 * _y[i - 1] - _h[i - 1] * _q[i - 1]) / (2 * (_h[i - 1] + _h[i]) + _p[i - 1] * _h[i - 1]);
//        _q.push_back(pushValue);
//    }
//
//    return _q;
//}
//
//vector<double> CountM(const vector<double>& _p, const vector<double>& _q, const int& node)
//{
//    vector<double> _m(node);
//    _m[node - 1] = 0;
//    for (int i = node - 1; i > 0; --i)
//    {
//        _m[i - 1] = _p[i - 1] * _m[i] + _q[i - 1];
//    }
//
//    /*reverse(_m.begin(), _m.end());*/
//
//    return _m;
//}


double** CubicSpline(double** _result, const int& node) // MAKE DIFFERENT FUNCTION FOR A MANUAL INPUT AND AUTOMATIC
{

	//PrintMatrix(_result, node);
    PrintPoints(_result, node);

    auto* P_i = new double[node - 1];
    auto* Q_i = new double[node - 1];
    auto* h_i = new double[node - 1];

    for (int i = 0; i < node - 1; i++) 
    {
        h_i[i] = _result[0][i + 1] - _result[0][i];
    }

    P_i[0] = 0, Q_i[0] = 0;

    for (int i = 0; i < node - 2; i++) 
    {
        double Y_i = (_result[1][i + 2] - _result[1][i + 1]) / (h_i[i + 1]) - (_result[1][i + 1] - _result[1][i]) / (h_i[i]);
        P_i[i + 1] = P(h_i[i], h_i[i + 1], P_i[i]);
        Q_i[i + 1] = Q(h_i[i], h_i[i + 1], P_i[i], Q_i[i], Y_i);
    }

    auto* m_i = new double[node];
    m_i[node - 1] = 0;
    for (int i = node - 1; i > 0; i--) 
    {
        m_i[i - 1] = P_i[i - 1] * m_i[i] + Q_i[i - 1];
    }

    auto** _final = new double* [node - 1];
    for (int i = 0; i < node - 1; ++i)
    {
        _final[i] = new double[6]; 
    }

    for (int i = 0; i < node - 1; i++) {
        _final[i][0] = _result[0][i];
        _final[i][1] = _result[0][i + 1];
        _final[i][2] = S3(h_i[i], m_i[i + 1], m_i[i]);
        _final[i][3] = S2(h_i[i], m_i[i + 1], m_i[i], _result[0][i + 1], _result[0][i]);
        _final[i][4] = S1(h_i[i], m_i[i + 1], m_i[i], _result[0][i + 1], _result[0][i], _result[1][i + 1], _result[1][i]);
        _final[i][5] = S(h_i[i], m_i[i + 1], m_i[i], _result[0][i + 1], _result[0][i], _result[1][i + 1], _result[1][i]);
    }

	return _final;
}

double** MatrixHub(const int& node, const int& nGroup, const int& a, const int& b)
{
    auto** _result = new double* [2];
    for (int i = 0; i < 2; ++i)
    {
        _result[i] = new double[node];
    }

    for (int x = 0; x < node; x += nStep)
    {
        _result[0][x] = x + a;
        _result[1][x] = nGroup * _result[0][x] + sin(_result[0][x]);                   // Main function to fill;
       /* _result[1][x - a] = x * sin(x);*/
    }
    return CubicSpline(_result, node);
}

double** MatrixHub(const int& node, const int& a, const int& b)
{
    auto** _result = new double* [2];
    for (int i = 0; i < 2; ++i)
    {
        _result[i] = new double[node];
    }

    for (int x = a; x <= b; x += nStep)
    {
        _result[0][x - a] = x;
        _result[1][x - a] = cos(x) + sin(x);
    }
    return CubicSpline(_result, node);
}

pair<int, int> CountTableSize(const int& nStudent)
{
    const int a = nStudent,
			b = nStudent + nValue;                                // nValue = 4;
	/*const int a = -4, b = 4;*/

    return make_pair(a, b);
}

pair<int, int> CountTableSize()
{
    random_device dev;
    mt19937 _randengine(dev());

    int a = uid(_randengine),
	b = uid(_randengine) + nValue;

    if (a > b)
    {
        int temp = b;
        a = b;
        b = temp;
    }
    return make_pair(a, b);
}

int main()
{
	setlocale(LC_ALL, "rus");

    cout << "Аппроксимация табличной функции кубическими сплайнами. " << endl
        << "Для ввода параметров студента, введите 0. Для ручного ввода параметров и рандомных точек: 1" << endl;

    int inputType;
    cin >> inputType;

    if (inputType == 0)
    {
        cout << "Введите номер группы и номер студента (через пробел)" << endl;
    	int nGroup, nStudent;
	    cin >> nGroup >> nStudent;                                      // Defined as StudentData

	    const int node = abs(CountTableSize(nStudent).first - CountTableSize(nStudent).second) + 1;

    	auto** _final = MatrixHub(node, nGroup, CountTableSize(nStudent).first, CountTableSize(nStudent).second);
    	PrintMatrix(_final, node);
    }
    else if (inputType == 1)
    {
        cout << "Введите значение N для задания интервала, шаг итерации: " << endl;
        cin >> nValue >> nStep;

        const int node = abs(CountTableSize().first - CountTableSize().second) + 1;

        auto** _final = MatrixHub(node, CountTableSize().first, CountTableSize().second);
        PrintMatrix(_final, node);
    }
   

    return 0;

}