#include "../include/myThread.hpp"

myThread::myThread(std::vector<size_t> init, int n, int i)
    : init_pos(init), end_pos(init), num_steps(n), index(i) {}

myThread::~myThread() {}

std::vector<size_t> myThread::getInitPos() {
    return init_pos;
}

std::vector<size_t> myThread::getEndPos() {
    return end_pos;
}

int myThread::getNumSteps() {
    return num_steps;
}

int myThread::getIndex() {
    return index;
}

void myThread::setEndPos(std::vector<size_t> e) {
    end_pos = e;
}

void myThread::incrementNumSteps() {
    num_steps++;
}