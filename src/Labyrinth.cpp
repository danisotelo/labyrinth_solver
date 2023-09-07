// Import for clearing console in Windows
#ifdef _WIN32
#include <windows.h>
#endif

// Function to clear the console
void clearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Import all the header files
#include "../include/Labyrinth.hpp"

using namespace std;

Labyrinth::Labyrinth(std::vector<std::string> labyrinth, bool disp)
    : labyr(labyrinth), display(disp)
{
    GetCoords();
}

Labyrinth::~Labyrinth() {}

// Function to get the coordinates of initial and final points of the labyrinth
void Labyrinth::GetCoords()
{

    for (int i = 0; i < labyr.size(); i++)
    {

        for (int j = 0; j < labyr[i].size(); j++)
        {

            if (labyr[i][j] == start)
            {
                in_coords.push_back(i);
                in_coords.push_back(j);
            }

            if (labyr[i][j] == end)
            {
                out_coords.push_back(i);
                out_coords.push_back(j);
            }
        }
    }
}

// Function to display on the screen the loaded labyrinth to be solved
void Labyrinth::PrintLabyrinth()
{

    cout << endl;
    for (int i = 0; i < labyr.size(); i++)
    {
        cout << labyr[i] << endl; // Print line by line
    }
    cout << endl;
}

/* Function to move each thread in the labyrinth (identified by "index")
   from its current row and column, "curr_r" and "curr_c", respectively.
   The mutex is locked and unlocked to avoid several threads trying to
   read and write simultaneously the Labyrinth object variables */
void Labyrinth::Move(size_t curr_r, size_t curr_c, int index)
{

    info_mutex.lock();

    // Check if the current position is the exit of the labyrinth
    if (labyr[curr_r][curr_c] == end)
    {
        labyr[curr_r][curr_c] = visited;
        info[index].setEndPos(out_coords);
        solved = true;
    }

    info_mutex.unlock();

    // Loop to avoid destroying mutex while some threads are still running
    while (solved != true)
    {

        info_mutex.lock();

        if (display == true)
        {
            clearConsole();
            PrintLabyrinth();
            this_thread::sleep_for(chrono::milliseconds(25));
        }

        // Mark current position as visited
        labyr[curr_r][curr_c] = visited;

        info_mutex.unlock();

        // Bool to continue or create a new thread
        bool cont_th = false;

        // Define next position
        size_t next_r, next_c;

        /* For a position to be available it must be satisfied that it is either a
           path(" ") or the end of the labyrinth ("O"). It additionally should be
           satisfied that the position is inside the boundaries of the labyrinth */

        // Check if left position is available
        if (curr_c > 0 && (labyr[curr_r][curr_c - 1] == path || labyr[curr_r][curr_c - 1] == end))
        {
            ContinueOrCreate(&cont_th, &next_r, &next_c, curr_r, curr_c, curr_r, curr_c - 1, index);
        }

        // Check if right position is available
        if ((curr_c < out_coords[1]) && (labyr[curr_r][curr_c + 1] == path || labyr[curr_r][curr_c + 1] == end))
        {
            ContinueOrCreate(&cont_th, &next_r, &next_c, curr_r, curr_c, curr_r, curr_c + 1, index);
        }

        // Check if upper position is available
        if (curr_r > 0 && (labyr[curr_r - 1][curr_c] == path || labyr[curr_r - 1][curr_c] == end))
        {
            ContinueOrCreate(&cont_th, &next_r, &next_c, curr_r, curr_c, curr_r - 1, curr_c, index);
        }

        // Check if bottom position is available
        if (((curr_r + 1) < labyr.size()) && (labyr[curr_r + 1][curr_c] == path || labyr[curr_r + 1][curr_c] == end))
        {
            ContinueOrCreate(&cont_th, &next_r, &next_c, curr_r, curr_c, curr_r + 1, curr_c, index);
        }

        // If the boolean has been set to 1 in a certain direction, move in that direction
        if (cont_th)
        {
            info_mutex.lock();
            info[index].incrementNumSteps();         // Update number of steps
            info[index].setEndPos({next_r, next_c}); // Update end position
            info_mutex.unlock();
            Move(next_r, next_c, index);
        }

        // If there are no more steps to take, make the thread wait until solution is found
        else
        {
            while (solved != true)
            {
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        }
    }
}

/* Function to choose if the thread given by the variable "index" in position given by
   "curr_r" and "curr_c", should continue to the next position given by "new_r" and
   "new_c", or a new thread should be created */
void Labyrinth::ContinueOrCreate(bool *continue_thread, size_t *next_r, size_t *next_c, size_t curr_r, size_t curr_c, size_t new_r, size_t new_c, int index)
{

    // If the boolean is still 0, the first thread continues through that path
    if (*continue_thread == false)
    {
        *continue_thread = true;
        *next_r = new_r;
        *next_c = new_c;
    }

    /* If the boolean has been set to 1, the first thread has already gone in another direction.
       Thus, a new thread is created in order to go in the new possible direction. The new thread
       starts with the number of steps already made plus one (step from intersection to beginning
       of the new thread), since the thread is initialize in the new position */
    else
    {
        info_mutex.lock();
        vector<size_t> init = {curr_r, curr_c};
        info.emplace_back(init, info[index].getNumSteps() + 1, number_of_threads + 1);
        number_of_threads += 1;
        info_mutex.unlock();

        // Thread created in new position with the next index
        thread new_thread(bind(&Labyrinth::Move, this, new_r, new_c, info.size() - 1));
        new_thread.detach();
    }
}

// Function to solve the labyrinth
void Labyrinth::SolveLabyrinth()
{
    cout << "      ******************************  SOLVING THE LABYRINTH ... ******************************      \n";

    auto t_0 = chrono::high_resolution_clock::now(); // Save time of initialization

    info.emplace_back(in_coords, 0, 1); // Introduce first myThread object into the info vector
    number_of_threads += 1;

    thread first(bind(&Labyrinth::Move, this, in_coords[0], in_coords[1], 0)); // Create first thread
    first.detach();                                                            // Threads are detached so that they can run independently

    while (!solved)
        ; // Wait until the labyrinth is finished

    auto t_f = chrono::high_resolution_clock::now();                  // Save time of finalization
    duration = chrono::duration_cast<chrono::nanoseconds>(t_f - t_0); // Estimate duration from t_0 and t_f in ms

    if (display == true)
    {
        clearConsole();
    }

    cout << endl;
    cout << "      ******************************  THE LABYRINTH IS SOLVED!  ******************************      \n";
    cout << endl;
}

// Function to display on the screen a report with the data requested after execution
void Labyrinth::PrintReport()
{

    cout << "====================================================================================================\n";
    cout << "                                     REPORT OF END OF EXECUTION                                     \n";
    cout << "====================================================================================================\n";
    cout << " LABYRINTH PARAMETERS" << endl;
    cout << "----------------------------------------------------------------------------------------------------\n";
    cout << " * Note: Coordinates are in format [Row, Column]. Both start counting from 1. \n"
         << endl;
    cout << " - Initial Coordinates ('I'): [ " << in_coords[0] + 1 << ", " << in_coords[1] + 1 << "]" << endl;
    cout << " - Final Coordinates ('O'):   [ " << out_coords[0] + 1 << ", " << out_coords[1] + 1 << "]" << endl;
    cout << " - Number of threads:         " << number_of_threads << endl;
    cout << " - Total duration:            " << duration.count() / 1e6 << " ms" << endl;
    cout << endl;
    cout << "----------------------------------------------------------------------------------------------------\n";
    cout << " THREAD PARAMETERS" << endl;
    cout << "----------------------------------------------------------------------------------------------------\n";
    cout << " * Note: Number of steps of a child thread includes the steps of the parent thread. \n"
         << endl;

    int min_steps = 0;
    for (int i = 0; i < info.size(); i++)
    {
        myThread th_info = info[i];
        vector<size_t> init_pos = th_info.getInitPos();
        vector<size_t> end_pos = th_info.getEndPos();
        int steps = th_info.getNumSteps();
        int id = th_info.getIndex();

        // Minimum number of steps is given by the thread whose final position is the end position of the labyrinth
        if (end_pos == out_coords)
        {
            min_steps = steps;
        }

        cout << " - Thread ID" << id << ": Start Position = [" << init_pos[0] + 1 << ", " << init_pos[1] + 1 << "]; End Position = [" << end_pos[0] + 1 << ", " << end_pos[1] + 1 << "]; Number of steps = " << steps << endl;
    }

    cout << endl;
    cout << "----------------------------------------------------------------------------------------------------\n";
    cout << " MINIMUM NUMBER OF STEPS TO SOLVE THE LABYRINTH" << endl;
    cout << "----------------------------------------------------------------------------------------------------\n";
    cout << " - Minimum number of steps to get out = " << min_steps << endl;
    cout << "====================================================================================================\n";
}