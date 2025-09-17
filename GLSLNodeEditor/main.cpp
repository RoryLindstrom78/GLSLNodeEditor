#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imnodes.h"
#include "Node.h"
#include "AddNode.h"
#include "ConstantNode.h"
#include "MultiplyNode.h"
#include "DivideNode.h"


#include <functional>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

std::vector<NodeLink> links;       // all current connections
std::vector<Node*> nodes;

int nextNodeID = 1;
int nextLinkID = 1;

static void glfw_error_callback(int error, const char* description);
Node* findNodeByOutputID(int outputID);
void updateEveryFrame(std::vector<Node*>& nodes, const std::vector<NodeLink>& links);
std::vector<std::string> topologicalSort(const std::vector<Node*>& nodes);


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImNodes Example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup ImNodes
    ImNodes::CreateContext();
    ImNodes::StyleColorsDark();

    bool show_demo = true;
    
    // Setup a new node
    ConstantNode node1 = ConstantNode();
    ConstantNode node2 = ConstantNode();
    DivideNode node3 = DivideNode();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Our Node Editor window
        ImGui::Begin("Node Editor");

        ImNodes::BeginNodeEditor();


        // --- Node 1 ---
        node1.drawUI();
        nodes.push_back(&node1); // every node added must be added to the nodes vector
        node2.drawUI();
        nodes.push_back(&node2);
        node3.drawUI();
        nodes.push_back(&node3);

        int start_attr, end_attr;
        for (const auto& link : links) {
            ImNodes::Link(link.id, link.start_attr, link.end_attr);
        }

        ImNodes::EndNodeEditor();

        // poll new links
        if (ImNodes::IsLinkCreated(&start_attr, &end_attr)) {
            links.push_back({ nextLinkID++, start_attr, end_attr });
        }

        ImGui::End(); // Node Editor window

        updateEveryFrame(nodes, links);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Topologically Sort Nodes
    std::vector<std::string> code = topologicalSort(nodes);
    for (int i = 0; i < code.size(); i++) {
        std::cout << code[i];
    }

    // Cleanup
    ImNodes::DestroyContext();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

Node* findNodeByOutputID(int outputID) {
    for (auto* node : nodes) {
        if (node->id * 10 == outputID) // or however you define output attribute IDs
            return node;
    }
    return nullptr;
}

void updateEveryFrame(std::vector<Node*>& nodes, const std::vector<NodeLink>& links) {
    // Create a map: output attribute -> node
    std::unordered_map<int, Node*> outputMap;
    for (auto* node : nodes) {
        outputMap[node->id * 10] = node; // assuming output attribute id = id * 10
    }

    // Reset all inputs
    for (auto* node : nodes) {
        auto inputIDs = node->getInputIDs();
        for (int i = 0; i < inputIDs.size(); ++i) {
            node->setInput(i, nullptr);
        }
    }

    // Update input pointers based on links
    for (const auto& link : links) {
        Node* srcNode = outputMap[link.start_attr];
        if (!srcNode) continue;

        for (auto* node : nodes) {
            auto inputIDs = node->getInputIDs();
            for (int i = 0; i < inputIDs.size(); ++i) {
                if (inputIDs[i] == link.end_attr) {
                    node->setInput(i, srcNode);
                }
            }
        }
    }
}

std::vector<std::string> topologicalSort(const std::vector<Node*>& nodes) {
    std::vector<std::string> code;

    // Reset states before traversal
    for (auto* node : nodes) {
        node->State = VisitState::Unvisited;
    }

    // Local DFS function  
    std::function<void(Node*)> dfs = [&](Node* node) {
        if (node->State == VisitState::Visiting) {
            std::cerr << "Cycle detected in DFS" << std::endl;
            return;
        }

        if (node->State == VisitState::Visited) return;

        node->State = VisitState::Visiting;

        for (Node* input : node->inputs) {
            if (input) dfs(input);
        }

        node->State = VisitState::Visited;
        code.push_back(node->getGLSL());
    };
    
    for (Node* node : nodes) {
        if (node->State == VisitState::Unvisited) {
            dfs(node);
        }
    }

    return code;
}