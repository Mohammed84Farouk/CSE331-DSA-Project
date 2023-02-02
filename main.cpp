#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define FIO ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
class SNA {
public:
    struct post{
        string body;
        vector<string>topics;
    };
    struct user {
        int id;                                                 // they are given in an increasing order starting from 1
        string name;
        vector<post> posts;
        vector<int> followers;
        user(int _id, string _name) { id = _id, name = _name; }
    };
    vector<user>v;
    vector<vector<int>>adj;
    vector<string>antiID;
    unordered_map<string,int>ID;
    SNA(){}
    SNA(int n){
        adj.resize(n+1);
        antiID.resize(n+1);                                  // Maximum number of IDs I can get will be n starting from 1
    }
    string minifying(string s) {
        int stringSize = s.size();
        string str1;
        if (s[0] != ' ') str1 += s[0];
        for (int i = 1; i < stringSize - 1; i++) {
            if (s[i] == '\n') continue;
            else if (s[i] == ' ') {
                if ((s[i - 1] == ' ') || (s[i - 1] == '>' && s[i + 1] == '<')) continue;
                else str1 += s[i];
            } else str1 += s[i];
        }
        if (s[stringSize - 1] != ' ') str1 += s[stringSize - 1];
        return str1;
    }
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
    void takeInput(SNA&sna){
        int n;
        cin>>n;
        sna=SNA(n);
        for(int i=0;i<n;i++){
            int id;
            string s;
            cin>>id; cin.ignore();
            getline(cin, s);
            sna.antiID[id]=s;
            ID[s]=id;
            user u(id, s);
            int pp; cin>>pp; cin.ignore();
            while(pp--){
                post p;
                getline(cin, p.body);
                int t;
                cin>>t; cin.ignore();
                vector<string>tops;
                while(t--){
                    string topicX;
                    getline(cin, topicX);
                    tops.push_back(topicX);
                }
                u.posts.push_back(p);
            }
            int f; cin>>f;
            while(f--){
                int follower;
                cin>>follower;
                u.followers.push_back(follower);
                sna.adj[id].push_back(follower);                   // follower follows id
            }
            sna.v.push_back(u);
        }
    }
    void extractInputFromMinifiedXML(string&s, SNA&sna){
        string temp;
        s= minifying(s);
        auto data= tagsData(s);
        for(auto x:data) cout<<x<<" ";
        cout<<endl;
        vector<string>res;
        sna.takeInput(sna);
    }
    string getMostInfluencer(SNA&sna) {                             // O(n^2)
        int mx = 0, influencer = -1;
        for (int i = 1; i <= sna.adj.size(); i++)
            if (sna.adj[i].size() > mx)
                mx = (int)sna.adj[i].size(), influencer = i;
        return sna.antiID[influencer];
    }
    string getMostActive(SNA&sna) {
        int mx = 0, active = -1;
        vector<int>freq(sna.adj.size());
        for (int i = 1; i <= sna.adj.size(); i++)                   // O(n^2)
            for(int j:sna.adj[i]) {
                freq[j]++;
                if (freq[j] > mx)
                    mx = freq[j], active = j;
            }
        return sna.antiID[active];
    }
    vector<string>getMutualFriends(string&a, string&b, SNA&sna){                // O(n^2)
        vector<string>mutuals;
        for(int x:adj[sna.ID[a]])
            for(int y:adj[sna.ID[b]])
                if(y==x && y!=sna.ID[a])
                    mutuals.push_back(sna.antiID[y]);
        return mutuals;
    }
    vector<string>suggestFollowers(string&a, SNA&sna){                          // O(n^2)
        vector<string>suggestedFriends;
        unordered_map<int, int>temp;
        temp[sna.ID[a]]=-1;
        for(int x:adj[sna.ID[a]]) temp[x]=-1;
        for(int x:adj[sna.ID[a]])
            for(int y:adj[x])
                if(~temp[y])
                    suggestedFriends.push_back(sna.antiID[y]), temp[y]=-1;
        return suggestedFriends;
    }
};

int main() {
    FIO
    ofstream MyFile("newInput.txt");
    string s = "<users><user><id>1</id><name>Mohammed Farouk</name><posts><post><body>hi this is Mohammed Farouk, and I'm using facebook</body><topics><topic> work </topic></topics></post><post><body> I'm in love with codeforces </body><topics><topic> personal </topic></topics></post></posts><followers><follower><id>3</id></follower><follower><id>4</id></follower></followers></user>"
               "<user><id>2</id><name>Karim</name><posts><post><body>hi this is Karim, and I'm using instagram</body><topics><topic> work </topic></topics></post><post><body> hi this is Karim, and I'm using LinkedIn </body><topics><topic> work </topic></topics></post></posts><followers><follower><id>3</id></follower><follower><id>4</id></follower></followers></user>"
               "<user><id>3</id><name>Ali</name><posts><post><body>this is not a priority for me to interfere with others work.</body><topics><topic> personal </topic><topic> nightly e3trafat </topic></topics></post><post><body> hi this is Ali, and I'm using facebook </body><topics><topic> work </topic></topics></post><post><body> hi there this is Ali, and I'm using GitHub </body><topics><topic> work </topic></topics></post><post><body> hello, this is Ali, and I'm using Twitter </body><topics><topic> work </topic></topics></post><post><body> hi! is Ali, and I'm using LinkedIn </body><topics><topic> work </topic></topics></post></posts><followers><follower><id>1</id></follower></followers></user>"
               "<user><id>4</id><name>Marwan Ahmed</name><posts><post><body>I wish I had friends</body><topics><topic> personal </topic></topics></post></posts><followers><follower><id>1</id></follower><follower><id>2</id></follower><follower><id>3</id></follower></followers></user>";
    // MyFile<<temp;
    // freopen("newInput.txt","r",stdin);
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    SNA sna=SNA();
    sna.extractInputFromMinifiedXML(s, sna);
    cout<<"The most Influencer is "<<sna.getMostInfluencer(sna)<<endl;
    cout<<"The most Active is "<<sna.getMostActive(sna)<<endl;
    string user1="Ali", user2="Marwan Ahmed";
    vector<string>mutuals=sna.getMutualFriends(user1, user2, sna);
    cout<<"Mutual friends between "<<user1<<" & "<<user2<<": ";
    for(auto&x:mutuals) cout<<x<<" ";
    cout<<endl;
    user1="Mohammed Farouk";
    vector<string>sug=sna.suggestFollowers(user1, sna);
    cout<<"Suggested friends for "<<user1<<": ";
    for(auto&x:sug) cout<<x<<" ";
    cout<<endl;
    return 0;
}
