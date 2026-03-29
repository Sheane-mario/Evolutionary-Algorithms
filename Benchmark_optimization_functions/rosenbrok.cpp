#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

// Implementation of the Rosenbrock (Banana) Function
double rosenbrock(double x, double y) {
    return 100.0 * std::pow((y - x * x), 2) + std::pow((x - 1.0), 2);
}

int main() {
    std::ofstream outFile("rosenbrock_data.csv");
    outFile << "x,y,z\n";

    // Standard domain for Rosenbrock is often [-2.048, 2.048]
    double step = 0.05;
    for (double x = -2.048; x <= 2.048; x += step) {
        for (double y = -2.048; y <= 2.048; y += step) {
            double z = rosenbrock(x, y);
            outFile << x << "," << y << "," << z << "\n";
        }
    }

    outFile.close();
    std::cout << "Data generated in rosenbrock_data.csv" << std::endl;
    return 0;
}