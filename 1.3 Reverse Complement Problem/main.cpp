#include <iostream>
#include <string>
using namespace std;
int main()
{
    string s1 = "", s2 = "";
    cin >> s1;
    size_t size = s1.size();
    for (size_t i = 0; i < size; i++)
    {
        switch(s1[i])
        {
            case 'A':{
                s1[i] = 'T';
                break;
            }
            case 'C':{
                s1[i] = 'G';
                break;
            }
            case 'G':{
                s1[i] = 'C';
                break;
            }
            case 'T':{
                s1[i] = 'A';
                break;
            }
        }
    }
    for (auto it = (s1.end() - 1); it != (s1.begin() - 1); --it)
        s2 += (*it);
    cout << s2;
    return 0;
}