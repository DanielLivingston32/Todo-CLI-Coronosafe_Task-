#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <iomanip>

#include <vector>
#include <string.h>

// Prints the commands and usage
void printHelp()
{
    std::cout << "Usage :-\n"
                 "$ ./todo add \"todo item\"  # Add a new todo\n"
                 "$ ./todo ls               # Show remaining todos\n"
                 "$ ./todo del NUMBER       # Delete a todo\n"
                 "$ ./todo done NUMBER      # Complete a todo\n"
                 "$ ./todo help             # Show usage\n"
                 "$ ./todo report           # Statistics"
              << std::endl;
}

// Prints the todoList in the current directory
void showList(char todoPath[])
{
    std::ifstream todoFile(todoPath);

    std::string line;
    std::vector<std::string> lines;

    if (todoFile)
    {
        // Reads text file line by line into a string vector
        while (getline(todoFile, line))
        {
            lines.push_back(line);
        }
        int noOfTodo = lines.size();

        // Prints in given format
        for (auto lineItr : lines)
        {
            std::cout << "[" << noOfTodo-- << "] " << lineItr << std::endl;
        }
    }
    else
    {
        std::cout << "There are no pending todos!" << std::endl;
    }
    todoFile.close();
}

// Adds Todo to the list
void addTodo(std::string newTodo, char todoPath[], char outTodoPath[])
{
    std::ifstream readTodoFile(todoPath);
    std::ofstream writeTodoFile(outTodoPath);

    std::string line;

    if (readTodoFile)
    {
        // Writes the new task and the previous ones to the ofstream object.
        writeTodoFile << newTodo + "\n";
        while (getline(readTodoFile, line))
        {
            writeTodoFile << line + "\n";
        }

        readTodoFile.close();
        std::remove(todoPath);
    }
    else
    {
        // Writes the new task to the ofstream object
        writeTodoFile << newTodo + "\n";
        readTodoFile.close();
    }

    writeTodoFile.close();
    std::rename(outTodoPath, todoPath);

    std::cout << "Added todo: \"" + newTodo + "\"" << std::endl;
}

// Delete a Todo from the list
void deleteTodo(int deleteNo, char todoPath[], char outTodoPath[])
{
    std::ifstream readTodoFile(todoPath);
    std::ofstream writeTodoFile(outTodoPath);

    std::string line;
    std::vector<std::string> lines;

    if (readTodoFile)
    {
        // Reads text file line by line into a string vector
        while (getline(readTodoFile, line))
        {
            lines.push_back(line);
        }
        int noOfTodo = lines.size();

        // Checks if user input is valid
        if (deleteNo > noOfTodo || deleteNo <= 0)
        {
            std::cout << "Error: todo #" << deleteNo << " does not exist. Nothing deleted." << std::endl;
            return;
        }

        // Removes completed todo
        const int deleteIdx = noOfTodo - deleteNo;
        std::string toRemove = lines[deleteIdx];
        std::cout << "Deleted todo #" << deleteNo << std::endl;
        lines.erase(lines.begin() + deleteIdx);

        // Writes the updated list to the ofstream object
        for (auto lineItr : lines)
        {
            writeTodoFile << lineItr + "\n";
        }

        readTodoFile.close();
        std::remove(todoPath);
        writeTodoFile.close();
        std::rename(outTodoPath, todoPath);
    }
    else
    {
        std::cout << "Error: todo #" << deleteNo << " does not exist. Nothing deleted." << std::endl;
        return;
    }
}

// Helper Function which writes the completed task to done.txt
void writeCompleted(std::string taskToAdd, char donePath[], char outDonePath[], char currentDate[])
{
    std::ifstream readDoneFile(donePath);
    std::ofstream writeDoneFile(outDonePath);

    std::string line;

    std::string addString(currentDate);
    addString = "x " + addString + " " + taskToAdd + "\n";

    if (readDoneFile)
    {
        // Writes the completed task and the previously completed task to the ofstream object
        writeDoneFile << addString;
        while (getline(readDoneFile, line))
        {
            writeDoneFile << line + "\n";
        }

        readDoneFile.close();
        std::remove(donePath);
    }
    else
    {
        // Writes the completed task to the ofstream object
        writeDoneFile << addString;
        readDoneFile.close();
    }

    writeDoneFile.close();
    std::rename(outDonePath, donePath);
}

// Marks a todo completed
void markComplete(int completedNo, char todoPath[], char outTodoPath[], char donePath[], char outDonePath[], char currentDate[])
{
    std::ifstream readTodoFile(todoPath);
    std::ofstream writeTodoFile(outTodoPath);

    std::string line;
    std::vector<std::string> lines;

    if (readTodoFile)
    {
        // Reads text file line by line into a string vector
        while (getline(readTodoFile, line))
        {
            lines.push_back(line);
        }
        int noOfTodo = lines.size();

        // Checks if user input is valid
        if (completedNo > noOfTodo || completedNo <= 0)
        {
            std::cout << "Error: todo #" << completedNo << " does not exist." << std::endl;
            return;
        }

        // Removes completed todo
        const int deleteIdx = noOfTodo - completedNo;
        std::string toRemove = lines[deleteIdx];
        std::cout << "Marked todo #" << completedNo << " as done." << std::endl;
        lines.erase(lines.begin() + deleteIdx);

        // Calls another function to write completed task on done.txt
        writeCompleted(toRemove, donePath, outDonePath, currentDate);

        // Writes the updated list to ofstream object
        for (auto lineItr : lines)
        {
            writeTodoFile << lineItr + "\n";
        }

        readTodoFile.close();
        std::remove("todo.txt");
        writeTodoFile.close();
        std::rename("outputTodo.txt", "todo.txt");
    }
    else
    {
        std::cout << "Error: todo #" << completedNo << " does not exist." << std::endl;
        return;
    }
}

void generateReport(char todoPath[], char donePath[], char currentDate[])
{
    std::ifstream readTodoFile(todoPath);
    std::ifstream readDoneFile(donePath);

    std::string line;
    int countTodo = 0;
    int countDone = 0;

    if (readTodoFile)
    {
        // Reads the todo.txt line by line and increments countTodo
        while (getline(readTodoFile, line))
        {
            countTodo++;
        }
    }

    if (readDoneFile)
    {
        // Reads the done.txt line by line and increments coundDone
        while (getline(readDoneFile, line))
        {
            countDone++;
        }
    }

    // Prints the report in the required format
    std::string date(currentDate);
    std::cout << date + " Pending : " + std::to_string(countTodo) + " Completed : " + std::to_string(countDone) << std::endl;
}

int main(int argc, char *argv[])
{
    // Gets current working directory path
    char path[256];
    getcwd(path, 256);
    strcat(path, "/");

    // Makes the path for todo.txt
    char todoPath[256];
    strcpy(todoPath, path);
    strcat(todoPath, "todo.txt");

    // Makes the path for outputTodo.txt
    char outTodoPath[256];
    strcpy(outTodoPath, path);
    strcat(outTodoPath, "outputTodo.txt");

    // Makes the path for done.txt
    char donePath[256];
    strcpy(donePath, path);
    strcat(donePath, "done.txt");

    // Gets date in YYYY-MM-DD format and stores in buffer char array
    std::time_t rawtime;
    std::tm *timeinfo;
    char buffer[80];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(buffer, 80, "%Y-%m-%d", timeinfo);

    // Performs operations based on the arguments
    if (argc == 1)
    {
        printHelp();
    }
    else if (strcmp(argv[1], "help") == 0)
    {
        printHelp();
    }
    else if (strcmp(argv[1], "ls") == 0)
    {
        showList(todoPath);
    }
    else if (strcmp(argv[1], "add") == 0)
    {
        if (argc > 2)
        {
            addTodo(argv[2], todoPath, outTodoPath);
        }
        else
        {
            std::cout << "Error: Missing todo string. Nothing added!" << std::endl;
        }
    }
    else if (strcmp(argv[1], "del") == 0)
    {
        if (argc > 2)
        {
            deleteTodo(atoi(argv[2]), todoPath, outTodoPath);
        }
        else
        {
            std::cout << "Error: Missing NUMBER for deleting todo." << std::endl;
        }
    }
    else if (strcmp(argv[1], "done") == 0)
    {
        if (argc > 2)
        {
            // Makes the path for outputDone.txt
            char outDonePath[256];
            strcpy(outDonePath, path);
            strcat(outDonePath, "outDone.txt");

            markComplete(atoi(argv[2]), todoPath, outTodoPath, donePath, outDonePath, buffer);
        }
        else
        {
            std::cout << "Error: Missing NUMBER for marking todo as done." << std::endl;
        }
    }
    else if (strcmp(argv[1], "report") == 0)
    {

        generateReport(todoPath, donePath, buffer);
    }
    else
    {
        std::cout << "Error: Option not Available. Try running \"./todo.sh help\" for Usage Information." << std::endl;
    }

    return 0;
}
