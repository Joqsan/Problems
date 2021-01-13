#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <iostream>

using namespace std;


class PlayingCubes
{
private:
    
public:
    
    bool FindPath(int word_source, const vector<string> &cubes, const vector<string> &words);
    vector<int> composeWords(vector<string> cubes, vector<string> words);
};



bool PlayingCubes::FindPath(int word_source, const vector<string> &cubes, const vector<string> &words)
{
    int current_letter = -1;
    int current_cube = -1;
    deque<int> Q;
    vector<int> from_letter(words[word_source].size(), -1);
    vector<int> letterMatchingCube(words[word_source].size(), -1);
    // which_cubes, which_letter.
    vector<int> cubeMatchingLetter(cubes.size(), -1);
    
    for (int i = 0; i != words[word_source].size(); ++i)
    {
        deque<int> for_shifting;
        from_letter[i] = -1;
        Q.push_back(i);
        bool path_found = false;
        
        while (!path_found && !Q.empty())
        {
            current_letter = Q.front();
            Q.pop_front();
            for (int j = 0; j != cubes.size(); ++j)
            {
                current_cube = j;
                int matched_letter = cubeMatchingLetter[current_cube];
                bool is_in_cube = cubes[current_cube].find(words[word_source][current_letter]) != string::npos;
                
                if (is_in_cube)
                {
                    if (matched_letter == -1)
                    {
                        path_found = true;
                        break;
                    }
                    
                    // add for shifting only if the two letters are different.
                    if (words[word_source][current_letter] != words[word_source][matched_letter])
                        for_shifting.push_back(matched_letter);
                }
            }
            
            // we loop for shifting only if the current letter wasn't found in any of
            // the remaining unmatched cubes.
            if (!path_found)
                while (!for_shifting.empty())
                {
                    if (from_letter[for_shifting.front()] != -1)
                    {
                        from_letter[for_shifting.front()] = current_cube;
                        Q.push_back(for_shifting.front());
                    }
                    for_shifting.pop_front();
                }
        }
        
        if (!path_found)
            break;
        
        while (current_letter != -1)
        {
            int temp_cube = letterMatchingCube[current_letter];
            letterMatchingCube[current_letter] = current_cube;
            cubeMatchingLetter[current_cube] = current_letter;
            
            current_letter = from_letter[current_letter];
            current_cube = temp_cube;
        }
    }
    
    if (find(letterMatchingCube.begin(), letterMatchingCube.end(), -1) != letterMatchingCube.end())
        return false;
    else
        return true;
}

vector<int> PlayingCubes::composeWords(vector<string> cubes, vector<string> words)
{
    
    vector<int> idxWords;
    
    for (int i = 0; i != words.size(); ++i)
    {
        if (FindPath(i, cubes, words))
            idxWords.push_back(i);
    }
    
    return idxWords;
}

int main(void)
{
    PlayingCubes game;
    vector<int> wordsFormed = game.composeWords({"ABCDEF", "DEFGHI", "OPQRST", "ZZZZZZ", "ZZZZZZ"}, {"CAT", "DOG", "PIZZA"});
    
    for (auto e : wordsFormed)
        cout << e << " ";
    cout << endl;
    
    return 0;
}
