#include <iostream>     // For input/output operations
#include <iomanip>      // For output formatting
#include <vector>       // For storing simulation results in a table, somewhat like a DataFrame
#include <random>       // For generating random numbers
#include <chrono>       // For measuring time
#include <cmath>        // For mathematical operations like abs()

int main() {
    // Define actual value of Pi
    const double ACTUAL_PI = 3.14159265359;

    // A struct (like a Python dict or mini-class) to hold each row of results
    struct ResultRow {
        std::string samples;    // e.g., "10^2"
        double est_pi;          // estimated value of Pi
        double error;           // error %
        double run_time;        // elapsed time in seconds
    };

    // A vector to hold all results, similar to a table or DataFrame
    std::vector<ResultRow> results;

    // Set up random number generation (Mersenne Twister engine, very standard)
    std::mt19937 rng(std::random_device{}());   // Initialize random engine
    std::uniform_real_distribution<double> dist(-1.0, 1.0); // Uniform distribution from -1 to 1

    // Loop over i = 2 to 8 (i inclusive), which means from 10^2 to 10^8 samples
    for (int i = 2; i <= 8; ++i) {
        // Measure starting time, like 'time.time()' in Python
        auto start = std::chrono::high_resolution_clock::now();

        // Calculate how many samples for this run (e.g., 10^2, 10^3, ..., 10^8)
        unsigned long long sample_count = static_cast<unsigned long long>(pow(10, i));

        unsigned long long samples_in_circle = 0;  // Points inside the unit circle

        // Loop over all samples
        for (unsigned long long sample_num = 0; sample_num < sample_count; ++sample_num) {
            // Generate a random point (x, y) where both x and y are in [0,1]
            double x = dist(rng);
            double y = dist(rng);

            // Check if the point is inside the unit circle (x^2 + y^2 <= 1)
            if ((x*x + y*y) <= 1.0) {
                ++samples_in_circle;
            }
        }

        // Measure ending time
        auto end = std::chrono::high_resolution_clock::now();

        // Calculate elapsed time in seconds
        std::chrono::duration<double> elapsed = end - start;
        double run_time = elapsed.count();

        // The Monte Carlo estimate of Pi: (points inside circle / total points) * 4
        double est_pi = 4.0 * static_cast<double>(samples_in_circle) / static_cast<double>(sample_count);

        // Error as a percentage
        double error = std::abs((est_pi - ACTUAL_PI) / ACTUAL_PI) * 100;

        // Prepare the results for this run
        ResultRow row;
        row.samples = "10^" + std::to_string(i);
        row.est_pi = est_pi;
        row.error = error;
        row.run_time = run_time;

        // Store the results in the vector
        results.push_back(row);

        // Print progress, similar to 'print(f"done with i={i}")'
        std::cout << "Done with i = " << i << std::endl;
    }

    // Now, print the table of results, similar to displaying the DataFrame
    std::cout << std::fixed << std::setprecision(8); // Format output nicely

    std::cout << "\nSamples   | Est. Pi      | Error (%)   | Run Time (s)\n";
    std::cout << "-------------------------------------------------------\n";
    for (const auto& row : results) {
        std::cout << std::setw(9)  << row.samples << " | "
                  << std::setw(12) << row.est_pi << " | "
                  << std::setw(11) << row.error << " | "
                  << std::setw(12) << row.run_time << std::endl;
    }
    return 0; // Program finished successfully
}