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

#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <algorithm>

#include "graphedge.h"
#include "graphnode.h"
#include "chatbot.h"
#include "chatlogic.h"


ChatLogic::ChatLogic() {
    _currentNode = nullptr;
    _chatBot = nullptr;
    _panelDialog = nullptr;

}

ChatLogic::~ChatLogic() = default;

template<typename T>
void ChatLogic::AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element) {
    // find all occurrences for current node
    auto token = tokens.begin();
    while (true) {
        token = std::find_if(token, tokens.end(), [&tokenID](const std::pair<std::string, std::string> &pair) {
            return pair.first == tokenID;;
        });
        if (token != tokens.end()) {
            element.AddToken(token->second); // add new keyword to edge
            token++;                         // increment iterator to next element
        } else {
            break; // quit infinite while-loop
        }
    }
}

void ChatLogic::LoadAnswerGraphFromFile(const std::string& filename) {
    // load file with answer graph elements
    std::ifstream file(filename);

    // check for file availability and process it line by line
    if (file) {
        // loop over all lines in the file
        std::string lineStr;
        while (getline(file, lineStr)) {
            // extract all tokens from current line
            tokenlist tokens;
            while (!lineStr.empty()) {
                // extract next token
                int posTokenFront = lineStr.find("<");
                int posTokenBack = lineStr.find(">");
                if (posTokenFront < 0 || posTokenBack < 0)
                    break; // quit loop if no complete token has been found
                std::string tokenStr = lineStr.substr(posTokenFront + 1, posTokenBack - 1);

                // extract token type and info
                int posTokenInfo = tokenStr.find(":");
                if (posTokenInfo != std::string::npos) {
                    std::string tokenType = tokenStr.substr(0, posTokenInfo);
                    std::string tokenInfo = tokenStr.substr(posTokenInfo + 1, tokenStr.size() - 1);

                    // add token to vector
                    tokens.push_back(std::make_pair(tokenType, tokenInfo));
                }

                // remove token from current line
                lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
            }

            // process tokens for current line
            auto type = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) {
                return pair.first == "TYPE";
            });
            if (type != tokens.end()) {
                // check for id
                auto idToken = std::find_if(tokens.begin(), tokens.end(),
                                            [](const std::pair<std::string, std::string> &pair) {
                                                return pair.first == "ID";
                                            });
                if (idToken != tokens.end()) {
                    // extract id from token
                    int id = std::stoi(idToken->second);

                    // node-based processing
                    if (type->second == "NODE") {


                        // check if node with this ID exists already

                        auto newNode = std::find_if(_nodes.begin(), _nodes.end(),
                                                    [&id](std::unique_ptr<GraphNode> &node) {
                                                        return node->GetID() == id;
                                                    });

                        // create new element if ID does not yet exist
                        if (newNode == _nodes.end()) {
                            _nodes.emplace_back(new GraphNode(id));
                            newNode = _nodes.end() - 1; // get iterator to last element

                            // add all answers to current node
                            AddAllTokensToElement("ANSWER", tokens, **newNode);
                        }

                    }

                    // edge-based processing
                    if (type->second == "EDGE") {


                        // find tokens for incoming (parent) and outgoing (child) node
                        auto parentToken = std::find_if(tokens.begin(), tokens.end(),
                                                        [](const std::pair<std::string, std::string> &pair) {
                                                            return pair.first == "PARENT";
                                                        });
                        auto childToken = std::find_if(tokens.begin(), tokens.end(),
                                                       [](const std::pair<std::string, std::string> &pair) {
                                                           return pair.first == "CHILD";
                                                       });

                        if (parentToken != tokens.end() && childToken != tokens.end()) {
                            // get iterator on incoming and outgoing node via ID search
                            auto parentNode = std::find_if(_nodes.begin(), _nodes.end(),
                                                           [&parentToken](std::unique_ptr<GraphNode> &node) {
                                                               return node->GetID() == std::stoi(parentToken->second);
                                                           });
                            auto childNode = std::find_if(_nodes.begin(), _nodes.end(),
                                                          [&childToken](std::unique_ptr<GraphNode> &node) {
                                                              return node->GetID() == std::stoi(childToken->second);
                                                          });

                            // create new edge
                            std::unique_ptr<GraphEdge> edge = std::make_unique<GraphEdge>(id);
                            edge->SetChildNode((*childNode).get());
                            edge->SetParentNode((*parentNode).get());

                            // find all keywords for current node
                            AddAllTokensToElement("KEYWORD", tokens, *edge);

                            // store reference in child node and parent node
                            (*childNode)->AddEdgeToParentNode(edge.get());
                            (*parentNode)->AddEdgeToChildNode(std::move(edge));
                        }

                    }
                } else {
                    std::cout << "Error: ID missing. Line is ignored!" << std::endl;
                }
            }
        } // eof loop over all lines in the file

        file.close();

    } // eof check for file availability
    else {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }



    // identify root node
    GraphNode *rootNode = nullptr;
    for (auto & _node : _nodes) {
        // search for nodes which have no incoming edges
        if (_node->GetNumberOfParents() == 0) {

            if (rootNode == nullptr) {
                rootNode = _node.get(); // assign current node to root
            } else {
                std::cout << "ERROR : Multiple root nodes detected" << std::endl;
            }
        }
    }

    // add chatbot to graph root node
    auto chatBot = ChatBot("../images/chatbot.png");
    chatBot.SetChatLogicHandle(this);
    chatBot.SetRootNode(rootNode);
    rootNode->MoveChatbotHere(std::move(chatBot));
}

void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog *panelDialog) {
    _panelDialog = panelDialog;
}

void ChatLogic::SetChatbotHandle(ChatBot *chatbot) {
    _chatBot = chatbot;
}

void ChatLogic::SendMessageToChatbot(std::string message) {
    _chatBot->ReceiveMessageFromUser(std::move(message));
}

void ChatLogic::SendMessageToUser(std::string message) {
    _panelDialog->PrintChatbotResponse(std::move(message));
}

wxBitmap *ChatLogic::GetImageFromChatbot() {
    return _chatBot->GetImageHandle();
}
