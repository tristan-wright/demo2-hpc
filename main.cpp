#include <iostream>
#include <cstdlib>
#include <random>
#include <fstream>
#include <chrono>
#include <iomanip>

#include "sim_serial.h"
#include "sim_omp.h"
#include "sim_cuda.h"
#include "surface.h"

/**
 * Initialisation of clocks to measure the runtime of the different
 * parallelization techniques.
 * @param lattice The Surface object containing the lattice configuration and functions.
 * @return The EXIT_STATUS of the simulation.
 */
int initialise(Surface lattice) {
    lattice.clear();
    lattice.save();
    auto StartTime = std::chrono::high_resolution_clock::now();
    int status = simulate(lattice);
    auto FinishTime = std::chrono::high_resolution_clock::now();
    lattice.complete = true;
    lattice.save();
    auto TotalTime = std::chrono::duration_cast<std::chrono::microseconds>(FinishTime - StartTime);
    cout << lattice.name << ":" << endl;
    cout << "Total time: " << std::setw(12) << TotalTime.count() << " us" << endl;
    return status;
}

/**
 * Main function of the application. Handles boundry and error checking.
 * Will initiate the simulation if everything is correct.
 * @param argc The number of arguments.
 * @param argv The string representation array of the arguments.
 * @return The EXIT_STATUS of the simulation.
 */
int main(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
        std::cout << "Usage: ./ising n size temperature {output}" << std::endl;
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        std::cout << "'n' has to be a positive integer" << std::endl;
        return EXIT_FAILURE;
    }

    Surface lattice(argv[0], n, atoi(argv[2]), strtod(argv[3], nullptr));

    if (argc == 5) {
        lattice.out = true;
        lattice.outName = argv[4];
    }

    return initialise(lattice);
}
