#include "netlist_parse.hpp"
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <cmath>

using namespace Eigen;

vector<int> sortandmerge(Network netw)
{
	vector<int> biglist;
	for(int i = 0; i < netw.parts.size(); i++)
	{
		for(int a = 0; a < netw.parts[i].nodes.size(); a++)
		{
			biglist.push_back(netw.parts[i].nodes[a]);
		}
	}
	sort(biglist.begin(), biglist.end());
	biglist.erase(unique(biglist.begin(), biglist.end()),biglist.end());
	return biglist;
}

vector<Component> listfornode(Network netw, int node)
{
	vector<Component> output;
	for(int i = 0; i < netw.parts.size(); i++)
	{
		for(int a = 0; a < netw.parts[i].nodes.size(); a++)
		{
			if(netw.parts[i].nodes[a] == node)
			{
				output.push_back(netw.parts[i]);
			}
		}
	}
	return output;
}

vector<Component> betweennodes(Network netw, int node1, int node2)
{
	vector<Component> input = listfornode(netw, node1);
	vector<Component> output;
	for(int i = 0; i < input.size(); i++)
	{
		for(int a = 0; a < input[i].nodes.size(); a++)
		{
			if(input[i].nodes[a] == node2)
			{
				output.push_back(input[i]);
			}
		}
	}
	return output;
}


// pair<MatrixXf,VectorXf) condmatrix(Network netw, float time)
// {
// 	vector<Component> list = netw.parts;
// 	vector<int> nodes = sortandmerge(netw);
// 	MatrixXf matrix(nodes.size()-1, nodes.size()-1);
// 	VectorXf curvec(nodes.size()-1);
// 	for(int i = 1; i < nodes.size(); i++)//iterating row
// 	{
// 		vector<Component> nodecom = listfornode(netw, nodes[i]);
// 		bool other = true;
// 		float voltage = 0;
// 		for(int a = 1; a < nodecom.size(); a++)
// 		{
// 			if(nodecom[a].flavour == 'V')
// 			{
// 				voltage += nodecom[a].value;
// 			}
// 			if(nodecom[a].flavour == 'W')
// 			{
// 				voltage += nodecom[a].offset + nodecom[a].amplitude * ;
// 			}
// 		} 
// 		for(int a = 1; a < nodes.size(); a++)//columns
// 		{
			
// 		}
// 	}
// }



pair<MatrixXf,VectorXf> condmatrix(Network netw, float time)
{
	vector<Component> components = netw.parts; // list of all components in circuit
	vector<int> nodes = sortandmerge(netw); // list of all nodes in netlist


	MatrixXf matrix(nodes.size()-1, nodes.size()-1); // define the conductance matrix and voltage vector
	matrix = MatrixXf::Zero(nodes.size()-1, nodes.size()-1);
	VectorXf curvec(nodes.size()-1); // define current vector
	curvec = VectorXf::Zero(nodes.size()-1);


	for(int i = 0; i < components.size(); i++) // loop through all components in circuit
	{
		int node1 = components[i].nodes[0];
		int node0 = components[i].nodes[1];
		float value = 0;
		if((components[i].flavour == 'W')||(components[i].flavour == 'J'))
		{
			value = components[i].offset + (components[i].amplitude * sin(components[i].frequency * time));
		}
		else
		{
			value = components[i].value; //for resistors, inductors, capacitors and dc sources
		}
		if(components[i].flavour == 'R')
		{
			//diagonal, addition of all conductances attached to node

			if(node0 != 0)
			{
				matrix(node0 - 1, node0 - 1) += 1/value; // every time go round loop add resistor conductance to G(node1 -1)(node1 -1)
			}											 // and G(node0 -1)(node0-1) once looped through all components the values
			if(node1 != 0)								 //of G for the diagonal will be correct
			{
				matrix(node1 - 1, node1 -1) += 1/value; 
			}

			//off diagonal, conductance between nodes

			if(node0 != 0 && node1 != 0)
			{
				matrix(node1 -1, node0 -1) += -1/value; //eg if node0 = 1 and node1 = 2 there must be a direct connection so G21 is -1/value
				matrix(node0 -1, node1 -1) += -1/value;	//since G12 = G21 
			}
		}
		else if((components[i].flavour == 'I') || (components[i].flavour == 'J')) //for DC current sources
		{
			if(node1 != 0)
			{
				curvec(node1 - 1) += -value; //defined +ve current as flowing from node0 to node1
			}
			if(node0 != 0)
			{
				curvec(node0 - 1) += value;
			}
		}
	}

	vector<bool> changed;
	for(int i = 0; i < nodes.size(); i++){
		changed.push_back(0); //note this is a bool, not a number
	}

	for(int i = 0; i < components.size(); i++){
		
		int node1 = components[i].nodes[0];
		int node0 = components[i].nodes[1];
		float value = 0;

		if((components[i].flavour == 'W')||(components[i].flavour == 'J'))
		{
			value = components[i].offset + (components[i].amplitude * sin(components[i].frequency * time));
		}
		else
		{
			value = components[i].value; //for resistors, inductors, capacitors and dc sources
		}

		if((components[i].flavour == 'V') || (components[i].flavour == 'W')){

			if(node1 != 0)
			{
				if(!changed[node1 - 1] && node1 != 0)
				{
					matrix.row(node1 - 1).setZero();
					curvec(node1 - 1) = 0;
					changed[node1 -1] = true;
				}
			}
			if(node0 != 0)
			{
				if(!changed[node0 - 1] && node0 != 0)
				{
					matrix.row(node0 - 1).setZero();
					curvec(node0 - 1) = 0;
					changed[node0 - 1] = true;
				}
			}
			if(node0 == 0)
			{
				matrix(node1 - 1, node1 - 1) += 1;
				curvec(node1 - 1) += value;
			}
			if(node1 == 0)
			{
				matrix(node0 - 1, node0 - 1) += 1;
				curvec(node0 - 1) -= value;
			}
			if((node1 != 0) && (node0 != 0))
			{
				matrix(node1 - 1, node1 - 1) += 1;
				matrix(node1 - 1, node0 - 1) -= 1;

				matrix(node0 - 1, node0 - 1) += 1;
				matrix(node0 - 1, node1 - 1) -= 1;

				curvec(node0 - 1) -= value;
				curvec(node1 - 1) += value;
			}
		}

	}
 
	return make_pair(matrix, curvec);
}

VectorXf solmatrix(Network netw, float time)
{
	pair<MatrixXf,VectorXf> evans = condmatrix(netw, time);
	MatrixXf condmat = evans.first;
	//cout << condmat << endl;
	VectorXf curvec = evans.second;
	//cout << curvec << endl;
	MatrixXf incondmat = condmat.inverse();
	//cout << incondmat << endl;
	VectorXf volvec = incondmat * curvec;
	return volvec;
}

int main()
{
	Network n = parseNetwork();
	vector<int> output = sortandmerge(n);
	cou
	//cout << solmatrix(n,0) << endl;
}
