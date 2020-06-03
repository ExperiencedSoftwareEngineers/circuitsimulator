#ifndef network_hpp

#define network_hpp

#include <vector>
#include <string>
#include <complex>
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;


class Component{
	public:
    string name; // name, e.g. R1, C3, etc.
    vector<int> nodes; // vector containing the 2 nodes of the component
	float amplitude;
    float frequency;
    float offset;
	float value;
	char type;
};

class Resistor : public Component {
public:
	Resistor(string na, vector<int> no, float val)
	{
		name = na;
		nodes = no;
		value = val;
	}
	type = 'R';
    // float value;
};

class Inductor : public Component {
public:
 	Inductor(string na, vector<int> no, float val)
	{
		name = na;
		nodes = no;
		value = val;
	}
	type = 'L';
    // float value;
};

class Capacitor : public Component {
public:
	Capacitor(string na, vector<int> no, float val)
	{
		name = na;
		nodes = no;
		value = val;
	}
	type = 'C';
    // float value;
};

class VoltageDC : public Component {
public:
	VoltageDC(string na, vector<int> no, float val)
	{
		name = na;
		nodes = no;
		value = val;
	}
	type = 'V';
    // float value;
};

class VoltageAC : public Component {
public:
	VoltageAC(string na, vector<int> no, float amp, float fre, float off)
	{
		name = na;
		nodes = no;
		amplitude = amp;
		frequency = fre;
		offset = off;
	}
	type = 'W';
    // float amplitude;
    // float frequency;
    // float offset;
};

class CurrentDC : public Component {
public:
	CurrentDC(string na, vector<int> no, float val)
	{
		name = na;
		nodes = no;
		value = val;
	}
	type = 'I';
    // float value;
};

class CurrentAC : public Component {
public:
	CurrentAC(string na, vector<int> no, float amp, float fre, float off)
	{
		name = na;
		nodes = no;
		amplitude = amp;
		frequency = fre;
		offset = off;
	}
	type = 'J';
    // float amplitude;
    // float frequency;
    // float offset;
};

class Network{
	public:
    vector<Component> parts;
	float stop; //stop time of the transient analysis
	float step; //time between intervals
	
};

//not compulsory bc it's *"advanced"*
class Diode : public Component {};
class Transistor : public Component {};

#endif
