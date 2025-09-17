#ifndef CONSTANT_NODE_H 
#define CONSTANT_NODE_H

class ConstantNode : public Node {
private:
	int currentType = -1;
	std::string varName;
	std::string typeName;
	glm::vec4 value;

public:
	ConstantNode(const std::string& type = "float", glm::vec4 val = glm::vec4(0.0f))
		: typeName(type), value(val)
	{
		varName = "const_" + std::to_string(counter);
		id = counter++;
	}

	//void updateEveryFrame() {
	//	// Update typeName string based on currentType
	//	static const char* types[] = { "float", "vec2", "vec3", "vec4" };
	//	if (currentType >= 0) {
	//		typeName = types[currentType];
	//	}
	//}

	void drawUI() override {
		// First we'll call updateEveryFrame
		//updateEveryFrame();
		
		ImNodes::BeginNode(id);

		// -- Node title Bar -- 
		ImNodes::BeginNodeTitleBar();
		ImGui::TextUnformatted("Constant Node");
		ImNodes::EndNodeTitleBar();

		ImGui::BeginGroup();

		// -- Type Selector -- 
		const char* types[] = { "float", "vec2", "vec3", "vec4" };
		ImGui::PushItemWidth(150); // width of type selector
		if (ImGui::Combo("##Type", &currentType, types, IM_ARRAYSIZE(types))) {
			typeName = types[currentType];
			// Reset value if type changes
			value = glm::vec4(0.0f);
		}

		// -- Value inputs -- 
		ImGui::PushItemWidth(250); // width of value inputs
		if (typeName == "float") ImGui::InputFloat("##Value", &value.x);
		else if (typeName == "vec2") ImGui::InputFloat2("##Value", &value.x);
		else if (typeName == "vec3") ImGui::InputFloat3("##Value", &value.x);
		else if (typeName == "vec4") ImGui::InputFloat4("##Value", &value.x);
		ImGui::PopItemWidth();

		ImGui::EndGroup();

		// -- Output attribute (for connecting links) --
		ImNodes::BeginOutputAttribute(id * 10);
		ImGui::Text("out");
		ImNodes::EndOutputAttribute();

		ImNodes::EndNode();
	}

	std::string getOutputVar() {
		return varName;
	}

	std::string getTypeName() override {
		return typeName;
	}

	std::string getGLSL() override {
		// Generate final string to be used for GLSL code 
		// Example 'float <varName> = 
		if (typeName == "float")
			return "float " + varName + " = " + std::to_string(value.x) + ";\n";
		else if (typeName == "vec2")
			return "vec2 " + varName + " = " + "vec2(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ");\n";
		else if (typeName == "vec3")
			return "vec3" + varName + " = " + "vec3(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + ");\n";
		else if (typeName == "vec4")
			return "vec4 " + varName + " = " + "vec4(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + ", " + std::to_string(value.w) + ");\n";
		return "";
	}
};

#endif
