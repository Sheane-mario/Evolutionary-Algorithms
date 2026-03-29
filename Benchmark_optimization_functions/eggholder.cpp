#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

// Implementation of the Eggholder Function (2D)
double eggholder(double x1, double x2) {
    double term1 = -(x2 + 47.0) * std::sin(std::sqrt(std::abs(x2 + x1 / 2.0 + 47.0)));
    double term2 = -x1 * std::sin(std::sqrt(std::abs(x1 - (x2 + 47.0))));
    return term1 + term2;
}

int main() {
    std::ofstream outFile("eggholder_data.csv");
    outFile << "x,y,z\n";

    // Domain is [-512, 512]. 
    // We use a step of 4.0 to get an overview of the landscape.
    double step = 4.0;
    for (double x = -512.0; x <= 512.0; x += step) {
        for (double y = -512.0; y <= 512.0; y += step) {
            outFile << x << "," << y << "," << eggholder(x, y) << "\n";
        }
    }

    outFile.close();
    std::cout << "Data generated in eggholder_data.csv" << std::endl;
    return 0;
}