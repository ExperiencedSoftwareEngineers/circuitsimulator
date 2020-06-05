#ifndef netlist_parse_hpp
#define netlist_parse_hpp

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

Network parseNetwork()
{
    vector<string> line;
	Network chad;
	bool end = true;
	int node1;
	int node2;
	chad.voltageCount = 0;
	chad.currentCount = 0;
	chad.resistorCount = 0;
    while(end)
	{
        line = troy();
		
		if (line[0][0] != '.')
		{
			if('0'!=line[1][0])
			node1 = stoi(line[1].substr(1));
			else  node1 = 0;
			if('0'!=line[2][0])
			{
			node2 = stoi(line[2].substr(1));
			}
			else  node2 = 0;
		}
	
		Component comp;
		vector<int> inputnodes{node1,node2};

		if (line[0][0] == 'R')
		{		
			comp = Resistor(line[0], inputnodes, stof(line[3]));
			chad.parts.push_back(comp);
			chad.resistorCount += 1;
			//cout << "parse Resistor count: " << chad.resistorCount << endl;
		}
		else if (line[0][0] == 'L')
		{
			comp = Inductor(line[0], inputnodes, stof(line[3]));
			chad.parts.push_back(comp);
		}
		else if (line[0][0] == 'C')
		{
			comp = Capacitor(line[0], inputnodes, stof(line[3]));
			chad.parts.push_back(comp);
		}
		else if (line[0][0] == 'V')
		{
			chad.voltageCount += 1;
			//cout << "parse voltage count: " << chad.voltageCount << endl;

			if(line[3][0]=='S')
			{
			comp = VoltageAC(line[0], inputnodes, stof(line[3].substr(5)), stof(line[4]), stof(line[5].substr(0,(line[5].length())-1)));
			}
			else
			{
			comp = VoltageDC(line[0], inputnodes, stof(line[3]));
			}
			chad.parts.push_back(comp);

		}
		else if (line[0][0] == 'I')
		{
			chad.currentCount += 1;
			//cout << "current Count: " << chad.currentCount << endl;
			if(line[3][0]=='S')
			{
			comp = CurrentAC(line[0], inputnodes, stof(line[3].substr(5)), stof(line[4]), stof(line[5].substr(0,(line[5].length())-1)));
			}
			else
			{
			comp = CurrentDC(line[0], inputnodes, stof(line[3]));
			}
			chad.parts.push_back(comp);
		}
		else if (line[0] == ".tran")
		{
			chad.stop = stof(line[2]);
			chad.step = stof(line[4]);
		}
		else if ((line[0]==".end")||(cin.fail()))
		{
			end = false;
		}

		

    }
		return chad;

}

#endif

