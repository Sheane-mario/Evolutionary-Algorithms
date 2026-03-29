#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

const double LOWER_BOUND = -5.12;
const double UPPER_BOUND = 5.12;

double sphere(double x, double y) {
    return (x * x) + (y * y);
}

int main() {

    // Generate a grid of points for export for plotting.
    std::ofstream outFile("sphere_data.csv");
    outFile << "x,y,z\n"; // CSV header

    double step = 0.2;
    for (double x = LOWER_BOUND; x <= UPPER_BOUND; x += step) {
        for (double y = LOWER_BOUND; y <= UPPER_BOUND; y += step) {
            double z = sphere(x, y);
            outFile << x << "," << y << "," << z << "\n";
        }
    }

    outFile.close();
    std::cout << "Data generated in sphere_data.csv" << "\n";
    return 0;
}