#ifndef NODE_H
#define NODE_H 

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <iostream>

// Forward declaration
template<typename T>
class Node;

// Input holds a pointer to a Node producing type T
template<typename T>
struct Input {
    Node<T>* source = nullptr;
};

// Base Node class
template<typename T>
class Node {
public:
    std::vector<Input<T>> inputs;

    virtual ~Node() = default;

    // Compute output
    virtual T evaluate() = 0;
};

// Constant Node
template<typename T>
class ConstantNode : public Node<T> {
public:
    T constant;

    ConstantNode(T v) : constant(v) {}

    T evaluate() override {
        return constant;
    }
};

// Multiply Node
template<typename T1, typename T2, typename TResult = decltype(T1{} * T2{}) >
class MultiplyNode : public Node<TResult> {
public:
    Node<T1>* inputA;
    Node<T2>* inputB;

    MultiplyNode(Node<T1>* a, Node<T2>* b) : inputA(a), inputB(b) {}

    TResult evaluate() override {
        T1 a_val = inputA ? inputA->evaluate() : T1{};
        T2 b_val = inputB ? inputB->evaluate() : T2{};
        return a_val * b_val; // GLM operators handle float*vec3, vec3*float, etc.
    }
};

#endif