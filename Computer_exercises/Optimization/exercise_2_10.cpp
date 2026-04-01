#include <iostream>
#include <fstream>
#include <vector>

int f1(int x1, int x2) {
    return x1 * x1 + x2;
}

int f2(int x1, int x2) {
    return x1 + x2 * x2;
}

int main() {
    std::ofstream outFile("exercise_2_10_data.csv");
    outFile << "x1,x2,f1,f2\n";

    for (int x1 = -10; x1 <= 10; x1++) {
        for (int x2 = -10; x2 <= 10; x2++) {
            outFile << x1 << "," << x2 << "," << f1(x1, x2) << "," << f2(x1, x2) << "\n";
        }
    }

    outFile.close();
    std::cout << "Data generated for exercise 2.10 \n";
    return 0;
}