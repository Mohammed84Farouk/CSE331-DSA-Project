#include <iostream>
#include <string>
#include <fstream>
#include <stack>

using namespace std;

void consistent(string input, string output){
    stack<string> stack;
    ifstream in(input);
    ofstream out(output);
    string line;
    while (getline(in,line))
    {
        string openTag;
        string closeTag;
        string text;
        string attribute;
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

                if(atribute) attribute+= line[i];
                
                else if(closingtag) {
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
                if(openTag[1] != '?') stack.push(openTag.substr(1,j-2));
                tagPushed = true;
                out<<endl;
                out << openTag.substr(0,j-1) << attribute << '>';
            }
            if (closeTag[0] == '<' && closeTag[1] == '/' && closeTag[z-1] == '>'){
                out << text ;
                if(stack.empty() == false){
                    out << '<' << closeTag.substr(2, z-3) << '>';
                    out << closeTag;
                    out <<endl;
                }
                else if(closeTag.substr(2, z-3) != stack.top()){
                    out << '<' << '/' << stack.top() << '>';
                    out <<endl;
                    stack.pop();
                    out << closeTag;
                    stack.pop();
                    continue;
                } 
                else{
                    if(text.size() < 1){
                        out << endl;
                    } 
                    out << closeTag;
                    stack.pop();
                }
            }else if (i == line.length() -1) out << text;
        }
    }
}
void pretify(string input, string output){
    int depth = 0;
    int depthfornextline = 0;
    ifstream in(input);
    ofstream out(output);
    string line;
    while (getline(in,line))
    {
        string line2;
        bool isTag = false;
        for (int i = 0, j = 0, z = 0, q = 0; i < line.length(); i++)
        {
            if (line[i] == ' ' && line2.size() == 0){
                continue;
            }
            else {
                line2 += line[i];
                j++;
            }
            if(line2[j-1] == '>'){
                z = j-1;
                isTag = true;
            }
            else if(line2[j-1] == '<') {
                q = j-1;
            }
            if (i == line.length()-1)
            {
                /* code */
            }
            
            if(line2[q] == '<' && line2[q+1] != '/' && line2[z] == '>' && isTag == true) {
                depth++;
                isTag = false;
            }
            else if(line2[q] == '<' && line2[q+1] == '/' && line2[z] == '>' && isTag == true) { 
                depth--;
                depthfornextline = depth;
                isTag = false;
            }
        } 
        for (int w = 0; w < depthfornextline; w++)
        {
            out<<'\t';
        }
        out<<line2<<endl;
        depthfornextline = depth;
    }
}
