#ifndef ADD_NODE_H
#define ADD_NODE_H


class AddNode : public Node {
private:
	Node* inputA = nullptr;
	Node* inputB = nullptr;
	glm::vec4 value;
	int inputA_id;
	int inputB_id;

public:
	AddNode() { 
        id = counter; 
        inputA_id = id * 10 + 1;
        inputB_id = id * 10 + 2;
        counter++;
    }

    std::vector<int> getInputIDs() override {
        return { inputA_id, inputB_id };
    }

    void setInput(int index, Node* node) override {
        if (index == 0) inputA = node;
        if (index == 1) inputB = node;
    }

    std::string getOutputVar() override {
        return inputA->getOutputVar();
    }

	void drawUI() override {
        ImNodes::BeginNode(id);

        // -- Node title --
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Add Node");
        ImNodes::EndNodeTitleBar();

        ImGui::BeginGroup();

        // Inputs
        ImGui::PushItemWidth(150);
        ImNodes::BeginInputAttribute(inputA_id);
        ImGui::Text("A");
        ImNodes::EndInputAttribute();

        ImGui::PushItemWidth(150);
        ImNodes::BeginInputAttribute(inputB_id);
        ImGui::Text("B");
        ImNodes::EndInputAttribute();

        // Output
        ImGui::PushItemWidth(150);
        ImNodes::BeginOutputAttribute(id * 10);
        ImGui::Text("out");
        ImNodes::EndOutputAttribute();
        ImGui::PopItemWidth();
        ImGui::EndGroup();

        ImNodes::EndNode();
	}

    std::string getGLSL() override {
        std::string nameA = inputA ? inputA->getOutputVar() : "0.0";
        std::string nameB = inputB ? inputB->getOutputVar() : "0.0";
        return nameA + " = " + nameA + " + " + nameB + ";";
    }
};

#endif