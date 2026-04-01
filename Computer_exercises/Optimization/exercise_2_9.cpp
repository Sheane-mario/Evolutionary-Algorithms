#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>

double f(double x) {
    return x * x * x + 4 * x * x - 4 * x + 1;
}

int main() {
    std::ofstream outFile("exercise_2_9_data.csv");
    outFile << "x,y\n";

    double step = 0.001;
    for (double x = -5; x <= 3; x += step) {
        double y = f(x);
        outFile << x << "," << y << "\n";
    }

    outFile.close();
    std::cout << "Data generated for exercise 2.9";
    return 0;
}