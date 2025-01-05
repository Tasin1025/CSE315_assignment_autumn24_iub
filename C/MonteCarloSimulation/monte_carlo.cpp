#include <iostream> // Include input-output stream for printing
#include <thread>   // Include thread library for multithreading
#include <vector>   // Include vector library to store threads
#include <random>   // Include random library for random number generation
#include <mutex>    // Include mutex for thread synchronization

std::mutex mtx;       // Mutex object for thread-safe access to shared resource
double totalArea = 0; // Variable to store the total area calculated by all threads

// Function representing the mathematical function to approximate (π in this case)
double f(double x)
{
    return 4.0 / (1.0 + x * x); // Example function: y = 4 / (1 + x^2)
}

// Function that performs Monte Carlo simulation on a range of points
void monteCarlo(int numPoints, double xMin, double xMax, double yMax)
{
    std::random_device rd;                              // Random device to seed the random number generator
    std::mt19937 gen(rd());                             // Mersenne Twister generator for high-quality randomness
    std::uniform_real_distribution<> xDist(xMin, xMax); // Distribution for random x values within the range
    std::uniform_real_distribution<> yDist(0, yMax);    // Distribution for random y values between 0 and yMax

    int underCurve = 0; // Counter for points that fall under the curve

    // Loop through the number of points to generate and check how many fall under the curve
    for (int i = 0; i < numPoints; i++)
    {
        double x = xDist(gen); // Generate a random x value
        double y = yDist(gen); // Generate a random y value

        // Check if the point (x, y) is under the curve
        if (y <= f(x))
        {
            underCurve++; // Increment the counter if the point is under the curve
        }
    }

    // Calculate the area estimate for this thread and scale it to the full range
    double threadArea = (static_cast<double>(underCurve) / numPoints) * (xMax - xMin) * yMax;

    // Lock the mutex to safely update the shared 'totalArea' variable from multiple threads
    std::lock_guard<std::mutex> lock(mtx);
    totalArea += threadArea; // Add the thread's result to the total area
}

int main(int argc, char *argv[])
{
    // Check if the correct number of arguments were provided
    if (argc != 5)
    {
        std::cerr << "Usage: ./monte_carlo <x_lowerbound> <x_upperbound> <max_y> <num_points>\n";
        return 1; // Return error code if the arguments are incorrect
    }

    // Parse command-line arguments
    double xMin = std::stod(argv[1]);   // Convert the first argument to a double (x lower bound)
    double xMax = std::stod(argv[2]);   // Convert the second argument to a double (x upper bound)
    double yMax = std::stod(argv[3]);   // Convert the third argument to a double (maximum y value)
    int numPoints = std::stoi(argv[4]); // Convert the fourth argument to an integer (number of points)

    int pointsPerThread = numPoints / 8; // Divide the total number of points by the number of threads (8 in this case)
    std::vector<std::thread> threads;    // Vector to hold the thread objects

    // Launch 8 threads, each executing the 'monteCarlo' function with pointsPerThread points
    for (int i = 0; i < 8; i++)
    {
        threads.emplace_back(monteCarlo, pointsPerThread, xMin, xMax, yMax); // Create a thread and add it to the vector
    }

    // Wait for all threads to finish before proceeding
    for (auto &t : threads)
    {
        t.join(); // Join each thread (wait for it to complete)
    }

    // Print the estimated area calculated by all threads
    std::cout << "Estimated Area: " << totalArea << "\n";

    return 0; // Return 0 to indicate successful program execution
}
