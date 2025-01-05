#include <iostream>           // Include input-output stream for printing
#include <queue>              // Include queue container for the shared buffer
#include <thread>             // Include thread library for multithreading
#include <mutex>              // Include mutex for thread synchronization
#include <condition_variable> // Include condition variable for blocking threads
#include <random>             // Include random library for generating random numbers
#include <chrono>             // Include chrono library for time-based operations

#define BUFFER_SIZE 5 // Define the buffer size (maximum items in the queue)

std::queue<int> buffer;                    // Shared queue to act as the buffer
std::mutex mtx;                            // Mutex to protect access to the shared buffer
std::condition_variable notEmpty, notFull; // Condition variables to handle buffer state

// Producer function that produces items and adds them to the buffer
void producer(int id)
{
    std::random_device rd;                        // Random device to seed the random number generator
    std::mt19937 gen(rd());                       // Mersenne Twister random number generator
    std::uniform_int_distribution<> dist(1, 100); // Distribution for generating random items (1 to 100)

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Simulate work by sleeping for 1 second
        int item = dist(gen);                                         // Generate a random item to produce

        std::unique_lock<std::mutex> lock(mtx); // Lock the mutex to access the buffer

        // Wait until the buffer is not full
        notFull.wait(lock, []
                     { return buffer.size() < BUFFER_SIZE; });

        buffer.push(item);                                               // Add the item to the buffer
        std::cout << "Producer " << id << " produced: " << item << "\n"; // Print the produced item

        notEmpty.notify_one(); // Notify one consumer that the buffer is no longer empty
    }
}

// Consumer function that consumes items from the buffer
void consumer(int id)
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // Simulate work by sleeping for 1.5 seconds

        std::unique_lock<std::mutex> lock(mtx); // Lock the mutex to access the buffer

        // Wait until the buffer is not empty
        notEmpty.wait(lock, []
                      { return !buffer.empty(); });

        int item = buffer.front();                                       // Get the item from the front of the buffer
        buffer.pop();                                                    // Remove the item from the buffer
        std::cout << "Consumer " << id << " consumed: " << item << "\n"; // Print the consumed item

        notFull.notify_one(); // Notify one producer that the buffer is no longer full
    }
}

int main()
{
    // Create arrays of producer and consumer threads
    std::thread producers[2], consumers[2];

    // Launch producer threads
    for (int i = 0; i < 2; i++)
    {
        producers[i] = std::thread(producer, i + 1); // Pass the producer ID (1 and 2)
    }

    // Launch consumer threads
    for (int i = 0; i < 2; i++)
    {
        consumers[i] = std::thread(consumer, i + 1); // Pass the consumer ID (1 and 2)
    }

    // Wait for all producer and consumer threads to finish
    for (auto &t : producers)
        t.join(); // Join producer threads
    for (auto &t : consumers)
        t.join(); // Join consumer threads

    return 0; // Return 0 to indicate successful execution
}
