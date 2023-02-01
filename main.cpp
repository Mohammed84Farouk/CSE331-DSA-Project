#include <bits/stdc++.h>

using namespace std;
class Xml {
private:
    string xml;
//f1
public:
    Xml(string s){
        xml = s;
    }
    string minifying(string s) {
        int stringSize = s.size();
        string str1;
        //handle the first char
        if (s[0] != ' ')
            str1 += s[0];
        //handle the rest of chars
        for (int i = 1; i < stringSize - 1; i++) {
            if (s[i] == '\n') {
                continue;
            } else if (s[i] == ' ') {
                if ((s[i - 1] == ' ') || (s[i - 1] == '>' && s[i + 1] == '<'))
                    continue;
                else
                    str1 += s[i];
            } else {
                str1 += s[i];
            }
        }
        //handle the last char
        if (s[stringSize - 1] != ' ')
            str1 += s[stringSize - 1];
        return str1;
    }

//f2
//returns vector of closing and opening tags
    vector<string> tags(string minifiedString) {
        string tagName;
        vector<string> tagsNames;
        bool openTheTag = false;
        int stringSize = minifiedString.size();
        for (int i = 0; i < stringSize; i++) {
            if (minifiedString[i] == '<') {
                openTheTag = true;
                continue;
            }
            if (openTheTag) {
                if (minifiedString[i] == '>') {
                    openTheTag = false;
                    tagsNames.push_back(tagName);
                    tagName = "";
                    continue;
                } else {
                    tagName += minifiedString[i];
                }
            }
        }
        return tagsNames;
    }

//f3
//gets the data of each tag
    vector<string> tagsData(string minifiedString) {
        vector<string> DataOfTags;
        int minifiedStringSize = minifiedString.size();
        string temp;
        for (int i = 0; i < minifiedStringSize; i++) {
            if (minifiedString[i] != '/') {
                temp += minifiedString[i];
            } else {
                while (minifiedString[i] != '>') {
                    i++;
                }
                if (i + 1 < minifiedStringSize)
                    temp += minifiedString[i + 1];
            }
        }
        int tempSize = temp.size();
        for (int i = 0; i < tempSize; i++) {
            string data;
            if ((temp[i] == '>') && (i + 1 < tempSize)) {
                if (temp[i + 1] == '<') {
                    DataOfTags.push_back("null");
                } else {
                    for (int j = i + 1; temp[j] != '<'; j++) {
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
    vector<bool> arrayTag(int tagsNamesSize, vector<string> tagsNames) {
        vector<bool> isArrayTag;
        int j = 0;
        for (int i = 0; i < tagsNamesSize; i++) {
            if (tagsNames[i].at(0) != '/') {
                if (tagsNames[i].at(tagsNames[i].length() - 1) == 's') {
                    isArrayTag.push_back(true);
                } else {
                    isArrayTag.push_back(false);
                }
            }
        }
        return isArrayTag;
    }

//f5
//insert tabs
    string insertTabs(int nTabs) {
        string tabs = "";
        while (nTabs--) {
            tabs += "    ";
        }
        return tabs;
    }

//f6
//xml to json function
    string xmlToJSON() {
        //preparing xml to be converted
        string minifiedString = minifying(xml);
        vector<string> tagsNames = tags(minifiedString);
        vector<string> myTagsData = tagsData(minifiedString);
        int tagsNamesSize = tagsNames.size();
        vector<bool> isArrayTag = arrayTag(tagsNamesSize, tagsNames);
        vector<string> keyOfArrayElements;
        //starting working on json
        stack<string> stack1;
        string JSON;
        string bracket = "{";
        string tagName;
        string key;
        JSON = "{\n";
        stack1.push(bracket);
        int tabs = 0;
        int dataCounter = 0;
        for (int i = 0; i < tagsNamesSize; i++, dataCounter++) {//looping on tags names
            tagName = tagsNames[i];
            //tabs++;
            //check if my tag is array or not, this code was a separate function but it introduces some bugs
            //if opening tag
            if (tagName[0] != '/') {
                stack1.push(tagName);
                tabs++;
                key = "\"" + tagName + "\"";
                //if my tag is array
                if (isArrayTag[dataCounter]) {
                    keyOfArrayElements.push_back(tagsNames[i + 1]);
                    if (myTagsData[dataCounter] == "null") {
                        if (JSON.at(JSON.length() - 2) == ']') {
                            JSON.pop_back();
                            JSON += ",\n";
                        }
                        JSON += insertTabs(tabs);
                        JSON += key + ": [\n";
                        bracket = "[";
                        stack1.push(bracket);
                    }
                }
                    // not array
                else {
                    bool writeTheKey = true;
                    for (int j = 0; j < keyOfArrayElements.size(); j++) {
                        if (tagName == keyOfArrayElements[j]) {
                            writeTheKey = false;
                            break;
                        }
                    }
                    if (myTagsData[dataCounter] == "null") {
                        JSON += insertTabs(tabs);
                        if (writeTheKey) {
                            JSON += key + ": {\n";
                        } else {
                            JSON += "{\n";
                        }
                        bracket = "{";
                        stack1.push(bracket);
                        //tabs++;
                    } else {
                        JSON += insertTabs(tabs);
                        if (writeTheKey) {
                            JSON += key + ": ";
                        }
                        JSON += "\"" + myTagsData[dataCounter] + "\",\n";
                    }
                }
            }
                //if closing tag
            else {
                while (stack1.top() == "{" || stack1.top() == "[") {
                    if (stack1.top() == "{") {
                        if (JSON.at(JSON.length() - 2) == ',') {
                            JSON.pop_back();
                            JSON.pop_back();
                            JSON += '\n';
                        }
                        JSON += insertTabs(tabs);
                        JSON += "},\n";
                        stack1.pop();
                    } else if (stack1.top() == "[") {
                        if (JSON.at(JSON.length() - 2) == ',') {
                            JSON.pop_back();
                            JSON.pop_back();
                            JSON += '\n';
                        }
                        JSON += insertTabs(tabs);
                        JSON += "]\n";
                        keyOfArrayElements.pop_back();
                        stack1.pop();
                    }
                }
                stack1.pop();
                tabs--;
                dataCounter--;
            }
        }
        tabs--;
        JSON += "}";
        return JSON;
    }
};
int main() {
    string s = "<users><user><id>1</id><name>Ahmed Ali</name><posts><post><body> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. </body><topics><topic> economy </topic><topic> finance </topic></topics></post><post><body> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. </body><topics><topic> solar_energy </topic></topics></post></posts><followers><follower><id>2</id></follower><follower><id>3</id></follower></followers></user><user><id>2</id><name>Yasser Ahmed</name><posts><post><body> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. </body><topics><topic> education </topic></topics></post></posts><followers><follower><id>1</id></follower></followers></user><user><id>3</id><name>Mohamed Sherif</name><posts><post><body> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. </body><topics><topic> sports </topic></topics></post></posts><followers><follower><id>1</id></follower></followers></user></users>";
    Xml xml =  Xml(s);
    string Json = xml.xmlToJSON();
    cout<<Json;
}



