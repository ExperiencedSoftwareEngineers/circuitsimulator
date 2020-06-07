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
	char flavour;
	float prevCurrent;
	float prevVoltage;
	float lastValue;
};

class Resistor : public Component {
public:
	Resistor(string na, vector<int> no, float val)
	{
		name = na;
		nodes = no;
		value = val;
		flavour = 'R';
	}
    // float value;
};

class Inductor : public Component {
public:
 	Inductor(string na, vector<int> no, float val)
	{
		name = na;
		nodes = no;
		value = val;
		flavour = 'L';
		prevVoltage = 0;
	}
    // float value;
};

class Capacitor : public Component {
public:
	Capacitor(string na, vector<int> no, float val)
	{
		name = na;
		nodes = no;
		value = val;
		flavour = 'C';
		prevCurrent;
		lastValue = 0;
	}
    // float value;
};

class VoltageDC : public Component {
public:
	VoltageDC(string na, vector<int> no, float val)
	{
		name = na;
		nodes = no;
		value = val;
		flavour = 'V';
	}
    // float value;
};

class VoltageAC : public Component {
public:
	VoltageAC(string na, vector<int> no, float off, float amp, float fre)
	{
		name = na;
		nodes = no;
		amplitude = amp;
		frequency = fre;
		offset = off;
		flavour = 'W';
	}
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
		flavour = 'I';
	}
    // float value;
};

class CurrentAC : public Component {
public:
	CurrentAC(string na, vector<int> no, float off, float amp, float fre)
	{
		name = na;
		nodes = no;
		amplitude = amp;
		frequency = fre;
		offset = off;
		flavour = 'J';
	}
    // float amplitude;
    // float frequency;
    // float offset;
};

class Network{
	public:
    vector<Component> parts;
	float stop; //stop time of the transient analysis
	float step; //time between intervals
	int voltageCount;
	int resistorCount;
	int currentCount;
	int capacitorCount;
	
};

//not compulsory bc it's *"advanced"*
class Diode : public Component {

	float Vt;
	double Is;

	Diode(string na, vector<int> no)
	{
		name = na;
		nodes = no;
		Vt = 0.7;
		flavour = 'D';
		Is = 0.0000000000000252;
	}
};


class Transistor : public Component {};

#endif
