/*
28. � ����� �����  Megasoft  �����������  N  �����������  � 
�������� �� 1 �� N, ��������� �� ���  ���������  �����  �����. 
��������� ����� ������������ ������������ �������� � �����  �� 
���� ����������� �� 1 �. ���������,  �������  ���������, ����� 
����������  ���  ����  �����������  �  ���  �����������.  C��� 
�������� ���, ��� ����� ������ ����� ������������  ����  ����, 
������ ������ ����. ����� ������ ���� �����������,  �  ������� 
������� ����������� ���� ����� ����� ��������� ���������  ���, 
����� ������������ �������� �  ���������  ���������  ����  ��� 
����� ������. 
  ���� �� ����� INPUT.TXT. � ������ ������ �������� �������� N 
(1<=N<=10^5). � ������ �� ��������� N-1  �����  �������� ����� 
������ ���� ������� �����������, ������������ ����������.
  ����� � ���� OUTPUT.TXT. � ������ ������ ��������� ����������
������� ����������� M. �� ������ ������ �������� ����� ������ �
������� ����������� ������ ������� �����������.
  ����� ����� �� ������ ��������� 2 ���.
  ������
  ����
4
1 2
4 3
2 3
  �����
2
2 3
  ��������. ���������� ��������� ������, �������������� �������
���������� ������� ������������ ������ �� ������� ������ (14).
��������: ������� ������ ��-21
IDE: VS Code
C++ 17
*/
#include <iostream>
#include <vector>
#include <fstream>


int findMax(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        // ���� ������ ����, ������ ����� �������� �� ���������, ��������, -1.
        return -1;
    }

    int max = numbers[0];  // �����������, ��� ������ ������� - ������������.

    for (int i = 1; i < numbers.size(); ++i) {
        if (numbers[i] > max) {
            max = numbers[i];  // ���� ������� ������� �������, ��������� max.
        }
    }

    return max;
}

int findMin(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        // ���� ������ ����, ������ ����� �������� �� ���������, ��������, -1.
        return -1;
    }

    int min = findMax(numbers);  // �����������, ��� ������ ������� - ������������.

    for (int i = 1; i < numbers.size(); ++i) {
        if (numbers[i] < min && numbers[i] != 0) {
            min = numbers[i];  // ���� ������� ������� �������, ��������� max.
        }
    }

    return min;
}

int dfs(int node, const std::vector<std::vector<int>>& adj_list, std::vector<bool>& visited, int distance, int& farthest_node) {
    visited[node] = true;

    std::vector<int> distances;

    for (int neighbor : adj_list[node]) {
        if (!visited[neighbor]) {
            int new_distance = distance + 1;
            int new_farthest_node = farthest_node;

            distances.push_back(dfs(neighbor, adj_list, visited, new_distance, new_farthest_node));
        }
    }

    if (distances.size() == 0) {
        return distance;
    }

    return findMax(distances);;
}

std::vector<int> find_desired_computers(int N, const std::vector<std::pair<int, int>>& connections) {
    std::vector<std::vector<int>> adj_list(N + 1);

    for (const auto& connection : connections) {
        int a = connection.first;
        int b = connection.second;
        adj_list[a].push_back(b);
        adj_list[b].push_back(a);
    }

    std::vector<int> desired_computers;

    std::vector<bool> visited(N + 1, false);
    std::vector<int> distances(N + 1, 0);
    
    for (int i = 1; i <= N; i++) {
        int distance = 0;
        int farthest_node = i;
        std::vector<bool> visited(N + 1, false);
        distance = dfs(farthest_node, adj_list, visited, distance, farthest_node);
        distances[i] = distance;
    }

    int min_distance = findMin(distances);

    for (int i = 1; i <= N; i++) {
       if (distances[i] == min_distance) {
            desired_computers.push_back(i);
       }
    }

    return desired_computers;
}

int main() {
    std::ifstream input("INPUT.TXT");
    std::ofstream output("OUTPUT.TXT");

    int N;
    input >> N;

    std::vector<std::pair<int, int>> connections(N - 1);
    for (int i = 0; i < N - 1; i++) {
        input >> connections[i].first >> connections[i].second;
    }

    std::vector<int> desired_computers = find_desired_computers(N, connections);

    output << desired_computers.size() << '\n';
    for (int computer : desired_computers) {
        output << computer << ' ';
    }
    output << '\n';

    return 0;
}