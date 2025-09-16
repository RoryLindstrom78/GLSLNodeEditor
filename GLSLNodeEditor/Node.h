#ifndef NODE_H
#define NODE_H 

#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>

struct NodeLink {
	int id;
	int start_attr; // output attribute id
	int end_attr; // input attribute id
};


class Node {
public:
	static int counter;
	int id;

	~Node() = default;

	// Every node must be able to draw its UI and convert its values into GLSL code
	virtual void drawUI() = 0;
	virtual std::string getGLSL() = 0;

	// Return all input attr IDs
	virtual std::vector<int> getInputIDs() { return {}; }

	// Set input pointer by index
	virtual void setInput(int index, Node* node) {}

	// For getting the output of any node function
	virtual std::string getOutputVar() = 0;

	virtual std::string getTypeName() = 0;

};

int Node::counter = 1; // NODE: NEED A BETTER WAY OF KEEPING TRACK OF NODES


#endif
