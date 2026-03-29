#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

const int DIMENTIONS = 6;
const int MAX_RESTARTS = 100;
const int STAGNATION_LIMIT = 50;
const double LOWER_BOUND = -5.12;
const double UPPER_BOUND = 5.12;
const double STEP_SIZE = 0.01;


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dist(LOWER_BOUND, UPPER_BOUND);
std::uniform_real_distribution<double> random_mutation(-STEP_SIZE, STEP_SIZE);
std::uniform_real_distribution<double> probability_distribution(0.0, 1.0);
std::uniform_int_distribution<int> int_dist(1, DIMENTIONS - 1);

struct Individual
{
    std::vector<double> features;
    double fitness;
};

// calculates fitness of feature vector
double fitness_measure(const std::vector<double>& x) {
    double s = 0;
    for (auto i : x) {
        s += i * i;
    }
    // sphere function was used to calculate the cost, but we need the fitness
    return -s;
}

Individual steepest_ascent() {
    Individual x0;
    x0.features.resize(DIMENTIONS);
    for (int i = 0; i < DIMENTIONS; i++) {
        x0.features[i] = dist(gen);
    }
    x0.fitness = fitness_measure(x0.features);

    bool stopped = false;
    int generation_count = 0;

    while (!stopped) {
        std::vector<Individual> neighbours;
        neighbours.push_back(x0);

        // for each solution  feature q=1....n
        for (int q = 0; q < DIMENTIONS; q++) {
            Individual xq = x0;

            // Randomly mutate the q-th feature 
            xq.features[q] += random_mutation(gen);
            // out-of-boundary check
            xq.features[q] = std::max(LOWER_BOUND, std::min(UPPER_BOUND, xq.features[q]));

            // calculate the fitness
            xq.fitness = fitness_measure(xq.features);

            neighbours.push_back(xq);
        }

        auto x_p = max_element(neighbours.begin(), neighbours.end(), [](const Individual& a, const Individual& b)
            { return a.fitness < b.fitness; });

        if (x_p->fitness <= x0.fitness) {
            stopped = true;
        }
        else {
            x0 = *x_p;
        }

        std::cout << "Gen: " << ++generation_count << " | Best fitness: " << x0.fitness << "\n";
    }
    return x0;
}

Individual next_ascent() {
    Individual x0;
    x0.features.resize(DIMENTIONS);
    for (int i = 0; i < DIMENTIONS; i++) {
        x0.features[i] = dist(gen);
    }
    x0.fitness = fitness_measure(x0.features);

    int generation_count = 0;
    bool improving = true;

    while (improving) {
        improving = false;

        for (int q = 0; q < DIMENTIONS; q++) {
            Individual xq = x0;
            xq.features[q] += random_mutation(gen);
            xq.features[q] = std::max(LOWER_BOUND, std::min(UPPER_BOUND, xq.features[q]));
            xq.fitness = fitness_measure(xq.features);
            // Follow a greedy approach, if just after q-th feature being mutated, fitness is greater, reject the existing individual and move to the new individual with feature being mutated.
            if (xq.fitness > x0.fitness) {
                x0 = xq;
                improving = true;
                break;
            }
        }
        std::cout << "Gen: " << ++generation_count << " | Best fitness: " << x0.fitness << "\n";
    }

    return x0;
}

Individual random_mutation_hill_climbing() {
    Individual x0;
    x0.features.resize(DIMENTIONS);
    for (int i = 0; i < DIMENTIONS; i++) {
        x0.features[i] = dist(gen);
    }
    x0.fitness = fitness_measure(x0.features);

    const int MAX_ITERATIONS = 10000;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        int q = int_dist(gen);

        Individual x1 = x0;
        x1.features[q] += random_mutation(gen);
        x1.features[q] = std::max(LOWER_BOUND, std::min(UPPER_BOUND, x1.features[q]));
        x1.fitness = fitness_measure(x1.features);

        // This is a stochastic search of making x0 reaching the global optima
        if (x1.fitness > x0.fitness) {
            x0 = x1;
        }
        std::cout << "Gen: " << i + 1 << " | Best fitness: " << x0.fitness << "\n";
    }
    return x0;
}

Individual adaptive_hill_climbing() {
    Individual x0;
    x0.features.resize(DIMENTIONS);
    for (int i = 0; i < DIMENTIONS; i++) {
        x0.features[i] = dist(gen);
    }
    x0.fitness = fitness_measure(x0.features);

    // Adaptive Parameters
    const double pm = 0.1; // 10% mutation probability
    double current_step = STEP_SIZE;
    const double adapt_factor = 0.99; // Slowly shrink search space on failure, expand on success
    const int MAX_ITERATIONS = 10000;

    for (int i = 0; i < MAX_ITERATIONS; i++) {
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

        x1.fitness = fitness_measure(x1.features);
        if (x1.fitness > x0.fitness) {
            x0 = x1;
            current_step /= adapt_factor; // Increase the step size, to make the search faster.
        }
        else {
            current_step *= adapt_factor; // Slow down and shrink the search space.
        }
        std::cout << "Gen: " << i + 1 << " | Best fitness: " << x0.fitness << "\n";
    }
    return x0;
}

int main() {

    Individual peak = adaptive_hill_climbing();
    std::cout << peak.fitness << "\n";

    // Individual peak = random_mutation_hill_climbing();
    // std::cout << peak.fitness << "\n";


    // Individual steepest_ascent_global_optima;
    // steepest_ascent_global_optima.fitness = -std::numeric_limits<double>::infinity();

    // int stagnation_count = 0;

    // for (int i = 0; i < MAX_RESTARTS; i++) {
    //     Individual local_optima = steepest_ascent();
    //     if (local_optima.fitness > steepest_ascent_global_optima.fitness) {
    //         steepest_ascent_global_optima = local_optima;
    //         stagnation_count = 0;
    //     }
    //     else {
    //         stagnation_count++;
    //     }

    //     // If number of local optinums have passed STAGNATION_LIMIT without finding a better solution than global optimum, it is more likely that algorithm saturated. 
    //     if (stagnation_count >= STAGNATION_LIMIT) {
    //         break;
    //     }
    // }

    // std::cout << "Steepest Ascent Hill Climbing Optimization finished !\n";
    // std::cout << "Best solution: ";
    // for (auto u : steepest_ascent_global_optima.features) std::cout << u << " ";
    // std::cout << std::endl;
    // std::cout << "Best fitness: ";
    // std::cout << steepest_ascent_global_optima.fitness << std::endl;

    // Individual next_ascent_global_optima;
    // next_ascent_global_optima.fitness = -std::numeric_limits<double>::infinity();

    // stagnation_count = 0;

    // for (int i = 0; i < MAX_RESTARTS; i++) {
    //     Individual local_optima = next_ascent();
    //     if (local_optima.fitness > next_ascent_global_optima.fitness) {
    //         next_ascent_global_optima = local_optima;
    //         stagnation_count = 0;
    //     }
    //     else {
    //         stagnation_count++;
    //     }
    //     if (stagnation_count >= STAGNATION_LIMIT) {
    //         break;
    //     }
    // }

    // std::cout << "Next Ascent Hill Climbing Optimization finished !\n";
    // std::cout << "Best solution: ";
    // for (auto u : next_ascent_global_optima.features) std::cout << u << " ";
    // std::cout << std::endl;
    // std::cout << "Best fitness: ";
    // std::cout << next_ascent_global_optima.fitness << std::endl;


    return 0;
}