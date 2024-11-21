#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <ctime>

void initializeBoard(std::vector<int>& queens, std::vector<int>& rowConflicts, std::vector<int>& diag1Conflicts, std::vector<int>& diag2Conflicts) 
{
    int N = queens.size();

    for (int col = 0; col < N; ++col) 
    {
        int row = std::rand() % N;
        queens[col] = row;

        rowConflicts[row] += 1;
        diag1Conflicts[col + row] += 1;
        diag2Conflicts[col - row + N - 1] += 1;
    }
}

int calculateConflicts(const std::vector<int>& rowConflicts, const std::vector<int>& diag1Conflicts, const std::vector<int>& diag2Conflicts, int row, int col, int N) 
{
    return rowConflicts[row] + diag1Conflicts[col + row] + diag2Conflicts[col - row + N - 1] - 3;
}

bool solveNQueens(int N, std::vector<int>& queens, std::vector<int>& rowConflicts, std::vector<int>& diag1Conflicts, std::vector<int>& diag2Conflicts, int maxSteps) 
{
    initializeBoard(queens, rowConflicts, diag1Conflicts, diag2Conflicts);

    for (int step = 0; step < maxSteps; ++step) 
    {
        int maxConflicts = 0;
        std::vector<int> conflictedColumns;

        // Find columns with the most conflicts
        for (int col = 0; col < N; ++col) 
        {
            int conflicts = calculateConflicts(rowConflicts, diag1Conflicts, diag2Conflicts, queens[col], col, N);
            if (conflicts > maxConflicts) 
            {
                maxConflicts = conflicts;
                conflictedColumns.clear();
                conflictedColumns.push_back(col);
            } 
            else if (conflicts == maxConflicts) 
            {
                conflictedColumns.push_back(col);
            }
        }

        if (maxConflicts == 0) 
        {
            return true;
        }

        // Choose a random conflicted column
        int conflictedColumn = conflictedColumns[std::rand() % conflictedColumns.size()];
        int minConflicts = N;
        std::vector<int> bestRows;

        // Find the row with the fewest conflicts for the chosen most-conflicted column
        for (int row = 0; row < N; ++row) 
        {
            int conflicts = calculateConflicts(rowConflicts, diag1Conflicts, diag2Conflicts, row, conflictedColumn, N);
            
            if (conflicts < minConflicts) 
            {
                minConflicts = conflicts;
                bestRows.clear();
                bestRows.push_back(row);
            } 
            else if (conflicts == minConflicts) 
            {
                bestRows.push_back(row);
            }
        }

        // Move the queen to the row with the fewest conflicts
        int newRow = bestRows[std::rand() % bestRows.size()];
        int oldRow = queens[conflictedColumn];

        // Update the conflicts for rows and diagonals
        queens[conflictedColumn] = newRow;
        rowConflicts[oldRow] -= 1;
        diag1Conflicts[conflictedColumn + oldRow] -= 1;
        diag2Conflicts[conflictedColumn - oldRow + N - 1] -= 1;

        rowConflicts[newRow] += 1;
        diag1Conflicts[conflictedColumn + newRow] += 1;
        diag2Conflicts[conflictedColumn - newRow + N - 1] += 1;
    }

    return false;
}

int main() 
{
    int N;
    std::cin >> N;

    std::srand(std::time(NULL));

    std::vector<int> queens(N, 0);
    std::vector<int> rowConflicts(N, 0);
    std::vector<int> diag1Conflicts(2 * N - 1, 0);
    std::vector<int> diag2Conflicts(2 * N - 1, 0);

    int maxSteps = 600;
    int maxAttempts = 5;
    bool solved = false;

    auto start_time = std::chrono::high_resolution_clock::now();
    

    for (int attempt = 0; attempt < maxAttempts; ++attempt) 
    {
        if (solved)
        {
            break;
        }

        std::fill(rowConflicts.begin(), rowConflicts.end(), 0);
        std::fill(diag1Conflicts.begin(), diag1Conflicts.end(), 0);
        std::fill(diag2Conflicts.begin(), diag2Conflicts.end(), 0);

        solved = solveNQueens(N, queens, rowConflicts, diag1Conflicts, diag2Conflicts, maxSteps);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    if (N > 100) 
    {
        std::cout << std::fixed << std::setprecision(3) << elapsed.count();
    } 
    else if (solved) 
    {
        // Print the array
        /*
        std::cout << "[";
        for (int i = 0; i < N; ++i) 
        {
            std::cout << queens[i];

            if (i < N - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
        */
        

        // Print the board
        for (int row = 0; row < N; row++)
        {
            for (int col = 0; col < N; col++)
            {
                if (queens[col] == row)
                {
                    std::cout << "* ";
                }
                else
                {
                    std::cout << "_ ";
                }
            }
            std::cout << std::endl;
        } 
    } 
    else 
    {
        std::cout << "-1" << std::endl;
    }

    return 0;
}
