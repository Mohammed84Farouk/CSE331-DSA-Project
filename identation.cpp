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
    int depth = 0;
    while (getline(in,line))
    {
        string openTag;
        string closeTag;
        string text;
        bool insidetag = false;
        bool atribute = false;
        bool tagPushed = true;
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
                else if(line[i] == '<' && line[i+1] != '/' && line[i+1] != '?' ) {
                    insidetag = true;
                    tagPushed = false;
                }

                if(insidetag && line[i] == ' '){
                    atribute = true;
                }

                if(atribute) continue;
                
                if(closingtag) {
                    closeTag += line[i];
                    z++;
                }
                else if(!tagPushed){
                    openTag += line[i];
                    j++;
                }
                else if(!insidetag && !closingtag) text += line[i];
            }
            if(i < 3) continue;


            if (openTag[0] == '<' && openTag[1] != '/' && openTag[j-1] == '>' && !tagPushed){
                cout<<openTag<<endl;
                stack.push(openTag.substr(1,j-2));
                tagPushed = true;
                out<<endl;
                for (int w = 0; i < depth * 4; w++)
                {
                    out.put(' ');
                }
                
                out << openTag ;
                depth++;
            }
            if (closeTag[0] == '<' && closeTag[1] == '/' && closeTag[z-1] == '>'){
                out << text ;
                if(closeTag.substr(2, z-3) != stack.top()){
                    cout << '<' << '/' << stack.top() << '>' <<endl;
                    out << '<' << '/' << stack.top() << '>' << endl;
                    out << closeTag;
                } 
                else{
                    cout << closeTag << endl;
                    out << closeTag;
                    stack.pop();
                }
                depth--;
            }else if (i == line.length() -1) out << text;
        }
    }
}