#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <chrono>
#include <iomanip>

struct Node
{
    std::vector<int> state;
    Node(const std::vector<int>& board) : state(board) {}
};

int findZeroPosition(const Node& node)
{
    for (int i = 0; i < node.state.size(); i++)
    {
        if (node.state[i] == 0)
        {
            return i;
        }
    }
    return -1;
}

bool isSolvable(const Node& node, int size)
{
    int inversions = 0;
    for (int i = 0; i < size * size - 1; i++)
    {
        for (int j = i + 1; j < size * size; j++)
        {
            if (node.state[j] && node.state[i] && node.state[i] > node.state[j])
            {
                inversions++;
            }
        }
    }

    if (size % 2 != 0)
    {
        return inversions % 2 == 0;
    }
    
    int zeroRow = findZeroPosition(node) / size;
        
    return (inversions + zeroRow) % 2 == 1;
    
}

int manhattanDistance(const Node& node, int realGoalZeroIndex)
{
    int dist = 0;
    int size = node.state.size();
    for (int i = 0; i < size; i++)
    {
        int value = node.state[i];
        if (value != 0)
        {
            int n = sqrt(size);
            // Target coordinates of the tile
            int targetX = 0;
            int targetY = 0;

            if (value < abs(realGoalZeroIndex))           
            {                
                targetX = (value - 1) % n;
                targetY = (value - 1) / n;
            }
            else
            {
                targetX = value % n;
                targetY = value / n;
            }

            int currX = i % n;
            int currY = i / n;

            dist += abs(currX - targetX) + abs(currY - targetY);
        }
    }
    return dist;
}

bool isValidMove(int x, int y, int size)
{
    return x >= 0 && x < size && y >= 0 && y < size;
}

int solvePuzzle(const Node& initialNode, int currCost, int bound, std::vector<std::string>& path, int realGoalZeroIndex)
{
    int heuristic = manhattanDistance(initialNode, realGoalZeroIndex);
    int f = currCost + heuristic;

    if (f > bound)
    {
        return f;
    }

    if (heuristic == 0)
    {
        return 0;
    }

    int min = INT_MAX;
    int zero = findZeroPosition(initialNode);
    int size = sqrt(initialNode.state.size());

    int currX = zero % size;
    int currY = zero / size;

    int dx[] = { 1, -1, 0, 0 };
    int dy[] = { 0, 0, 1, -1 };

    for (int i = 0; i < 4; i++)
    {
        int targetX = currX + dx[i];
        int targetY = currY + dy[i];
        int newIndex = targetX + targetY * size;
        
        Node child = initialNode;

        std::string oppositeDir = (i == 0) ? "right" : (i == 1) ? "left" : (i == 2) ? "down" : "up";

        if (!path.empty() && path.back() == oppositeDir)
        {
            continue;
        }

        if (isValidMove(targetX, targetY, size))
        {   
            std::swap(child.state[zero], child.state[newIndex]);

            path.push_back((i == 0) ? "left" : (i == 1) ? "right" : (i == 2) ? "up" : "down");

            int t = solvePuzzle(child, currCost + 1, bound, path, realGoalZeroIndex);

            if (t == 0)
            {
                return 0;
            }
          
            if (t < min)
            {
                min = t;
            }
          
            path.pop_back();
        }
    }
    return min;
}

int main()
{
    int n;
    std::cin >> n;

    std::vector<int> board(n + 1);
    std::vector<std::string> path;

    int goalZeroPosition;
    std::cin >> goalZeroPosition;

    int realGoalZeroPos;
    if (goalZeroPosition < 0) 
    {
        realGoalZeroPos = goalZeroPosition + n + 2;
    } 
    else if (goalZeroPosition == 0) 
    {
        realGoalZeroPos = goalZeroPosition;
    }  
    else 
    {
        realGoalZeroPos = goalZeroPosition + 1;
    }

    for (int i = 0; i < n + 1; i++)
    {
        std::cin >> board[i];
    }

    Node start(board);

    if (!isSolvable(start, sqrt(n + 1)))
    {
        std::cout << -1 << std::endl;
        return -1;
    }

    int bound = manhattanDistance(start, realGoalZeroPos);
    bool solved = false;

    auto start_time = std::chrono::high_resolution_clock::now();

    while (!solved) 
    {
        int t = solvePuzzle(start, 0, bound, path, realGoalZeroPos);

        if (t == INT_MAX) 
        {
            std::cout << "No solution found." << std::endl;
            break;
        }

        if (t == 0) 
        {
            solved = true;

            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end_time - start_time;

            //Print the time needed to find the path
            //std::cout << "Time to find solution: " << std::fixed << std::setprecision(6) << elapsed.count() << " seconds" << std::endl;

            std::cout << path.size() << std::endl;

            for (size_t i = 0; i < path.size(); ++i) 
            {
                std::cout << path[i] << std::endl;
            }
            break;
        }
        bound = t;
    }
}
