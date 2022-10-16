#include <iostream>
#include <climits>
#include <vector>
#include <stack>
#include <queue>
#include <numeric>

using namespace std;

class Edge {
public:
    int to;
    int weight;
    int from;

    explicit Edge(int from = -1, int to = -1, int weight = 0) : from(from), to(to), weight(weight) {}

    Edge(const Edge &edge) {
        from = edge.from;
        to = edge.to;
        weight = edge.weight;
    }

    int operator<(const Edge &edge) const {
        return (weight < edge.weight);
    }

    friend ostream &operator<<(ostream &out, Edge &edge);


};

ostream &operator<<(ostream &out, const Edge &edge) {
    out << "From: " << edge.from << ", to: " << edge.to << ", weight: " << edge.weight;
    return out;
}

void MST(vector<vector<int>> &matrix, vector<Edge> &tree_edges, int &mst_weight) {
    vector<bool> used(matrix.size(), false);
    vector<Edge> edges;

    edges.emplace_back(0, 0, 0);

    while (!edges.empty()) {
        Edge minEdge = edges.front();
        auto position = 0;

        for (auto i = 1; i < edges.size(); i++) {
            if (edges[i] < edges.front()) {
                minEdge = edges[i];
                position = i;
            }
        }

        edges.erase(edges.begin() + position);

        if (used[minEdge.to]) continue;

        used[minEdge.to] = true;
        mst_weight += minEdge.weight;
        tree_edges.push_back(minEdge);

        for (auto i = 0; i < matrix[minEdge.to].size(); i++) {
            if (matrix[minEdge.to][i] && !used[i]) edges.emplace_back(minEdge.to, i, matrix[minEdge.to][i]);
        }
    }
}

vector<int> DFS(const vector<vector<int>> &matrix) {
    vector<bool> used(matrix.size(), false);

    stack<int> s;
    s.push(0);

    vector<int> path;
    path.push_back(0);

    while (!s.empty()) {
        auto vertex = s.top();
        s.pop();

        if (used[vertex]) continue;

        used[vertex] = true;
        path.push_back(vertex);

        for (auto i = 0; i < matrix.size(); i++) {
            if (matrix[vertex][i] && !used[i]) s.push(i);
        }
    }
    return path;
}

vector<vector<int>> convertEdgeToMat(const vector<Edge> &edges) {
    vector<vector<int>> Matrix(edges.size(), vector<int>(edges.size(), 0));

    for (const auto &edge: edges) {
        Matrix[edge.from][edge.to] = edge.weight;
        Matrix[edge.to][edge.from] = edge.weight;
    }
    return Matrix;
}

vector<int> min_path(int source, const vector<vector<int>> &matrix) {
    vector<int> dist(matrix.size(), INT_MAX);
    dist[source] = 0;

    queue<int> q;
    q.push(source);

    while (!q.empty()) {
        auto vertex = q.front();
        q.pop();

        for (auto i = 0; i < matrix.size(); i++) {
            if (matrix[vertex][i] > 0 && matrix[vertex][i] < INT_MAX && dist[i] > dist[vertex] + matrix[vertex][i]) {
                dist[i] = dist[vertex] + matrix[vertex][i];
                q.push(i);
            }
        }
    }
    return dist;
}

vector<int> Degree(const vector<vector<int>> &matrix) {
    vector<int> dist(matrix.size(), INT_MAX);
    vector<bool> used(matrix.size(), false);
    queue<int> q;

    q.push(0);
    dist[0] = 0;

    vector<int> degrees;

    while (!q.empty()) {
        auto vertex = q.front();
        q.pop();

        if (used[vertex]) continue;

        used[vertex] = true;
        auto degree = 0;
        for (auto i = 0; i < matrix.size(); i++) {
            if (matrix[vertex][i] > 0 && !used[i]) {
                if (dist[i] > dist[vertex] + matrix[vertex][i]) dist[i] = dist[vertex] + matrix[vertex][i];
                q.push(i);
            }
            if (matrix[vertex][i] != 0) degree++;
        }
        degrees.push_back(degree);
    }
    return degrees;
}

int main() {
    vector<vector<int>> matrix = {
            {0, 6, 2, 1, 9, 1, 8, 1, 4, 8, 6, 1, 3},
            {6, 0, 2, 5, 1, 9, 9, 8, 1, 7, 9, 1, 1},
            {2, 2, 0, 4, 2, 2, 5, 3, 4, 6, 0, 3, 0},
            {1, 5, 4, 0, 1, 2, 4, 9, 4, 8, 8, 0, 9},
            {9, 1, 2, 1, 0, 3, 5, 4, 4, 4, 5, 4, 8},
            {1, 9, 2, 2, 3, 0, 2, 5, 1, 6, 9, 5, 8},
            {8, 9, 5, 4, 5, 2, 0, 7, 9, 3, 5, 9, 6},
            {1, 8, 3, 9, 4, 5, 7, 0, 5, 2, 0, 9, 2},
            {4, 1, 4, 4, 4, 1, 9, 5, 0, 6, 9, 2, 9},
            {8, 7, 6, 8, 4, 6, 3, 2, 6, 0, 9, 5, 4},
            {6, 9, 0, 8, 5, 9, 5, 0, 9, 9, 0, 5, 1},
            {1, 1, 3, 0, 4, 5, 9, 9, 2, 5, 5, 0, 0},
            {3, 1, 0, 9, 8, 8, 6, 2, 9, 4, 1, 0, 0},
    };

    vector<Edge> tree;
    auto weight = 0;
    MST(matrix, tree, weight);
    vector<vector<int>> matrixTree;
    matrixTree = convertEdgeToMat(tree);

    cout << "MST: " << endl;
    for (auto const &line: matrixTree) {
        for (int item: line) {
            cout << item << ' ';
        }
        cout << endl;
    }
    cout << "Weight: " << weight << endl << endl;

    vector<int> path;
    path = DFS(matrixTree);
    cout << "Path: ";
    for (int i = 1; i < path.size(); ++i) cout << path[i] << ' ';
    cout << endl << endl;

    vector<int> minPath;
    minPath = min_path(0, matrix);
    cout << "Min path:" << endl;
    for (int i = 1; i < minPath.size(); ++i) cout << "From 0 to " << i << " len: " << minPath[i] << endl;
    cout << endl;

    vector<int> degrees;
    degrees = Degree(matrixTree);

    cout << "Degree: " << endl;
    for (int i = 0; i < degrees.size(); ++i) cout << "Vertex " << i << ": " << degrees[i] << endl;
    cout << endl;

    auto meanDegree = 0;
    meanDegree += std::accumulate(degrees.begin(), degrees.end(), 0);
    meanDegree /= int(degrees.size());
    cout << "Mean degree: " << meanDegree << endl;
    return 0;
}
