#include <bits/stdc++.h>
#include <QString>

using namespace std;
//f1
string minifying (string s){
    int stringSize = s.size();
    string str1;
    //handle the first char
    if(s[0]!=' ')
        str1+=s[0];
    //handle the rest of chars
    for(int i = 1; i < stringSize-1; i++){
        if(s[i] == '\n'){
            continue;
        }
        else if (s[i] == ' '){
            if((s[i-1]== ' ') ||(s[i-1] == '>' && s[i+1] == '<'))
                continue;
            else
                str1+=s[i];
        }
        else{
            str1+=s[i];
        }
    }
    //handle the last char
    if(s[stringSize-1]!=' ')
        str1+=s[stringSize-1];
    return str1;
}
//f2
//returns vector of closing and opening tags
vector <string> tags(string minifiedString){
    string tagName;
    vector <string> tagsNames;
    bool openTheTag = false;
    int stringSize = minifiedString.size();
    for (int i = 0; i < stringSize ; i++) {
        if(minifiedString[i]=='<'){
            openTheTag = true;
            continue;
        }
        if (openTheTag){
            if(minifiedString[i]=='>'){
                openTheTag = false;
                tagsNames.push_back(tagName);
                tagName = "";
                continue;
            }
            else{
                tagName += minifiedString[i];
            }
        }
    }
    return tagsNames;
}

//f3
//gets the data of each tag
vector<string> tagsData(string minifiedString){
    vector<string> DataOfTags;
    int minifiedStringSize = minifiedString.size();
    string temp;
    for(int i = 0; i < minifiedStringSize; i++){
        if(minifiedString[i] != '/'){
            temp += minifiedString[i];
        }
        else{
            while(minifiedString[i] != '>'){
                i++;
            }
            if(i+1 < minifiedStringSize)
                temp += minifiedString[i+1];
        }
    }
    int tempSize = temp.size();
    for (int i = 0; i < tempSize; i++) {
        string data;
        if((temp[i]=='>') && (i+1 < tempSize)){
            if(temp[i+1] == '<'){
                DataOfTags.push_back("null");
            } else{
                for (int j = i+1; temp[j] != '<'; j++) {
                    data += temp[j];
                }
                DataOfTags.push_back(data);
            }
        }
    }
    return DataOfTags;
}
//f4
//check if my tag is array or not, this code was a separate function but it introduces some bugs
/*bool arrayTag(int tagsNamesSize,vector<string>tagsNames){
    bool isArrayTag[tagsNamesSize-1];
    for (int i = 0; i < tagsNamesSize; i++) {
        for (int j = i+1; j < tagsNamesSize-1; j++) {
            if(tagsNames[j]=="/"+tagsNames[i]){
                if(tagsNames[j+1]==tagsNames[i]){
                    isArrayTag[i]= true;
                    isArrayTag[j+1]= true;
                    break;
                }
            }
        }
    }
    return isArrayTag;
}*/
bool arrayTag(int tagsNamesSize,vector<string>tagsNames,int tagNameIndex){
    bool isArray = false;
    for (int j = tagNameIndex+1; j < tagsNamesSize-1; j++) {
        if(tagsNames[j]=="/"+tagsNames[tagNameIndex]){
            if(tagsNames[j+1]==tagsNames[tagNameIndex]){
                isArray = true;
                break;
            }
        }
    }
    return isArray;
}



//f5
//insert tabs
string insertTabs(int nTabs){
    string tabs="";
    while (nTabs--){
        tabs+="    ";
    }
    return tabs;
}

//f6
//xml to json function
string xmlToJSON(string xml){
    //preparing xml to be converted
    string minifiedString = minifying(xml);
    vector<string> tagsNames = tags(minifiedString);
    vector<string> myTagsData=tagsData(minifiedString);
    int tagsNamesSize = tagsNames.size();
    //starting working on json
    stack <string> stack1;
    string JSON;
    string bracket = "{";
    string tagName;
    string key;
    vector<string> repeatedArrayTags;
    int repeatedArrayTagsSize = repeatedArrayTags.size();
    JSON="{\n";
    stack1.push(bracket);
    int tabs = 0;
    int dataCounter = 0;
    for (int i = 0; i < tagsNamesSize; i++,dataCounter++) {//looping on tags names
        bool repeatedTags = false;
        tagName = tagsNames[i];

        //tabs++;
        //check if my tag is array or not, this code was a separate function but it introduces some bugs
        bool isArray = arrayTag(tagsNamesSize,tagsNames,i);
        //if opening tag
        if(tagName[0] !='/'){
            stack1.push(tagName);
            tabs++;
            key = "\""+tagName+"\"";
            //if my tag is array
            if(isArray){
                repeatedArrayTags.push_back(tagName);
                for (int j = 0; j < repeatedArrayTagsSize-1; j++) {
                    // checks if this tag is not the first element in the array
                    if(tagName == repeatedArrayTags[j]){
                        repeatedTags = true;
                        break;
                    }
                }
                if(repeatedTags){
                    repeatedArrayTags.pop_back();
                }
                if (repeatedTags){
                    if(myTagsData[dataCounter]=="null"){
                        JSON += insertTabs(tabs);
                        JSON +="{\n";
                        bracket = "{";
                        stack1.push(bracket);
                        //tabs++;
                    }
                    else{ // repeated but data = value
                        JSON += insertTabs(tabs);
                        JSON +="\""+myTagsData[dataCounter]+"\",\n";
                        //next two lines are added
                        bracket = "{";
                        stack1.push(bracket);
                    }

                }
                else{
                    // not repeated but data = null
                    if(myTagsData[dataCounter]=="null"){
                        JSON += insertTabs(tabs);
                        JSON += key + ": [\n";
                        bracket = "[";
                        stack1.push(bracket);
                        JSON += insertTabs(++tabs);
                        JSON += "{\n";
                        //next two lines are added
                        bracket = "{";
                        stack1.push(bracket);
                        //tabs++;
                    }
                        // not repeated but data = value
                    else{
                        JSON += insertTabs(tabs);
                        JSON += key + ": [\n";
                        bracket = "[";
                        stack1.push(bracket);
                        //tabs++;
                        JSON += insertTabs(tabs);
                        JSON += "\"" + myTagsData[dataCounter] + "\",\n";
                    }
                }
            }
                // not array
            else{
                if(myTagsData[dataCounter]=="null"){
                    JSON += insertTabs(tabs);
                    JSON += key + ": {\n";
                    bracket = "{";
                    stack1.push(bracket);
                    //tabs++;
                }
                else{
                    JSON += insertTabs(tabs);
                    JSON += key + ": ";
                    JSON += "\"" + myTagsData[dataCounter] + "\",\n";
                }
            }
        }
            //if closing tag
        else{
            while (stack1.top()=="{" || stack1.top()=="["){
                if(stack1.top()=="{"){
                    JSON += insertTabs(tabs);
                    JSON += "}\n";
                    stack1.pop();
                }
                else if(stack1.top()=="["){
                    JSON += insertTabs(tabs);
                    JSON += "]\n";
                    stack1.pop();
                }
            }
            stack1.pop();
            tabs--;
            dataCounter--;
        }
    }
    return JSON;
}
