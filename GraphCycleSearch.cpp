#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <string>
#define MAX_GRAPH_SIZE 8
#define MIN_GRAPH_SIZE 1
#define MAX_CYCLES_CAPACITY 64
#define MIN_CHOICE_OPTION 1
#define MAX_CHOICE_OPTION 6
#define PROPORTION 0.6
#define ClearScreen() system("cls")
#define TEMPLATE1 {{0, 1}, {1, 2}, {2, 0}}                                                                             //Рис. 1
#define TEMPLATE2 {{0, 1}, {1, 2}, {2, 0}, {1, 0}}                                                                     //Рис. 2
#define TEMPLATE3 {{0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 3}, {4, 5}}                                                     //Рис. 3
#define TEMPLATE4 {{0, 1}, {0, 2}, {2, 0}, {1, 2}, {1, 3}, {2, 3}, {4, 5}, {5, 4}}                                     //Рис. 4
#define TEMPLATE5 {{0, 1}, {0, 2}, {2, 0}, {1, 2}, {1, 3}, {2, 3}, {3, 2}, {4, 5}, {5, 4}}                             //Рис. 5

struct Edge {
    int Source, Destination;
};

class Graph {
    int n;
public:
    Graph();                                                                                                            
    Graph(std::vector<Edge> const& Edges, int n);
    Graph(std::vector <std::vector <int>> UserAdjacentLists, int n);
    void ManualInput();
    void TemplateInput();
    void PrintGraph();
    void СyclesSearch();
private:
    void FillGraphWithEdges(std::vector<Edge> const& Edges, int n);
    void FillGraphWithAdjacementLists(std::vector <std::vector <int>> UserAdjacentLists, int n);
    void RandomGeneration();
    void DFSСycle(int CurrentVertex, int EndVertex, std::vector <int> Visited, std::list <int> Cycle);
    std::vector < std::list<int>> CyclesList;
    bool CycleAlreadyExists(std::list <int> CurrentCycle);
    void CyclesListOut();
    std::vector <std::vector <int>> AdjacentLists;          //Граф хранится как вектор списков смежности каждой вершины
};

Graph::Graph() { n = 0; }

Graph::Graph(std::vector<Edge> const& Edges, int n)
    {
        AdjacentLists.resize(n);
        for (auto& Edge : Edges)
        {
            AdjacentLists[Edge.Source].push_back(Edge.Destination);
        }
        this->n = n;
    }

Graph::Graph(std::vector <std::vector <int>> UserAdjacentLists, int n) {
    AdjacentLists = UserAdjacentLists;
    this->n = n;
}

void Graph::ManualInput() {
    std::cout << "Пожалуйста, введите количество вершин в графе (1-16): ";
    std::cin >> n;
    while ((n < MIN_GRAPH_SIZE) || (n > MAX_GRAPH_SIZE)) {
        std::cout << "Пожалуйста, введите число в диапазоне (0-16): ";
        std::cin >> n;
    }
    std::cout << "Ввод производится в виде списков смежности.\n";
    for (size_t i = 0; i != n; ++i) {
        int AdjacentVertexesCount;
        std::vector <int> UserAdjacentList;
        std::cout << "Количество вершин, смежных с вершиной " << i<< " (0 - " << n - 1 << "): ";
        std::cin >> AdjacentVertexesCount;
        while ((AdjacentVertexesCount < MIN_GRAPH_SIZE - 1) || (AdjacentVertexesCount > n - 1)) {
            std::cout << "Пожалуйста, введите число в диапазоне (0-" << n - 1 << "): ";
            std::cin >> AdjacentVertexesCount;
        }
        if (AdjacentVertexesCount != 0) {
            std::cout << "Вершины, смежные с вершиной " << i << " (0-" << n - 1 << ") " << ": ";
            for (size_t j = 0; j != AdjacentVertexesCount; ++j) {
                int Vertex;
                std::cin >> Vertex;
                if (Vertex < n) {
                    UserAdjacentList.push_back(Vertex);
                }
                else {
                    std::cout << "Вершины " << Vertex << " не существует!\n";
                }
            }
        }
        AdjacentLists.push_back(UserAdjacentList);
        UserAdjacentList.clear();
    }
    (*this).FillGraphWithAdjacementLists(AdjacentLists, n);
}

void Graph::TemplateInput() {
    int Choice;
    std::cout << "Графическое представление всех шаблонов прилагается в отчете\n";
    std::cout << "1. Шаблон 1\n2. Шаблон 2\n3. Шаблон 3\n4. Шаблон 4\n5. Шаблон 5\n6. Случайная генерация\nВыбор: ";
    std::cin >> Choice;
    while ((Choice < MIN_CHOICE_OPTION) || (Choice > MAX_CHOICE_OPTION)) {
        std::cout << "Пожалуйста, введите число в диапазоне (0-16): ";
        std::cin >> Choice;
    }
    switch (Choice) {
    case 1: {
        std::vector<Edge> Edges = TEMPLATE1;
        n = 3;
        (*this).FillGraphWithEdges(Edges, n);
        break;
    }
    case 2: {
        std::vector<Edge> Edges = TEMPLATE2;
        n = 3;
        (*this).FillGraphWithEdges(Edges, n);
        break;
    }
    case 3: {
        std::vector<Edge> Edges = TEMPLATE3;
        n = 6;
        (*this).FillGraphWithEdges(Edges, n);
        break;
    }
    case 4: {
        std::vector<Edge> Edges = TEMPLATE4;
        n = 6;
        (*this).FillGraphWithEdges(Edges, n);
        break;
    }
    case 5: {
        std::vector<Edge> Edges = TEMPLATE5;
        n = 6;
        (*this).FillGraphWithEdges(Edges, n);
        break;
    }
    case 6: {
        (*this).RandomGeneration();
    }
    }
}

void Graph::RandomGeneration() {
    srand(time(nullptr));
    int n = rand() % MAX_GRAPH_SIZE + MIN_GRAPH_SIZE;
    this->n = n;
    std::vector<std::vector <int>> RandomAdjacentLists (n);
    for (int i = 0; i != n; ++i) {
        for (int j = 0; j != n; ++j) {
            bool vertex = ((double)(rand()) / RAND_MAX >= PROPORTION);
            if (vertex) {
                RandomAdjacentLists.at(i).push_back(j);
            }
        }
    }
    AdjacentLists = RandomAdjacentLists;
}

void Graph::FillGraphWithEdges(std::vector<Edge> const& Edges, int n) {
    AdjacentLists.resize(n);
    for (auto& Edge : Edges)
    {
        AdjacentLists[Edge.Source].push_back(Edge.Destination);
    }
    this->n = n;
}

void Graph::FillGraphWithAdjacementLists(std::vector <std::vector <int>> UserAdjacentLists, int n) {
    AdjacentLists = UserAdjacentLists;
    this->n = n;
}

void Graph::PrintGraph()
    {
    std::cout << "Граф в виде списков смежности:\n";
        for (int i = 0; i < this->n; i++)
        {
            std::cout << i << " --> ";
            for (int v : this->AdjacentLists[i]) {
                std::cout << v << " ";
            }
            std::cout << std::endl;
        }
    }

void Graph::СyclesSearch()
    {
        std::vector <int> Visited(this->n);
        for (int i = 0; i < this->n; i++)
        {
            for (int k = 0; k < this->n; k++)
                Visited[k] = 0;
            std::list <int> Cycle;
            Cycle.push_back(i);
            DFSСycle(i, i, Visited, Cycle);
        }
        CyclesListOut();
    }

void Graph::DFSСycle(int CurrentVertex, int EndVertex, std::vector <int> Visited, std::list <int> Cycle){
    if (CurrentVertex != EndVertex)
    Visited[CurrentVertex] = 1; // 0 - нет, 1 - да
    else if (Cycle.size() >= 2) {
        if (!CycleAlreadyExists(Cycle)) {
            CyclesList.push_back(Cycle);
        }
        return;
    }
    for (int NextVertex : AdjacentLists[CurrentVertex]) {
        if (Visited[NextVertex] == 0) {
            std::list<int> NextVertexCycle(Cycle);
            NextVertexCycle.push_back(NextVertex);
            DFSСycle(NextVertex, EndVertex, Visited, NextVertexCycle);
            Visited[NextVertex] = 0;
        }
    }
}

bool Graph::CycleAlreadyExists(std::list <int> CurrentCycle) {
    std::vector <bool> Cycle1BitArray(MAX_GRAPH_SIZE);
    if (!CyclesList.empty()) {
        for (int Vertex : CurrentCycle) {
            Cycle1BitArray[Vertex] = 1;
        }
        for (auto CycleListInstance: CyclesList) {
            std::vector <bool> Cycle2BitArray(MAX_GRAPH_SIZE);
            for (int Vertex : CycleListInstance) {
                Cycle2BitArray[Vertex] = 1;
                }
            //std::copy(Cycle2BitArray.begin(), Cycle2BitArray.end(), std::ostream_iterator<int>(std::cout, " "));
            if (Cycle1BitArray != Cycle2BitArray) {
                continue;
            }
            else {
                return true;
            }
        }
    }
    else {
        return false;
    }
    return false;
}

void Graph::CyclesListOut() {
    if (!CyclesList.empty()) {
        std::cout << "\nЭлементарные циклы в графе:\n";
        for (auto Cycles : CyclesList) {
            std::copy(Cycles.begin(), Cycles.end(), std::ostream_iterator<int>(std::cout, "-"));
            printf_s("\b \b");
            std::cout << std::endl;
        }
    }
    else {
        std::cout << "\nВ графе нет элементарных циклов!\n";
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    Graph graph;
    std::vector <std::vector <int>> AdjacentLists;
    int n = 0;
    int ManualInput;
    std::cout << "Данная программа найдет все элементарные циклы в графе!\n\nПожалуйтса, выберите способ ввода графа:\n1. Вручную\n2. Выбрать шаблон или сгенерировать случайно.\nВыбор: ";
    std::cin >> ManualInput;
    ClearScreen();
    if (ManualInput == 1) {
        graph.ManualInput();
    }
    else {
        graph.TemplateInput();
    }
    ClearScreen();
    graph.PrintGraph();
    graph.СyclesSearch();
    //std::cin.get();
    return 0;
}