/* A Program designed to emulate a shell
 * File:   main.cpp
 * Author: romanoad
 * Copyright (C) Anthony Romano
 * Created on September 23, 2019, 2:41 PM
 */
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>


typedef std::vector<std::string> StrVec;
typedef std::vector<std::vector<std::string>> StrVec2D;
typedef std::vector<int> pidVec;

void parallel(std::string &line);
void parseCommand(std::string &line, StrVec &parsedArgs);
void myExec(StrVec argList);
void simpleExecute(const std::string &line, const StrVec &args);
void serial(std::string &line);
bool handleLineSerial(std::string &line);
void printCommand(const StrVec &args);
void handleLinesParallel(StrVec2D &lines);

int main(int argc, char** argv) {
    std::string line;
    StrVec args;
    while (std::cout << ">", std::getline(std::cin, line)) {
        if (handleLineSerial(line)) {
            return 0;
        }
    }
}

void parseCommand(std::string &line, StrVec &parsedArgs) {
    parsedArgs.clear();
    std::string argument;
    std::stringstream lineStream(line);
    while (lineStream >> std::quoted(argument)) {
        parsedArgs.push_back(argument);
    }
}

void printCommand(const StrVec &args) {
    std::cout << "Running: ";
    for (size_t i = 0; i < args.size(); i++) {
        std::cout << args[i] << " ";
    }
    std::cout << std::endl;
}

void myExec(StrVec argList) {
    std::vector<char*> args;
    for (size_t i = 0; (i < argList.size()); i++) {
        args.push_back(&argList[i][0]);  
    }  // nullptr is very important
    args.push_back(nullptr);
    execvp(args[0], &args[0]);
}

// used to execute commands that are done in series
void simpleExecute(const std::string &line, const StrVec &args) {
    const int pid = fork();
    if (pid == 0) {
        printCommand(args);
        myExec(args);
    } else {
        int childExit = -1;
        waitpid(pid, &childExit, 0);
        std::cout << "Exit code: " << childExit << std::endl;
    }
}

void serial(std::string &line) {
    StrVec args;
    parseCommand(line, args);
    std::ifstream commandFile(args[1]);
    std::string command;
    while (std::getline(commandFile, command)) {
        if (handleLineSerial(command)) {
            return;
        }
    }
}

// Function to handle calls that are made in serial, returns true if the exit
// command was called, used in both main and serial methods
bool handleLineSerial(std::string &line) {
    StrVec args;
    parseCommand(line, args);
    if (args.size() ==0) {
        // If the line is empty, do nothing
    } else if (args[0] == "exit") {
        return true;
    } else if (args[0] == "SERIAL") {
        serial(line);
    } else if (args[0] == "PARALLEL") {
        parallel(line);
    } else if (args[0] == "#") {
        // If the line is a comment, skip it
    } else {
        simpleExecute(line, args);
    }
    return false;
}

void parallel(std::string &line) {
    // Read in all the lines and pass them to the handler function
    StrVec args;
    parseCommand(line, args);
    std::ifstream commandFile(args[1]);
    std::string command;
    StrVec2D lines;
    while (std::getline(commandFile, command)) {
        parseCommand(command, args);
        lines.push_back(args);
    }
    handleLinesParallel(lines);
}

void handleLinesParallel(StrVec2D &args) {
    pidVec pids;
    int pid;
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i].size() == 0 || args[i][0] == "#") {
            // Skip this line
        } else {
            // If it is a real line, fork then start running a process
            // If the process is a child, it will break after executing
            printCommand(args[i]);
            pid = fork();
            if (pid == 0) {
                myExec(args[i]);
                break;
            } else {
                pids.push_back(0);
            }
        }
    }
    if (pid != 0) {
        for (size_t i = 0; i < pids.size(); i++) {
            int childExit = -1;
            waitpid(pids[i], &childExit, 0);
            std::cout << "Exit code: " << childExit << std::endl;
        }
    }
}

