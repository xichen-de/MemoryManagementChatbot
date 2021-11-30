// MIT License
//
// Copyright (c) 2021 Xi Chen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "graphedge.h"
#include "graphnode.h"

GraphNode::GraphNode(int id) {
    _id = id;
}

GraphNode::~GraphNode() {

}

void GraphNode::AddToken(std::string token) {
    _answers.push_back(token);
}

void GraphNode::AddEdgeToParentNode(GraphEdge *edge) {
    _parentEdges.push_back(edge);
}

void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> &&edge) {
    _childEdges.push_back(std::move(edge));
}


void GraphNode::MoveChatbotHere(ChatBot chatbot) {
    _chatBot = std::move(chatbot);
    _chatBot.SetCurrentNode(this);
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode) {
    newNode->MoveChatbotHere(std::move(_chatBot));
}


GraphEdge *GraphNode::GetChildEdgeAtIndex(int index) {

    return _childEdges[index].get();
}