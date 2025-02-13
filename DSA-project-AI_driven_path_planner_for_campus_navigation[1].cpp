#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits.h>
#include <string>

using namespace std;

class Graph {
private:
    unordered_map<string, vector<pair<string, int>>> adjList;

public:
    void addEdge(const string& u, const string& v, int weight) {
        adjList[u].emplace_back(v, weight);
        adjList[v].emplace_back(u, weight);
    }

    vector<string> getBuildings() const {
        vector<string> buildings;
        for (const auto& entry : adjList) {
            buildings.push_back(entry.first);
        }
        return buildings;
    }

    class PriorityQueue {
    private:
        vector<pair<int, string>> elements;

        void swap(int i, int j) {
            auto temp = elements[i];
            elements[i] = elements[j];
            elements[j] = temp;
        }

        void heapifyDown(int index) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < elements.size() && elements[left].first < elements[smallest].first) {
                smallest = left;
            }

            if (right < elements.size() && elements[right].first < elements[smallest].first) {
                smallest = right;
            }

            if (smallest != index) {
                swap(index, smallest);
                heapifyDown(smallest);
            }
        }

        void heapifyUp(int index) {
            if (index == 0) return;
            int parent = (index - 1) / 2;
            if (elements[parent].first > elements[index].first) {
                swap(parent, index);
                heapifyUp(parent);
            }
        }

    public:
        void push(const pair<int, string>& element) {
            elements.push_back(element);
            heapifyUp(elements.size() - 1);
        }

        pair<int, string> pop() {
            if (elements.empty()) throw runtime_error("Priority queue is empty");
            pair<int, string> top = elements.front();
            elements[0] = elements.back();
            elements.pop_back();
            heapifyDown(0);
            return top;
        }

        bool empty() const {
            return elements.empty();
        }
    };

    void dijkstra(const string& src, const string& dest) {
        if (adjList.find(src) == adjList.end() || adjList.find(dest) == adjList.end()) {
            cout << "Invalid building name. Please check the input." << endl;
            return;
        }

        if (src == dest) {
            cout << "\nOpen your eyes. Use your mind. You are already at your destination." << endl;
            return;
        }

        unordered_map<string, int> distances;
        unordered_map<string, string> parent;

        for (const auto& node : adjList) {
            distances[node.first] = INT_MAX;
        }
        distances[src] = 0;

        PriorityQueue pq;
        pq.push({0, src});

        while (!pq.empty()) {
            pair<int, string> current = pq.pop();
            int dist = current.first;
            string u = current.second;

            for (const auto& neighbor : adjList[u]) {
                string v = neighbor.first;
                int weight = neighbor.second;

                if (dist + weight < distances[v]) {
                    distances[v] = dist + weight;
                    pq.push({distances[v], v});
                    parent[v] = u;
                }
            }
        }

        if (distances[dest] == INT_MAX) {
            cout << "No path found from " << src << " to " << dest << endl;
            return;
        }

        displayPath(src, dest, distances[dest], parent);
    }

    void displayPath(const string& src, const string& dest, int distance, unordered_map<string, string>& parent) {
        cout << "\nShortest path from " << src << " to " << dest << " is " << distance << " units." << endl;
        string path = dest;
        string temp = dest;

        while (parent.find(temp) != parent.end()) {
            path = parent[temp] + " -> " + path;
            temp = parent[temp];
        }

        cout << "Path: " << path << endl;
    }
};

void initializeGraph(Graph& g) {
    g.addEdge("IST Mosque", "IST Girls Hostel", 2);
    g.addEdge("IST Mosque", "Block 2", 2);
    g.addEdge("Block 2", "Lawn", 2);
    g.addEdge("Lawn", "Raza Block", 2);
    g.addEdge("Block 2", "Gym", 2);
    g.addEdge("Gym", "Block 3", 2);
    g.addEdge("Block 3", "Block 6", 2);
    g.addEdge("Block 6", "Block 7", 2);
    g.addEdge("Block 7", "Main Gate", 2);
    g.addEdge("Block 6", "Library Mess", 2);
    g.addEdge("Library Mess", "Faculty Hostel", 2);
    g.addEdge("Library Mess", "IST Boys Hostel", 2);
    g.addEdge("Library Mess", "Canteen", 2);
    g.addEdge("Block 6", "Canteen", 2);
    g.addEdge("Canteen", "Raza Block", 2);
    g.addEdge("Raza Block", "Main Gate", 2);
    g.addEdge("Main Gate", "Lawn", 2);
    g.addEdge("IST Girls Hostel", "IST Cricket Ground", 2);
    g.addEdge("IST Cricket Ground", "IST Boys Hostel", 2);
    g.addEdge("IST Cricket Ground", "Faculty Hostel", 2);
    g.addEdge("IST Cricket Ground", "Library Mess", 2);
}

void welcomeMessage() {
    cout << "=============================================" << endl;
    cout << "  Welcome to AI-Driven Path Planner for IST Campus Navigation!" << endl;
    cout << "=============================================" << endl;
}

string getBuildingInput(const string& prompt, const vector<string>& buildings) {
    cout << "\n" << prompt << endl;
    for (size_t i = 0; i < buildings.size(); ++i) {
        cout << i + 1 << ". " << buildings[i] << endl;
    }
    int choice;
    cout << "Enter the number corresponding to your choice: ";
    cin >> choice;

    while (choice < 1 || choice > buildings.size()) {
        cout << "Invalid choice. Please try again: ";
        cin >> choice;
    }

    return buildings[choice - 1];
}

int main() {
    Graph g;
    initializeGraph(g);

    welcomeMessage();

    vector<string> buildings = g.getBuildings();

    while (true) {
        string start = getBuildingInput("Select your starting point:", buildings);
        string end = getBuildingInput("Select your destination point:", buildings);

        g.dijkstra(start, end);

        cout << "\nDo you want to find another path? (yes/no): ";
        string choice;
        cin >> choice;

        while (choice != "yes" && choice != "no") {
            cout << "Invalid choice. Please enter 'yes' or 'no': ";
            cin >> choice;
        }

        if (choice == "no") {
            break;
        }
    }

    cout << "\nHave a safe journey! Wear your ID cards and beware of TP.\n";
    return 0;
}