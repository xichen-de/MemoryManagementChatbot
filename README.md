# Memory Management Chat Bot

## Description
![CI](https://github.com/xichen-de/MemoryManagementChatbot/actions/workflows/docker.yml/badge.svg)

The ChatBot code creates a dialog where users can ask questions about some aspect of memory management in C++. After the chatbot's knowledge base is loaded from a text file, a knowledge graph representation is created in computer memory, with the chatbot's answers representing the nodes of the graph and the user queries representing the edges of the graph. After a user query is sent to the chatbot, the Levenshtein distance is used to determine the most likely response. 

The project uses the memory management techniques in C++ 11, namely RAII, move semantics and smart pointers. It is developed and tested on Ubuntu 20.04.

![example](README.assets/example.png)

## Usage

### Intall dependencies
- cmake >= 3.11
- make >= 4.1
- gcc/g++ >= 5.4
- wxWidgets >= 3.0

### Clone the repo with Git
```
git clone https://github.com/xichen-de/MemoryManagementChatbot.git
``` 

### Compiling

```
mkdir build && cd build
cmake ..
make
```

### Running

The executable will be placed in the `build` directory. From within `build`, you can run the project as follows:

```
./membot
```
