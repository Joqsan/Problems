#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Graduation
{
private:
    string classesFinished;
    string classesLeft;
    string classesTaken;
public:
    Graduation(string classTaken);
    void FindPath(const string &s, int &n_classesLeft);
    string moreClasses(string classesTaken, vector<string> requirements);
};

Graduation::Graduation(string classesTaken)
{
    this->classesTaken = classesTaken;
}

// s is a requirement // classesTaken
void Graduation::FindPath(const string &s, int &n_classesLeft)
{
    for (auto it = s.begin(); it != s.end(); ++it)
    {
        char class_requested = *it;
        
        bool requested_taken = find(classesTaken.begin(), classesTaken.end(), class_requested) != classesTaken.end();
        bool class_not_visited = classesFinished.find(class_requested) == string::npos;
        
        if (requested_taken)
        {
            if (class_not_visited)
            {
                classesFinished += class_requested;
                --n_classesLeft;
            }
        }
        else
        {
            if (classesLeft.find(class_requested) == string::npos)
                classesLeft += class_requested;
        }
            
    }
}

string Graduation::moreClasses(string classesTaken, vector<string> requirements)
{
    int n_classesLeft = 0;
    bool impossibleGraduation = false;
    for (auto it = requirements.begin(); it != requirements.end(); ++it)
    {
        string s = *it;
        string::size_type sz;
        
        int n_request = stoi(s, &sz);
        s = s.substr(sz);
        
        if (n_request > s.size())
        {
            impossibleGraduation = true;
            break;
        }
        
        n_classesLeft += n_request;
        FindPath(s, n_classesLeft);
    }
    
    if (impossibleGraduation)
        return string("0");
    
    if (n_classesLeft == 0)
        return string("");
    
    classesLeft = classesLeft.substr(0, n_classesLeft);
    sort(classesLeft.begin(), classesLeft.end());
    
    return classesLeft;
}

int main(void)
{
    Graduation student("");
    
    cout << student.moreClasses("", {"5ABCDE","1BCDE,"}) << endl;
    
    return 0;
}
