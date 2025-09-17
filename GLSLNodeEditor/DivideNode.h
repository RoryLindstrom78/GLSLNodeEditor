#ifndef DIVIDE_NODE_H
#define DIVIDE_NODE_H

class DivideNode : public Node {
private:
    int currentType = -1;
    std::string typeName;
    std::string varName;
    Node* inputA = nullptr;
    Node* inputB = nullptr;
    glm::vec4 value;
    int inputA_id;
    int inputB_id;

public:
    DivideNode() {
        id = counter;
        inputA_id = id * 10 + 1;
        inputB_id = id * 10 + 2;
        inputs.push_back(nullptr);
        inputs.push_back(nullptr);

        varName = "const_" + std::to_string(counter);

        counter++;
    }

    void updateTypeEveryFrame() {
        if (!inputA || !inputB) {
            typeName = "float"; // default fallback
            return;
        }

        std::string typeA = inputA->getTypeName();
        std::string typeB = inputB->getTypeName();

        if (typeA == typeB) typeName = typeA;
        else if (typeA == "float") typeName = typeB;
        else if (typeB == "float") typeName = typeA;
        else std::cout << "error" << std::endl;
    }

    std::vector<int> getInputIDs() override {
        return { inputA_id, inputB_id };
    }

    void setInput(int index, Node* node) override {
        if (index == 0) {
            inputA = node;
            inputs[0] = node;
        }
        if (index == 1) {
            inputB = node;
            inputs[1] = node;
        }
    }

    std::string getOutputVar() override {
        return varName;
    }

    void drawUI() override {
        // First we'll update the type
        updateTypeEveryFrame();

        ImNodes::BeginNode(id);

        // -- Node title --
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Divide Node");
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

    std::string getTypeName() override {
        return typeName;
    }

    std::string getGLSL() override {
        std::string nameA = inputA ? inputA->getOutputVar() : "0.0";
        std::string nameB = inputB ? inputB->getOutputVar() : "0.0";
        return typeName + " " + varName + " = " + nameA + " / " + nameB + ";\n";
    }
};

#endif