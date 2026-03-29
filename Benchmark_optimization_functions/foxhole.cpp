#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

double shekel(double x1, double x2) {
    // The constant matrix a_ij for the 25 foxholes
    double a[2][25];
    int count = 0;
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            a[0][count] = 16.0 * j;
            a[1][count] = 16.0 * i;
            count++;
        }
    }

    double sum = 0.0;
    for (int j = 0; j < 25; j++) {
        double inner_sum = std::pow(x1 - a[0][j], 6) + std::pow(x2 - a[1][j], 6);
        sum += 1.0 / ((j + 1) + inner_sum);
    }

    return 1.0 / (0.002 + sum);
}

int main() {
    std::ofstream outFile("shekel_data.csv");
    outFile << "x,y,z\n";

    // Domain is [-65.536, 65.536]
    double step = 1.0;
    for (double x = -65.536; x <= 65.536; x += step) {
        for (double y = -65.536; y <= 65.536; y += step) {
            outFile << x << "," << y << "," << shekel(x, y) << "\n";
        }
    }

    outFile.close();
    std::cout << "Data generated in shekel_data.csv" << std::endl;
    return 0;
}