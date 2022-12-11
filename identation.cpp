#include <iostream>
#include <string>
#include <fstream>
#include <stack>

using namespace std;

int main()
{
    string line;
    ifstream in("test.xml");
    stack<string> stack;

    bool begin_tag = false;
    while (getline(in,line))
    {
        string tmp;

        for (int i = 0, j = 0, k = 0; i < line.length(); i++)
        {
            
            if (line[i] == ' ' && tmp.size() == 0){
            }
            else{
                j++;

                tmp += line[i];
            }


            if (tmp[k] == '<' && tmp[k+1] != '/' && tmp[j-1] == '>'){
                cout << tmp << endl;
                stack.push(tmp);
                continue;
            }
        }
    }
}