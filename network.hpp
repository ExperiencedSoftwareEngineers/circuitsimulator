#ifndef network_hpp

#define network_hpp

#include <vector>
#include <string>
#include <complex>
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

class Network{
	public:
    vector<Component> parts;
	float stop; //stop time of the transient analysis
	float step; //time between intervals
	
};

class Component{
	public:
    string name; // name, e.g. R1, C3, etc.
    vector<int> nodes; // vector containing the 2 nodes of the component
};

class Resistor : public Component {
public:
	Resistor(string na, vector<int> no, float val)
	{
		name = na;
		nodes = no;
		value = val;
	}

    float value;
};

class Inductor : public Component {
public:
 	Inductor(string na, vector<int> no, float val)
	{
		name = na;
		nodes = no;
		value = val;
	}
    float value;
};

class Capacitor : public Component {
public:
	Capacitor(string na, vector<int> no, float val)
	{
		name = na;
		nodes = no;
		value = val;
	}
    float value;
};

class Voltage : public Component {
public:
	Voltage(string na, vector<int> no, float amp, float fre, float off)
	{
		name = na;
		nodes = no;
		amplitude = amp;
		frequency = fre;
		offset = off;
	}
    float amplitude;
    float frequency;
    float offset;
};

class Current : public Component {
public:
	Current(string na, vector<int> no, float amp, float fre, float off)
	{
		name = na;
		nodes = no;
		amplitude = amp;
		frequency = fre;
		offset = off;
	}
    float amplitude;
    float frequency;
    float offset;
};

//not compulsory bc it's *"advanced"*
class Diode : public Component {};
class Transistor : public Component {};

#endif
