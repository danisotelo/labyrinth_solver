/*
=================================================================================
                       CONCURRENT LABYRINTH SEARCH PROGRAM
=================================================================================
- Author: Daniel Sotelo Aguirre
- E-mail: danisotacam@gmail.com
- Subject: INGENIA-SE 22/23
- Date: April 5, 2023
=================================================================================
*/

// Import all the header files
#include <iostream>
#include <fstream>
#include "../include/Labyrinth.hpp"

using namespace std;

// Function to load the labyrinth from a file
vector<string> LoadLabyrinth(const string &file)
{

    vector<string> Labyrinth;

    ifstream source;
    source.open(file);
    string line;

    while (getline(source, line))
    {
        Labyrinth.push_back(line);
    }

    return Labyrinth;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " labyrinth_file_name.txt" << endl;
        return 1;
    }

    string file_name = argv[1];
    string file_path = "../labyrinth/" + file_name;
    ifstream file_stream(file_path);
    if (!file_stream.is_open())
    {
        cout << endl;
        cout << "Error: File not found. Ensure that the file is in the appropiate folder." << endl;
        return 1;
    }

    bool display;
    cout << "Do you want to display the labyrinth solver animation?: 1 (Yes) / 0 (No): ";
    cin >> display;

    vector<string> data = LoadLabyrinth(file_path);
    Labyrinth labyrinth(data, display);

    cout << "====================================================================================================\n";
    cout << "                                       LABYRINTH TO BE SOLVED                                       \n";
    cout << "====================================================================================================\n";
    labyrinth.PrintLabyrinth();

    labyrinth.SolveLabyrinth();
    labyrinth.PrintReport();
    return 0;
}