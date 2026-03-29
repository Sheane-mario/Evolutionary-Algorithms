#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

double griewank(double x, double y) {
    double sum = (x * x + y * y) / 4000.0;
    double prod = std::cos(x) * std::cos(y / std::sqrt(2.0));
    return 1.0 + sum - prod;
}

int main() {
    std::ofstream outFile("griewank_data.csv");
    outFile << "x,y,z\n";

    // Domain is [-600, 600]. 
    // For visualization, I'll zoom in to [-50, 50] to see the ripples clearly.
    double step = 1.0;
    for (double x = -50.0; x <= 50.0; x += step) {
        for (double y = -50.0; y <= 50.0; y += step) {
            outFile << x << "," << y << "," << griewank(x, y) << "\n";
        }
    }

    outFile.close();
    std::cout << "Data generated in griewank_data.csv" << std::endl;
    return 0;
}