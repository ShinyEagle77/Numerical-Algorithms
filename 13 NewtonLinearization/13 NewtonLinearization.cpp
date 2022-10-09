#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>

static constexpr double eps = 0.00001;
static int itCount = 0;

using namespace std;

#define IntervalData a, b
#define StudentData nGroup, nStudent

double f1(const double& a, const double& b, const int& nGroup, const int& nStudent)
{
    return nGroup * (pow(a, 3) - pow(b, 3)) - nStudent;
}

double f1_x(const double& a, const int& nGroup)
{ //a
    return 3 * nGroup * pow(a, 2);
}

double f1_y(const double& b, const int& nGroup)
{ //b
    return -3 * nGroup * pow(b, 2);
}

double f2(const double& a, const double& b, const int& nGroup, const int& nStudent)
{
    return a * b - b - nStudent * nGroup;
}

double f2_x(const double& b)
{ //c
    return b;
}

double f2_y(const double& a)
{ //d
    return a - 1;
}

pair<double, double> Eqw(const double& _x, const double& _y, const int& nGroup, const int& nStudent)
{
    double a = f1_x(_x, nGroup),
        b = f1_y(_y, nGroup),
        c = f2_x(_y),
        d = f2_y(_x),
        _f1 = f1(_x, _y, StudentData),
        _f2 = f2(_x, _y, StudentData),
        cof = 1 / (a * d - c * b),
        x_ = _x - cof * (d * _f1 - b * _f2),
        y_ = _y - cof * (-c * _f1 + a * _f2);

    return make_pair(x_, y_);
}

void Newton(double& a, double& b, const int& nGroup, const int& nStudent)
{
    setlocale(LC_ALL, "rus");

    pair<double, double> eqw = Eqw(IntervalData, StudentData);

    for (int k = 0; abs(eqw.first - a) > eps && abs(eqw.second - b) > eps; ++k)
    {
        a = eqw.first;
        b = eqw.second;
        eqw = Eqw(IntervalData, StudentData);
        ++itCount;
    }
    cout << "Полученные координаты точки решения системы уравнений: " << fixed << setprecision(4) << eqw.first << ' ' << eqw.second << endl
        << "Количество совершённых итераций: " + to_string(itCount) << endl << "С точностью, равной " + to_string(eps) << endl;
}

int main()
{
    setlocale(LC_ALL, "rus");

    cout << "Линеаризация Ньютона для нахождения решений системы уравнений." << endl
        << "Введите номер группы и номер студента." << endl;
    int nGroup, nStudent;
    cin >> nGroup >> nStudent;

    cout << "Введите границы интервала" << endl;
    double a, b;
    cin >> a >> b;

    Newton(IntervalData, StudentData);

    return 0;
}

