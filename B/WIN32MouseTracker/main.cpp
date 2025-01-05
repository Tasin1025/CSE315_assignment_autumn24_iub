#include <windows.h> // Include Windows API functions
#include <iostream>  // Include input-output stream for printing

int main()
{
    POINT cursorPos;                                                 // Declare a variable to store the mouse cursor position
    std::cout << "Tracking mouse position. Press Ctrl+C to exit.\n"; // Output a message to the user

    while (true)
    { // Infinite loop to continuously track the cursor position
        // Get the current position of the mouse cursor
        if (GetCursorPos(&cursorPos))
        {
            // If successful, print the X and Y coordinates of the cursor
            std::cout << "X: " << cursorPos.x << ", Y: " << cursorPos.y << "\r";
            // '\r' is used to overwrite the previous output on the same line
        }
        else
        {
            // If failed to get the cursor position, output an error message
            std::cerr << "Failed to get cursor position.\n";
        }
        Sleep(100); // Pause for 100 milliseconds before checking the position again
    }

    return 0; // Return 0 to indicate successful program execution
}
