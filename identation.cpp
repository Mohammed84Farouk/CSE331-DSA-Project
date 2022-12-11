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
    ofstream out("output.xml");
    while (getline(in,line))
    {
        string openTag;
        string closeTag;
        string text;
        int depth = 0;
        bool insidetag = false;
        bool atribute = false;
        bool tagPushed = false;
        bool closingtag = false;
        for (int i = 0, j = 0, z = 0; i < line.length(); i++)
        {
            if (line[i] == ' ' && openTag.size() == 0){
            }
            else{
                if(line[i] == '>'){
                    insidetag = false;
                    atribute = false;
                }
                else if(line[i] == '<' && line[i+1] == '/') closingtag = true;
                else if(line[i] == '<' && line[i+1] != '/' && line[i+1] != '?' ) insidetag = true;

                if(insidetag && line[i] == ' '){
                    atribute = true;
                }

                if(atribute) continue;
                if(!tagPushed){
                    openTag += line[i];
                    j++;
                }
                if(closingtag) {
                    closeTag += line[i];
                    z++;
                }
                if(!insidetag && !closingtag) text += line[i];
            }
            if(i < 3) continue;


            if (openTag[0] == '<' && openTag[1] != '/' && openTag[j-1] == '>' && !tagPushed){
                cout<<openTag<<endl;
                stack.push(openTag.substr(1,j-2));
                tagPushed = true;
                for (int w = 0; i < depth * 4; w++)
                {
                    out.put(' ');
                }
                
                out << openTag ;
                depth++;
            }
            if (closeTag[0] == '<' && closeTag[1] == '/' && closeTag[z-1] == '>'){
                cout << closeTag << endl;
                /*while(closeTag.substr(2, z-3) != stack.top() && !stack.empty()){

                    stack.pop();
                } */
                depth--;
            }
        }
    }
}