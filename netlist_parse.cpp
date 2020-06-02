// #include "network.hpp"

#include <vector>
#include <string>
#include <complex>
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

//Splits a single component line into individual vectors
vector<string> troy()
{   

    string line;

    vector<string> vector_line;

    getline(cin, line);


    line += ' ';
    int prevBreak = 0;

    for (int i = 0; i <= line.length(); i++)
    {
        if (line[i] == ' ' || line[i] == '\n')
        {
            string word = "";


            for (int z = prevBreak; z < i; z++)
            {
                word = word + line[z];
            }

            vector_line.push_back(word);
            prevBreak = i+1;

        }
    }

    return vector_line;
}

int main(){

    vector<string> cabbage = troy();
    
    for (int i = 0; i < cabbage.size(); i++)
     {
        cout<< cabbage[i] << endl;
     }
}