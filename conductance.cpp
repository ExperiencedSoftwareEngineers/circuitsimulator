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

pair<MatrixXf,VectorXf> condmatrix(Network netw, float time)
{
	vector<Component> components = netw.parts; // list of all components in circuit
	vector<int> nodes = sortandmerge(netw); // list of all nodes in netlist
	
	// cout << "capacitorCount: " << netw.capacitorCount << endl;
	int group2Count = netw.voltageCount + netw.capacitorCount;
	// cout << "Group2Count: " << group2Count << endl;
	// cout << "nodes.size -1: " << nodes.size() -1<< endl;
	int sizeOfMatrix = group2Count + nodes.size() -1;

	MatrixXf matrixA(sizeOfMatrix, sizeOfMatrix); // define the conductance matrix and voltage vector
	matrixA = MatrixXf::Zero(sizeOfMatrix, sizeOfMatrix);
	// cout << matrixA << endl;
	VectorXf curvec(sizeOfMatrix); // define current vector
	curvec = VectorXf::Zero(sizeOfMatrix);
	// cout << curvec << endl;
	
	int group2Index = sizeOfMatrix - group2Count;
	// cout << "group2Index: " << group2Index << endl;

	float step = netw.step;



	for(int i = 0; i < components.size(); i++) // loop through all components in circuit
	{
		int node1 = components[i].nodes[0];
		int node0 = components[i].nodes[1];
		float value = 0;
		// cout << "flavour: " << components[i].flavour << endl;

		if((components[i].flavour == 'W')||(components[i].flavour == 'J'))
		{
			value = components[i].offset + (components[i].amplitude * sin(components[i].frequency * 2 * M_PI * time));
			// cout << "time: " << time << endl;
			// cout << "value: " << value << endl;
		}
		else if(components[i].flavour == 'C')
		{
			value = (step * components[i].prevCurrent)/components[i].value;
			// cout << "cap val: " << value << endl;
			// cout << "prevCurrent: " << components[i].prevCurrent << endl;
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
				matrixA(node0 - 1, node0 - 1) += 1/value; // every time go round loop add resistor conductance to G(node1 -1)(node1 -1)
			}											 // and G(node0 -1)(node0-1) once looped through all components the values
			if(node1 != 0)								 //of G for the diagonal will be correct
			{
				matrixA(node1 - 1, node1 -1) += 1/value; 
			}

			//off diagonal, conductance between nodes

			if(node0 != 0 && node1 != 0)
			{
				matrixA(node1 -1, node0 -1) += -1/value; //eg if node0 = 1 and node1 = 2 there must be a direct connection so G21 is -1/value
				matrixA(node0 -1, node1 -1) += -1/value;	//since G12 = G21 
			}
		}
		else if((components[i].flavour == 'I') || (components[i].flavour == 'J')) 
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
		else if((components[i].flavour == 'V')|| (components[i].flavour == 'W') || (components[i].flavour == 'C'))
		{
			if(node0 != 0)
			{
				matrixA(node0 -1, group2Index) -= 1;
				matrixA(group2Index, node0 -1) -= 1;
			}
			if(node1 != 0)
			{
				matrixA(node1 - 1, group2Index) += 1;
				matrixA(group2Index, node1 -1) += 1;
			}

			curvec(group2Index) = value;

			group2Index +=1;

		}
	}

	// vector<bool> changed;
	// for(int i = 0; i < nodes.size(); i++){
	// 	changed.push_back(0); //note this is a bool, not a number
	// }

	// for(int i = 0; i < components.size(); i++){
		
	// 	int node1 = components[i].nodes[0];
	// 	int node0 = components[i].nodes[1];
	// 	float value = 0;

	// 	if((components[i].flavour == 'W')||(components[i].flavour == 'J'))
	// 	{
	// 		value = components[i].offset + (components[i].amplitude * sin(components[i].frequency * 2* M_PI* time));
	// 		//cout << "value: " << value << endl;
	// 	}
	// 	else
	// 	{
	// 		value = components[i].value; //for resistors, inductors, capacitors and dc sources
	// 	}

	// 	if((components[i].flavour == 'V') || (components[i].flavour == 'W')){

	// 		if(node1 != 0)
	// 		{
	// 			if(!changed[node1 - 1] && node1 != 0)
	// 			{
	// 				matrixA.row(node1 - 1).setZero();
	// 				curvec(node1 - 1) = 0;
	// 				changed[node1 -1] = true;
	// 			}
	// 		}
	// 		if(node0 != 0)
	// 		{
	// 			if(!changed[node0 - 1] && node0 != 0)
	// 			{
	// 				matrixA.row(node0 - 1).setZero();
	// 				curvec(node0 - 1) = 0;
	// 				changed[node0 - 1] = true;
	// 			}
	// 		}
	// 		if(node0 == 0)
	// 		{
	// 			matrixA(node1 - 1, node1 - 1) += 1;
	// 			curvec(node1 - 1) += value;
	// 		}
	// 		if(node1 == 0)
	// 		{
	// 			matrixA(node0 - 1, node0 - 1) += 1;
	// 			curvec(node0 - 1) -= value;
	// 		}
	// 		if((node1 != 0) && (node0 != 0))
	// 		{
	// 			matrixA(node1 - 1, node1 - 1) += 1;
	// 			matrixA(node1 - 1, node0 - 1) -= 1;

	// 			matrixA(node0 - 1, node0 - 1) += 1;
	// 			matrixA(node0 - 1, node1 - 1) -= 1;

	// 			curvec(node0 - 1) -= value;
	// 			curvec(node1 - 1) += value;
	// 		}
	// 	}

	// }

	return make_pair(matrixA, curvec);
}

VectorXf solmatrix(Network netw, float time)
{
	pair<MatrixXf,VectorXf> evans = condmatrix(netw, time);
	MatrixXf condmat = evans.first;
	// cout << condmat << endl << endl;
	VectorXf curvec = evans.second;
	// cout << curvec << endl << endl;
	MatrixXf incondmat = condmat.inverse();
	//cout << incondmat << endl;
	VectorXf volvec = incondmat * curvec;
	// cout << volvec << endl;

	int group2Count = netw.voltageCount + netw.capacitorCount;

	int nodes = sortandmerge(netw).size();
	int index = nodes-1;

	for(int i = 0; i < netw.parts.size(); i++ )
	{
		if(netw.parts[i].flavour == 'C')
		{
			index++;
			netw.parts[i].prevCurrent = volvec(index);
			//cout << "volvec index: " << volvec(index) << endl;
		}
	}

	return volvec;
}

vector<float> current(Network netw, VectorXf volvec, float time)
{
	vector<Component> components = netw.parts;
	vector<float> output;
	vector<float> nodalcomponents;
	float value = 0;

	for(int i = 0; i < components.size(); i++) // loop through all components in circuit
	{
		int node1 = components[i].nodes[0];
		int node0 = components[i].nodes[1];
		float voltage = 0;
	
		if((components[i].flavour == 'W')||(components[i].flavour == 'J'))
		{
			value = components[i].offset + (components[i].amplitude * sin(components[i].frequency * 2* M_PI* time));
			//cout << "value: " << value << endl;
		}
		else
		{
			value = components[i].value; //for resistors, inductors, capacitors and dc sources
		}
	
		if(components[i].flavour == 'R')
		{
			if(node0 == 0)
			{
				output.push_back(volvec[node1 - 1] / value);
			}
			else if(node1 == 0)
			{
				output.push_back(-1 * volvec[node0 - 1] / value);
			}
			else
			{
				output.push_back((volvec[node1 - 1] - volvec[node0 - 1]) / value);
			}
		}
		if((components[i].flavour == 'I')||(components[i].flavour == 'J'))
		{
				output.push_back(value);
		}
		
	}

	return output;
}

vector<VectorXf> simulate(Network netw)
{
	vector<VectorXf> output;
	float stop = netw.stop;
	float step = netw.step;
	for(float i = 0; i <= stop; i += step)
	{
		VectorXf volvec = solmatrix(netw,i);



		int group2Count = netw.voltageCount + netw.capacitorCount;

		int nodes = sortandmerge(netw).size();
		int index = nodes-1;

		output.push_back(volvec);
		vector<float> veccur = current(netw,volvec,i);
		VectorXf xfcur(veccur.size());
		for(int a = 0; a < veccur.size(); a++)
		{
			xfcur(a) = veccur[a];
		}
		output.push_back(xfcur);
	}
	return output;		
}


int main()
{
	Network n = parseNetwork();
	// vector<int> output = sortandmerge(n);
	// VectorXf lol = solmatrix(n, 1);
	// cout << lol << endl;

	// vector<float> woah = current(n, lol, 1);

	// for(int i = 0; i < woah.size(); i++)
	// {
	// 	cout << "current: " << woah[i] << endl;
	// }

	


	vector<int> nodes = sortandmerge(n);
	
	vector<VectorXf> printed = simulate(n);
	float step = n.step;
	//cout << step << endl;

	cout << "time" <<  ',';

	for(int i = 1; i < nodes.size(); i++)
	{
		cout << "V(node" << nodes[i] << ")" << ',';
	}

	for(int i = 0; i< n.parts.size(); i++)
	{
		if((n.parts[i].flavour == 'V') || (n.parts[i].flavour == 'W'))
		{
			cout << n.parts[i].name <<  ',';
		}
	}

	for(int i = 0; i< n.parts.size(); i++)
	{
		if((n.parts[i].flavour != 'V') && (n.parts[i].flavour != 'W'))
		{
			cout << n.parts[i].name <<  ',';
		}
	}

	cout << endl;


	for(int i = 0; i < printed.size(); i++)
	{
		if(i%2 == 0)
		{
			cout << step * i / 2 << ',';
		}
		for(int a = 0; a < printed[i].size(); a++)	
		{
			cout << printed[i](a);
			if(printed[i].size()-1 != a)
			{
				cout << ',';
			}
		}
		if(i%2)
		{
			cout<<endl;
		}
		else
		{
			cout<< ',';
		}
	}
}
