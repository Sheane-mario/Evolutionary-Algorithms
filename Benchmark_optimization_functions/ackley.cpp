#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <numbers>

const double LOWER_BOUND = -30.0;
const double UPPER_BOUND = 30.0;

double ackley(double x, double y) {
    const double PI = 3.14159265358979323846;
    double t1 = -20.0 * std::exp(-0.2 * std::sqrt((x * x + y * y) * 0.5));
    double t2 = -std::exp(0.5 * (std::cos(2.0 * PI * x) + std::cos(2.0 * PI * y)));
    return 20.0 + std::exp(1.0) + t1 + t2;
}

int main() {
    std::ofstream outFile("ackley_data.csv");
    outFile << "x,y,z\n";

    double step = 0.2;
    for (double x = LOWER_BOUND; x <= UPPER_BOUND; x += step) {
        for (double y = LOWER_BOUND; y <= UPPER_BOUND; y += step) {
            double z = ackley(x, y);
            outFile << x << "," << y << "," << z << "\n";
        }
    }
    outFile.close();
    std::cout << "Data generated in ackley_data.csv" << "\n";
    return 0;
}