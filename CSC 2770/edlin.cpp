#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <unistd.h>

// Function prototype for the filter function
void filter(std::vector<std::string> &lines, const std::string &user_cmd);

int main() 
{
    std::vector<std::string> lines; // Vector to store the lines
    std::cout << "Line Editor" << std::endl; // Print welcome message
    std::string command; // Variable to store the user's command

    while (true) // Main loop to keep accepting user commands
    {
        std::cout << "edlin> ";
        std::getline(std::cin, command); // Get user command

        if (command == "q") // Quit command
        {
            std::cout << "Exiting the editor" << std::endl;
            return 0;
        } 
        else if (command == "l") // List command: displays the lines in memory
        {
            for (size_t i = 0; i < lines.size(); ++i) // Iterate through the lines
            {
                std::cout << i << ": " << lines[i] << std::endl; // Print each line
            }
        } 
        else if (command.substr(0, 2) == "r ") // Read command: reads a file
        {
            std::string filename = command.substr(2); // Get the filename from the command
            std::ifstream file(filename); // Open the file

            if (file.is_open()) // Check if the file is open
            {
                lines.clear(); // Clear the lines vector
                std::string line; // Variable to store each line
                while (std::getline(file, line)) // Read the file line by line
                {
                    lines.push_back(line); // Add the line to the lines vector
                }
                file.close(); // Close the file
            } 
            else // Error handling
            {
                std::cerr << "Error: Cannot open file " << filename << std::endl;
            }
        } 
        else if (command.substr(0, 2) == "s ") // Save command: writes the lines to a file
        {
            std::string filename = command.substr(2); // Get the filename from the command
            std::ofstream file(filename); // Open the file

            if (file.is_open()) // Check if the file is open
            {
                for (const auto &line : lines) // Iterate through the lines
                {
                    file << line << std::endl; // Write each line to the file
                }
                file.close(); // Close the file
            } 
            else // Error handling
            {
                std::cerr << "Error: Cannot open file " << filename << std::endl;
            }
        } 
        else if (command.substr(0, 2) == "e ") // Edit command: edits a specific line or appends a new line
        {
            std::stringstream ss(command.substr(2)); // Create a stringstream for parsing
            size_t line_number; // Variable to store the line number
            std::string new_line; // Variable to store the new line

            if (ss >> line_number && std::getline(ss, new_line)) // Parse the line number and the new line
            {
                if (new_line.size() > 0 && new_line[0] == ' ') // Remove leading space if present
                {
                                    new_line = new_line.substr(1);
                }
                if (line_number < lines.size()) // If the line number is within the existing lines
                {
                    lines[line_number] = new_line; // Replace the line with the new content
                } 
                else // If the line number is beyond the existing lines
                {
                    lines.push_back(new_line); // Add the new line to the end
                }
            } 
            else // Error handling
            {
                std::cerr << "Error: Invalid input format" << std::endl;
            }
        } 
        else if (command[0] == '!') // Filter command: processes a line using a shell command
        {
            filter(lines, command.substr(1)); // Call the filter function with the user command
        } 
        else // Error handling for invalid commands
        {
            std::cerr << "Error: Invalid command" << std::endl;
        }
    }
}

// Function to filter a line through a shell command
void filter(std::vector<std::string> &lines, const std::string &user_cmd) 
{
    std::stringstream ss(user_cmd); // Create a stringstream for parsing
    size_t lno; // Variable to store the line number
    std::string ucmd; // Variable to store the shell command

    if (!(ss >> lno) || !std::getline(ss, ucmd) || ucmd.empty()) // Parse the line number and the shell command
    {
        std::cerr << "Error: Invalid filter command format" << std::endl;
        return;
    }

    ucmd = ucmd.substr(1); // Remove the leading space
    if (lno >= lines.size()) // Check if the line number is within the existing lines
    {
        std::cerr << "Error: Line number out of range" << std::endl;
        return;
    }

    int from_parent[2], to_parent[2]; // Create pipes
    if (pipe(from_parent) == -1 || pipe(to_parent) == -1) // Error handling for pipe creation
    {
        std::cerr << "Error: Failed to create pipes" << std::endl;
        return;
    }

    pid_t pid = fork(); // Fork the process
    if (pid == -1) // Error handling for fork failure
    {
        std::cerr << "Error: Failed to fork" << std::endl;
        return;
    }

    if (pid == 0) // Child process
    {    
        dup2(from_parent[0], STDIN_FILENO); // Redirect standard input
        dup2(to_parent[1], STDOUT_FILENO); // Redirect standard output
        close(from_parent[1]);
        close(to_parent[0]);

        const char *args[] = {"/bin/sh", "-c", ucmd.c_str(), nullptr}; // Create an argument list for execve
        execve(args[0], (char *const *) args, nullptr); // Execute the shell command
        std::cerr << "Error: Failed to execute command" << std::endl; // Error handling for execve failure
        exit(1);
    } 
    else // Parent process
    { 
        close(from_parent[0]); // Close unused pipe ends
        close(to_parent[1]);

        FILE *fp_write = fdopen(from_parent[1], "w"); // Open a FILE pointer for writing
        if (fp_write) // Check
        {       
            fprintf(fp_write, "%s\n", lines[lno].c_str()); // Write the line to the pipe
            fclose(fp_write); // Close the FILE pointer
        }

        FILE *fp_read = fdopen(to_parent[0], "r"); // Open a FILE pointer for reading
        if (fp_read) // Check if the FILE pointer is valid
        {
            char buf[BUFSIZ]; // Buffer to store the output from the command
            if (fgets(buf, sizeof(buf), fp_read)) // Read the output from the command
            {
                lines[lno] = buf; // Replace the line with the command output
                lines[lno].pop_back(); // Remove the newline character
            }
            fclose(fp_read); // Close the FILE pointer
        }

        waitpid(pid, nullptr, 0); // Wait for the child process to terminate
    }
}