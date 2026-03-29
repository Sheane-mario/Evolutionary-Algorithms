#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

// Implementation of the Sine Envelope (Schaffer) Function (2D)
double sineEnvelope(double x, double y) {
    double numerator = std::pow(std::sin(std::sqrt(x * x + y * y)), 2) - 0.5;
    double denominator = std::pow(0.001 * (x * x + y * y) + 1.0, 2);
    return 0.5 + (numerator / denominator); // Note: f(0,0) = 0
}

int main() {
    std::ofstream outFile("sine_envelope_data.csv");
    outFile << "x,y,z\n";

    // Domain is [-100, 100]. 
    // We use a step of 1.0 to get a good overview of the ripples.
    double step = 1.0;
    for (double x = -100.0; x <= 100.0; x += step) {
        for (double y = -100.0; y <= 100.0; y += step) {
            outFile << x << "," << y << "," << sineEnvelope(x, y) << "\n";
        }
    }

    outFile.close();
    std::cout << "Data generated in sine_envelope_data.csv" << std::endl;
    return 0;
}