#ifndef LABYRINTH_HPP
#define LABYRINTH_HPP

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <mutex>
#include <functional>
#include "myThread.hpp" // Include the myThread class header

class Labyrinth {

private:
    std::vector<std::string> labyr;
    std::vector<size_t> in_coords;
    std::vector<size_t> out_coords;
    int number_of_threads = 0;
    std::chrono::nanoseconds duration;
    bool solved = false;

    std::mutex info_mutex;
    std::vector<myThread> info;

    bool display;
    const char wall = '#';
    const char start = 'I';
    const char end = 'O';
    const char path = ' ';
    const char visited = '.';

public:
    Labyrinth(std::vector<std::string> labyrinth, bool disp);
    ~Labyrinth();
    void GetCoords();
    void PrintLabyrinth();
    void Move(size_t curr_x, size_t curr_y, int index);
    void ContinueOrCreate(bool* continue_thread, size_t* next_r, size_t* next_c, size_t curr_r, size_t curr_c, size_t new_r, size_t new_c, int index);
    void SolveLabyrinth();
    void PrintReport();
};

#endif
