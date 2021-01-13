#include <vector>
#include <deque>
#include <utility>
#include <algorithm>
#include <cmath>

using namespace std;

vector<int> dc = {1, 0, -1, 0};
vector<int> dr = {0, 1, 0, -1};

typedef pair<int, int> coord;

bool InInside(int N, int r, int c)
{
    return r >= 0 && r < N && c >= 0 && c < N;
}

class MTWalk
{
private:
    int N;
    vector<vector<bool>> inPathYet;
public:
    int currentBestDiff;
    
    MTWalk(int N);
    void DFS(const vector<vector<int>> Grid, coord s, int minHeight, int maxHeight);
};

MTWalk::MTWalk(int N) :
N(N), currentBestDiff(111), inPathYet(N, vector<bool>(N, false))
{
}


void MTWalk::DFS(const vector<vector<int>> Grid, coord s, int minHeight, int maxHeight)
{
    int ur = s.first;
    int uc = s.second;
    
    inPathYet[ur][uc] = true;
    
    if (ur == N - 1 && uc == N - 1)
        currentBestDiff = maxHeight - minHeight;
    
    for (int i = 0; i != 4 && currentBestDiff > maxHeight - minHeight; ++i)
    {
        int vr = ur + dr[i];
        int vc = uc + dc[i];
        
        if (InInside(N, vr, vc) && !inPathYet[vr][vc])
        {
            int height = Grid[vr][vc];
            
            if (minHeight <= height && height <= maxHeight)
                DFS(Grid, {vr, vc}, minHeight, maxHeight);
            else if (height <= minHeight && maxHeight - height < currentBestDiff)
                DFS(Grid, {vr, vc}, height, maxHeight);
            else if (height - minHeight < currentBestDiff)
                DFS(Grid, {vr, vc}, minHeight, height);
        }
        
    }
    
    inPathYet[ur][uc] = false;
}

int main(void)
{
    FILE *fp = fopen("mtwalk.txt", "r");
    
    int N;
    
    fscanf(fp, "%d", &N);
    MTWalk game(N);
    
    vector<vector<int>> Grid(N, vector<int>(N));
    for (int i = 0; i != N; ++i)
        for (int j = 0; j != N; ++j)
            fscanf(fp, "%d", &Grid[i][j]);
    
    int minHeight = min(Grid[0][0], Grid[N-1][N-1]);
    int maxHeight = max(Grid[0][0], Grid[N-1][N-1]);
    
    game.DFS(Grid, {0, 0}, minHeight, maxHeight);
    
    printf("%d\n", game.currentBestDiff);
    
    return 0;
}

