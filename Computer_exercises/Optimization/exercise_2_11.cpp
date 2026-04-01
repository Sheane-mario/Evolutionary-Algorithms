// Monte-carlo simulations on Adaptive hill climbing algorithm 
// Optimization on Ackley function
#include <iostream>
#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <fstream>

const int DIMENTIONS = 10;
const int GENERATION_SIZE = 10000;
const int MONTE_CARLO_TRIALS = 100;
const double LOWER_BOUND = -30.0;
const double UPPER_BOUND = 30.0;
const int STEP_SIZE = 0.5;
const double PI = 3.14159265358979323846;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dist(LOWER_BOUND, UPPER_BOUND);
std::uniform_real_distribution<double> random_mutation(-STEP_SIZE, STEP_SIZE);
std::uniform_real_distribution<double> probability_distribution(0.0, 1.0);

struct Individual
{
    std::vector<double> features;
    double fitness;
};

double ackley(const std::vector<double>& x) {
    int n = x.size();
    double squared_sum = 0.0, cos_summation = 0.0;
    for (double y : x) {
        squared_sum += y * y;
    }
    for (double y : x) {
        cos_summation += std::cos(2.0 * PI * y);
    }
    double t1 = -20.0 * std::exp(-0.2 * std::sqrt((squared_sum) / (double)n));
    double t2 = -std::exp(cos_summation / (double)n);
    double cost = 20.0 + std::exp(1.0) + t1 + t2;
    return -cost;
}

Individual adaptive_hill_climbing(double pm) {
    Individual x0;
    x0.features.resize(DIMENTIONS);
    for (int i = 0; i < DIMENTIONS; i++) {
        x0.features[i] = dist(gen);
    }
    x0.fitness = ackley(x0.features);

    double current_step = STEP_SIZE;
    const double adapt_factor = 0.99;

    for (int i = 0; i < GENERATION_SIZE; i++) {
        Individual x1 = x0;
        bool is_mutated = false;
        for (int q = 0; q < DIMENTIONS; q++) {
            double r = probability_distribution(gen);
            if (r < pm) {
                x1.features[q] += random_mutation(gen);
                x1.features[q] = std::max(LOWER_BOUND, std::min(UPPER_BOUND, x1.features[q]));
                is_mutated = true;
            }
        }
        if (!is_mutated) continue;

        x1.fitness = ackley(x1.features);
        if (x1.fitness > x0.fitness) {
            x0 = x1;
            current_step /= adapt_factor;
        }
        else {
            current_step *= adapt_factor;
        }
    }
    return x0;
}

int main() {
    std::ofstream outfile("exercise_2_11_data.csv");
    outfile << "pm,avg_fitness\n";

    for (int k = 1; k <= 10; k++) {
        double pm = k / 10.0;
        double total_fitness = 0.0;
        for (int mcs = 0; mcs < MONTE_CARLO_TRIALS; mcs++) {
            Individual local_peak = adaptive_hill_climbing(pm);
            total_fitness += local_peak.fitness;
        }
        double avg_fitness = total_fitness / MONTE_CARLO_TRIALS;
        outfile << pm << "," << avg_fitness << "\n";
    }

    outfile.close();
    std::cout << "Data generated for exercise 2.11\n";
}