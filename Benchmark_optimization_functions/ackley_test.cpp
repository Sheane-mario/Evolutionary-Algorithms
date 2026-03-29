#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

double ackleyTest(double x1, double x2) {
    double term1 = 3.0 * (std::cos(2.0 * x1) + std::sin(2.0 * x2));
    double term2 = std::exp(-0.2) * std::sqrt(x1 * x1 + x2 * x2);
    return term1 + term2;
}

int main() {
    std::ofstream outFile("ackley_test_data.csv");
    outFile << "x,y,z\n";

    double step = 0.5;
    for (double x = -30.0; x <= 30.0; x += step) {
        for (double y = -30.0; y <= 30.0; y += step) {
            double z = ackleyTest(x, y);
            outFile << x << "," << y << "," << z << "\n";
        }
    }

    outFile.close();
    std::cout << "Data generated in ackley_test_data.csv" << std::endl;
    return 0;
}