#ifndef network_hpp

#define network_hpp

#include <vector>
#include <string>
#include <complex>
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

//class Network{
//    vector<Component> parts;
//};

class Component{
    string name; // name, e.g. R1, C3, etc.
    vector<int> nodes; // vector containing the 2 nodes of the component
};

class Resistor : public Component {
    float value;
};

class Inductor : public Component {
    float value;
};

class Capacitor : public Component {
    float value;
};

class Voltage : public Component {
    float amplitude;
    float frequency;
    float offset;
};

class Current : public Component {
    float amplitude;
    float frequency;
    float offset;
};

//not compulsory bc it's *"advanced"*
class Diode : public Component {};
class Transistor : public Component {};

#endif
