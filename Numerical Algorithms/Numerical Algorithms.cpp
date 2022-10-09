//+ 01 Метод Гаусса
//+ 02 Метод прогонки
//+ 03 Метод простых итераций
//+ 04 Метод Якоби
//+ 05.1 Уточнение корней уравнения метод простых итераций
//+ 05.2 Уточнение корней уравнения метод Ньютона
//+ 06.1 Построение интерполяционного многочлена методом Лагранжа
//+ 06.2 Построение интерполяционного многочлена методом Ньютона
//+ 07 Интерполяция сплайном
//+ 08 Метод наименьших квадратов
//- 09 Производная интерполяционного многочлена
//+ 10.1 Трапеция
//+ 10.2 Симпсона
//+ 11.1 Решение дифференциального уравнения методом Эйлера
//+ 11.2 Решение дифференциального уравнения методом Рунге - Кутта
//+ 12 Решение краевой задачи
//+ 13 Линеаризация

#include <iostream>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <map>
#include <vector>
#include <string>

using namespace std;

double f(double x, double y, double z, int n_g, int n_s) {
    return sqrt(n_s) * x - y;
    //return -z / (2 * x);
}


double** Minor_matrix(double** matrix, int n, int i, int j) {
    int t, step_i, step_j, index_i = 0, index_j;
    double** minor = new double* [n];
    for (t = 0; t < n; t++)
        minor[t] = new double[n];
    for (step_i = 0; step_i < n; step_i++) {
        if (step_i == i) index_i = 1;
        index_j = 0;
        for (step_j = 0; step_j < n; step_j++) {
            if (step_j == j) index_j = 1;
            minor[step_i][step_j] = matrix[step_i + index_i][step_j + index_j];
        }
    }
    return(minor);
}
//Процедура реккурсивного вычисления определителя матрицы.
double determinant(double** matrix, int n) {
    int i, j, k;
    double det = 0;
    k = n - 1;
    i = 0;
    if (n == 1) {
        det = matrix[0][0];
        return(det);
    }
    if (n == 2) {
        det = (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
        return(det);
    }
    if (n > 2) {
        for (j = 0; j < n; j++) det = det + pow(-1, j) * matrix[0][j] * determinant(Minor_matrix(matrix, k, i, j), k);
        return(det);
    }
}
void Eiler_second(double a, double b, double y_0, double z_0, double n, int n_g, int n_s) {
    int i, j, p = 2, pc = 0;
    double x_i, x_, y_i, y_, z_i, z_, y_pr, h, h_pr;
    double** D_1 = new double* [n];
    for (i = 0; i < n; ++i) {
        D_1[i] = new double[n];
    }
    double** D_2 = new double* [n];
    for (i = 0; i < n; ++i) {
        D_2[i] = new double[n];
    }
    for (i = 0; i < n; i++) {
        pc = pc + 5;
        h = (b - a) / pc;
        x_ = a;
        y_ = y_0;
        z_ = z_0;
        cout << "\nCoordinates of the numerical solution of the differential equation of " << pc << " nodes:\n";
        cout << setprecision(4) << fixed << "(" << x_ << "," << y_ << ")\n";
        for (j = 0; j < pc; j++) {
            x_i = x_ + h;
            y_i = y_ + h * z_;
            z_i = z_ + h * f(x_, y_, z_, n_g, n_s);
            x_ = x_i;
            y_ = y_i;
            z_ = z_i;
            if (n == 2 && i == 0) {
                y_pr = y_;
                h_pr = h;
            }
            cout << setprecision(4) << fixed << "(" << x_ << "," << y_ << ")\n";
        }
        if (n > 2) {
            for (j = 1; j < n; j++) {
                D_1[i][j] = pow(h, p + j - 1);
                D_2[i][j] = D_1[i][j];
            }
            D_1[i][0] = y_;
            D_2[i][0] = 1;
        }
    }
    if (n == 2) {
        cout << "\nThe last refined root by Runge-Romberg of the equation for " << setprecision(0) << n << " iteration: " << setprecision(6) << fixed << y_pr + (y_pr - y_) / (pow(h / h_pr, p) - 1) << "\n";
    }
    if (n > 2) {
        cout << "\nThe last refined root by Runge of the equation for " << setprecision(0) << n << " iteration: " << setprecision(6) << fixed << determinant(D_1, n) / determinant(D_2, n) << "\n";
    }
}
int main() {
    int n_g, n_s, n;
    double a, b, y_0, z_0;
    cout << "Solving a differential equation by the Euler method.\n";
    cout << "Enter the group number: ";
    cin >> n_g;
    cout << "Enter the student number: ";
    cin >> n_s;
    // cout << "Enter the borders [a,b]( a ->{space}-> b ->{enter}): ";
    // cin >> a >> b;
    a = 0;
    b = 3.14159265;
    y_0 = 0;
    z_0 = n_g + sqrt(n_s);
    /* a = 1;
     b = 10;
     y_0 = 2;
     z_0 = 1;*/
    cout << "Enter the number of consecutive node samples (iteration): ";
    cin >> n;
    Eiler_second(a, b, y_0, z_0, n, n_g, n_s);
    return 0;
}