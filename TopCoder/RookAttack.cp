#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class RookAttack
{
private:
    
    int rows;
    int cols;
    
    vector<string> cutouts;
    vector<int> rowMatchingCol;
    vector<int> colMatchingRow;
    vector<int> from_row;
public:
    
    bool FindPath(int row_source);
    int howMany(int rows, int cols, vector<string> cutouts);
    
    RookAttack(int r, int c, vector<string> cutouts) :
    rows(r), cols(c), rowMatchingCol(r, -1), colMatchingRow(c, -1),
    from_row(r, -1), cutouts(cutouts)
    {}
};

bool RookAttack::FindPath(int row_source)
{
    int current_row = -1;
    int current_col = -1;
    deque<int> for_shifting;
    deque<int> Q;
    
    from_row[row_source] = -1;
    Q.push_back(row_source);
    
    bool path_found = false;
    
    while (!path_found && !Q.empty())
    {
        current_row = Q.front();
        Q.pop_front();
        
        for (int i = 0; i != cols; ++i)
        {
            current_col = i;
            int matched_row = colMatchingRow[current_col];
            
            bool allowed_square = find(cutouts.begin(), cutouts.end(),
                                       to_string(current_row) + " " + to_string(current_col)) == cutouts.end();
            
            if (allowed_square)
            {
                if (matched_row == -1)
                {
                    path_found = true;
                    break;
                }
                
                if (current_row != matched_row)
                    for_shifting.push_back(matched_row);
            }
        }
        
        if (!path_found)
            while (!for_shifting.empty())
            {
                if (from_row[for_shifting.front()] == -1)
                {
                    from_row[for_shifting.front()] = current_row;
                    Q.push_back(for_shifting.front());
                }
                
                for_shifting.pop_front();
            }
    }
    
    if (!path_found)
        return false;
    
    while (current_row != -1)
    {
        int temp_col = rowMatchingCol[current_row]; // it makes available the square it was for the new rook
        rowMatchingCol[current_row] = current_col;
        colMatchingRow[current_col] = current_row;
        
        current_row = from_row[current_row];
        current_col = temp_col;
    }
    
    return true;
}

int RookAttack::howMany(int rows, int cols, vector<string> cutouts)
{
    int n_placed_rooks = 0;
    for (int i = 0; i != rows; ++i)
        n_placed_rooks += FindPath(i);
    
    return n_placed_rooks;
}

int main(void)
{
    RookAttack game(200, 200, {});
    
    cout << game.howMany(200, 200,{}) << endl;
    
    return 0;
}
