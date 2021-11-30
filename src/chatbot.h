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

#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>
#include <string>

class GraphNode; // forward declaration
class ChatLogic; // forward declaration

class ChatBot {
private:
    // data handles (owned)
    wxBitmap *_image; // avatar image

    // data handles (not owned)
    GraphNode *_currentNode;
    GraphNode *_rootNode;
    ChatLogic *_chatLogic;

    // proprietary functions
    int ComputeLevenshteinDistance(std::string s1, std::string s2);

public:
    // constructors / destructors
    ChatBot();                     // constructor WITHOUT memory allocation
    explicit ChatBot(const std::string &filename); // constructor WITH memory allocation
    ~ChatBot();

    ChatBot(const ChatBot &source); // copy constructor
    ChatBot &operator=(const ChatBot &source); // copy assignment operator
    ChatBot(ChatBot &&source) noexcept; // move constructor
    ChatBot &operator=(ChatBot &&source) noexcept; // move assignment operator

    // getters / setters
    void SetCurrentNode(GraphNode *node);

    void SetRootNode(GraphNode *rootNode) { _rootNode = rootNode; }

    void SetChatLogicHandle(ChatLogic *chatLogic) { _chatLogic = chatLogic; }

    ChatLogic *GetChatLogicHandle() { return _chatLogic; }

    wxBitmap *GetImageHandle() { return _image; }

    // communication
    void ReceiveMessageFromUser(const std::string &message);
};

#endif /* CHATBOT_H_ */
