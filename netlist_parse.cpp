#include "network.hpp"

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

int main()
{
    vector<string> line;
	Network chad;
    while(!cin.fail())
	{
        line = troy();
		if (line[0][0] == 'R')
		{
			int node1;
			int node2;

			if('0'!=line[1][0])
			{
				node1 = stoi(line[1].substr(1));
			}
			else
			{
				node1 = 0;
			}

			if('0'!=line[1][0])
			{
				node2 = stoi(line[1].substr(1));
			}
			else
			{
				node2 = 0;
			}

			vector<int> inputnodes{node1,node2};
			Component lol = Resistor(line[0], inputnodes, stof(line[3]));

			chad.parts.push_back(lol);

		}

		if (line[0][0] == 'L')
		{
			int node1;
			int node2;

			if('0'!=line[1][0])
			{
				node1 = stoi(line[1].substr(1));
			}
			else
			{
				node1 = 0;
			}
			if('0'!=line[1][0])
			{
			int node2 = stoi(line[1].substr(1));
			}
			else
			{
				node2 = 0;
			}

			vector<int> inputnodes{node1,node2};
			Component lol = Inductor(line[0], inputnodes, stof(line[3]));

			chad.parts.push_back(lol);
		}

		if (line[0][0] == 'C')
		{
			int node1;
			int node2;
			if('0'!=line[1][0])
			{
				node1 = stoi(line[1].substr(1));
			}
			else
			{
				node1 = 0;
			}
			if('0'!=line[1][0])
			{
				node2 = stoi(line[1].substr(1));
			}
			else
			{
				node2 = 0;
			}

			vector<int> inputnodes{node1,node2};
			Component lol = Capacitor(line[0], inputnodes, stof(line[3]));

			chad.parts.push_back(lol);
		}

		if (line[0][0] == 'V')
		{
			int node1;
			int node2;

			if('0'!=line[1][0])
			{
				node1 = stoi(line[1].substr(1));
			}
			else
			{
				node1 = 0;
			}

			if('0'!=line[1][0])
			{
			int node2 = stoi(line[1].substr(1));
			}

			else
			{
				node2 = 0;
			}

			vector<int> inputnodes{node1,node2};
			Component lol = Voltage(line[0], inputnodes, stof(line[3].substr(5)), stof(line[5]), stof(line[5].substr(0,(line[5].length())-1)));

			chad.parts.push_back(lol);
		}
    }


// vector<string> cabbage = troy();
        
//         for (int i = 0; i < cabbage.size(); i++)
//         {
//             cout<< cabbage[i] << endl;
//         }

}