#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <locale.h>
#include <queue>
#include <stack>
#include <set>

using namespace std;

struct Edge {
    string u, v;
    int weight;
    Edge(string _u, string _v, int _w) : u(_u), v(_v), weight(_w) {}
};

class DSU {
private:
    map<string, string> parent;
    map<string, int> rank;

public:
    void makeSet(string peak) {
        parent[peak] = peak;
        rank[peak] = 0;
    }
    string find(string peak) {
        if (parent[peak] != peak) {
            parent[peak] = find(parent[peak]);
        }
        return parent[peak];
    }

    void unionSets(string u, string v) {
        string rootU = find(u);
        string rootV = find(v);

        if (rootU != rootV) {
            if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            }
            else if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            }
            else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }
};

bool compareEdges(const Edge& a, const Edge& b) {
    if (a.weight != b.weight) return a.weight < b.weight;
    if (a.u != b.u) return a.u < b.u;
    return a.v < b.v;
}

void bubbleSort(vector<Edge>& edges) {
    int n = edges.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (!compareEdges(edges[j], edges[j + 1])) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }
}

void readGraphFromFile(const string& filename, vector<string>& peaks, vector<Edge>& edges, map<string, vector<pair<string, int>>>& adjacencyList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл: " << filename << endl;
        exit(1);
    }

    string line;
    edges.clear();
    adjacencyList.clear();

    // Чтение вершин
    getline(file, line);
    stringstream ss(line);
    string peak;

    peaks.clear();
    while (ss >> peak) {
        peaks.push_back(peak);
        adjacencyList[peak] = vector<pair<string, int>>();
    }

    int n = peaks.size();
    if (n > 50) {
        cerr << "Превышен максимальный размер графа (50 вершин)" << endl;
        exit(1);
    }

    // Чтение матрицы смежности
    vector<vector<int>> adjacencyMatrix(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        if (!getline(file, line)) {
            cerr << "Неожиданный конец файла" << endl;
            exit(1);
        }

        stringstream row_ss(line);
        for (int j = 0; j < n; j++) {
            if (!(row_ss >> adjacencyMatrix[i][j])) {
                cerr << "Ошибка чтения матрицы смежности" << endl;
                exit(1);
            }
        }
    }

    file.close();

    // Создание рёбер и списка смежности
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int weight = adjacencyMatrix[i][j];
            if (weight > 0) {
                if (weight < 1 || weight > 1023) {
                    cerr << "Вес ребра должен быть в диапазоне от 1 до 1023" << endl;
                    exit(1);
                }
                string u = peaks[i];
                string v = peaks[j];

                edges.push_back(Edge(u, v, weight));

                adjacencyList[u].push_back({ v, weight });
                adjacencyList[v].push_back({ u, weight });
            }
        }
    }
}

// Обход в глубину (DFS)
void DFS(const map<string, vector<pair<string, int>>>& adjacencyList, const string& startVertex) {
    cout << "Обход в глубину (DFS) начиная с вершины " << startVertex << ":" << endl;

    vector<string> traversalOrder;
    set<string> visited;
    stack<string> s;

    s.push(startVertex);

    while (!s.empty()) {
        string current = s.top();
        s.pop();

        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            traversalOrder.push_back(current);

            const vector<pair<string, int>>& neighbors = adjacencyList.at(current);
            for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
                if (visited.find(it->first) == visited.end()) {
                    s.push(it->first);
                }
            }
        }
    }
    for (size_t i = 0; i < traversalOrder.size(); i++) {
        cout << traversalOrder[i];
        if (i < traversalOrder.size() - 1) cout << " - ";
    }
    cout << endl << endl;
}

// Обход в ширину (BFS)
void BFS(const map<string, vector<pair<string, int>>>& adjacencyList, const string& startVertex) {
    cout << "Обход в ширину (BFS) начиная с вершины " << startVertex << ":" << endl;
    vector<string> traversalOrder;
    set<string> visited;
    queue<string> q;

    q.push(startVertex);
    visited.insert(startVertex);

    while (!q.empty()) {
        string current = q.front();
        q.pop();
        traversalOrder.push_back(current);

        const vector<pair<string, int>>& neighbors = adjacencyList.at(current);
        for (const auto& neighbor : neighbors) {
            if (visited.find(neighbor.first) == visited.end()) {
                visited.insert(neighbor.first);
                q.push(neighbor.first);
            }
        }
    }
    for (size_t i = 0; i < traversalOrder.size(); i++) {
        cout << traversalOrder[i];
        if (i < traversalOrder.size() - 1) cout << " - ";
    }
    cout << endl << endl;
}

void Kruskal(const vector<string>& peaks, const vector<Edge>& edges, vector<Edge>& mst) {
    // Создаем копию рёбер для сортировки
    vector<Edge> sortedEdges = edges;

    bubbleSort(sortedEdges);

    DSU dsu;
    for (const auto& peak : peaks) {
        dsu.makeSet(peak);
    }

    mst.clear();

    for (const auto& edge : sortedEdges) {
        if (dsu.find(edge.u) != dsu.find(edge.v)) {
            dsu.unionSets(edge.u, edge.v);
            mst.push_back(edge);

            if (mst.size() == peaks.size() - 1) {
                break;
            }
        }
    }
}

int calculateTotalWeight(const vector<Edge>& mst) {
    int total = 0;
    for (const auto& edge : mst) {
        total += edge.weight;
    }
    return total;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    cout << "Алгоритм Краскала" << endl;
    cout << endl;

    const string filename = "graph.txt";

    vector<string> peaks;
    vector<Edge> edges;
    map<string, vector<pair<string, int>>> adjacencyList;

    readGraphFromFile(filename, peaks, edges, adjacencyList);

    if (peaks.empty()) {
        cout << "Граф пуст или не содержит вершин" << endl;
        return 0;
    }

    cout << "Рёбра графа:" << endl;
    for (const auto& edge : edges) {
        cout << edge.u << " - " << edge.v << "  " << edge.weight << endl;
    }
    cout << endl;

    if (peaks.empty()) {
        cout << "Граф пуст" << endl;
        return 0;
    }

    string startVertex = peaks[0];

    DFS(adjacencyList, startVertex);
    BFS(adjacencyList, startVertex);
    vector<Edge> mst;
    Kruskal(peaks, edges, mst);

    cout << "Минимальный остов:" << endl;
    for (const auto& edge : mst) {
        cout << edge.u << " " << edge.v << endl;
    }

    int totalWeight = calculateTotalWeight(mst);
    cout << totalWeight << endl;

    return 0;
}