#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

const double PI = 3.14159265358979323846;

double u(double xi, double a, double k, double m) {
    if (xi > a) return k * std::pow(xi - a, m);
    if (xi < -a) return k * std::pow(-xi - a, m);
    return 0;
}

double penalty1(double x1, double x2) {
    double x[2] = { x1, x2 };
    double y[2];
    for (int i = 0; i < 2; ++i) y[i] = 1.0 + (x[i] + 1.0) / 4.0;

    double term1 = 10.0 * std::pow(std::sin(PI * y[0]), 2);
    double term2 = std::pow(y[0] - 1.0, 2) * (1.0 + 10.0 * std::pow(std::sin(PI * y[1]), 2));
    double term3 = std::pow(y[1] - 1.0, 2);

    double sum_u = u(x[0], 10, 100, 4) + u(x[1], 10, 100, 4);

    return (PI / 2.0) * (term1 + term2 + term3) + sum_u;
}

int main() {
    std::ofstream outFile("penalty1_data.csv");
    outFile << "x,y,z\n";

    // Domain is [-50, 50]. Using step 1.0 for the overview.
    double step = 1.0;
    for (double x = -50.0; x <= 50.0; x += step) {
        for (double y = -50.0; y <= 50.0; y += step) {
            outFile << x << "," << y << "," << penalty1(x, y) << "\n";
        }
    }

    outFile.close();
    std::cout << "Data generated in penalty1_data.csv" << std::endl;
    return 0;
}