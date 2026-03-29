#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <random>

const double PI = 3.14159265358979323846;

// Parameters for the Fletcher function
double a[2][2], b[2][2], alpha[2];

void initializeParameters() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist_ab(-100.0, 100.0);
    std::uniform_real_distribution<double> dist_alpha(-PI, PI);

    for (int i = 0; i < 2; ++i) {
        alpha[i] = dist_alpha(gen);
        for (int j = 0; j < 2; ++j) {
            a[i][j] = dist_ab(gen);
            b[i][j] = dist_ab(gen);
        }
    }
}

double fletcher(double x1, double x2) {
    double x[2] = { x1, x2 };
    double sum_outer = 0;
    for (int i = 0; i < 2; ++i) {
        double Ai = 0, Bi = 0;
        for (int j = 0; j < 2; ++j) {
            Ai += a[i][j] * std::sin(alpha[j]) + b[i][j] * std::cos(alpha[j]);
            Bi += a[i][j] * std::sin(x[j]) + b[i][j] * std::cos(x[j]);
        }
        sum_outer += std::pow(Ai - Bi, 2);
    }
    return sum_outer;
}

int main() {
    initializeParameters();
    std::ofstream outFile("fletcher_data.csv");
    outFile << "x,y,z\n";

    double step = 0.1;
    for (double x = -PI; x <= PI; x += step) {
        for (double y = -PI; y <= PI; y += step) {
            outFile << x << "," << y << "," << fletcher(x, y) << "\n";
        }
    }
    outFile.close();
    std::cout << "Data generated in fletcher_data.csv" << std::endl;
    return 0;
}