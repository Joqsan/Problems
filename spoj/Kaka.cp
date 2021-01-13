#include <vector>
#include <utility>
#include <deque>
#include <cstdio>

using namespace std;

typedef pair<int, int> cell;

vector<int> dr = {1, 0};
vector<int> dc = {0, 1};

bool IsInside(int r, int R, int c, int C)
{
    return r >= 0 && r < R && c >= 0 && c < C;
}

int N, K;

void TraverseBoard(vector<vector<bool>> &visited, vector<vector<int>> board, int &maxSum)
{
    deque<cell> Q;
    
    Q.push_back({0, 0});
    while (!Q.empty())
    {
        int r = Q.front().first;
        int c = Q.front().second;
        Q.pop_front();
        
        if (r == N-1 && c == N-1)
            break;
        
        int rBest, cBest;
        rBest = cBest = -1;
        for (int j = 0; j != 2; ++j)
        {
            if (IsInside(r + dr[j], N, c + dc[j], N) && visited[r + dr[j]][c + dc[j]] == false)
            {
                if (rBest == -1)
                {
                    rBest = r + dr[j];
                    cBest = c + dc[j];
                }
                else
                    if (board[rBest][cBest] < board[r + dr[j]][c + dc[j]])
                    {
                        rBest = r + dr[j];
                        cBest = c + dc[j];
                    }
            }
        }
        
        if (rBest != -1)
        {
            visited[rBest][cBest] = true;
            maxSum += board[rBest][cBest];
            Q.push_back({rBest, cBest});
        }
    }
    
}

int main(void)
{
    FILE *fp = fopen("Kaka.txt", "r");
    fscanf(fp, "%d %d", &N, &K);
    
    
    vector<vector<int>> board(N, vector<int>(N));
    vector<vector<bool>> visited(N, vector<bool>(N, false));
    int maxSum = 0;
    
    int i = 0;
    while (i != N)
    {
        for (int j = 0; j != N; ++j)
        {
            int u;
            fscanf(fp, "%d", &u);
            board[i][j] = u;
        }
        ++i;
    }
    
    visited[0][0] = true;
    maxSum += board[0][0];
    
    for (int i = 0; i != K; ++i)
        TraverseBoard(visited, board, maxSum);
    
    printf("%d\n", maxSum);
    
    return 0;
    
}
