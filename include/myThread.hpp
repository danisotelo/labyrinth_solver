#ifndef MYTHREAD_HPP
#define MYTHREAD_HPP

#include <vector>

class myThread {

private:
    std::vector<size_t> init_pos;
    std::vector<size_t> end_pos;
    int num_steps;
    int index;

public:
    myThread(std::vector<size_t> init, int n, int i);
    ~myThread();

    // Accessors for member variables
    std::vector<size_t> getInitPos();
    std::vector<size_t> getEndPos();
    int getNumSteps();
    int getIndex();
    void setEndPos(std::vector<size_t> e);
    void incrementNumSteps();
};

#endif