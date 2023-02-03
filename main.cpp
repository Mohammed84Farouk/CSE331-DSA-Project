#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define FIO ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
const int OO=0x3f3f3f3f;
int cnt;
class SNA {
public:
    struct post{
        string body;
        vector<string>topics;
        post(){}
    };
    struct user {
        int id;                                                 // they are given in an increasing order starting from 1
        string name;
        vector<post> posts;
        vector<int> followers;
        user(){}
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
                    if (tagName.find("/") == string::npos) tagsNames.push_back(tagName);
                    tagName = "";
                    continue;
                } else {
                    tagName += minifiedString[i];
                }
            }
        }
        return tagsNames;
    }
    void createFile(vector<user>&users) {
        ofstream MyFile("newOne.txt");
        MyFile << users.size() << '\n';
        for (int i = 0; i < users.size(); i++) {
            MyFile << users[i].id << " " << users[i].name << '\n';
            MyFile << users[i].posts.size() << '\n';
            for (int j = 0; j < users[i].posts.size(); j++) {
                MyFile << users[i].posts[j].body << '\n';
                MyFile << users[i].posts[j].topics.size() << '\n';
                for (auto &k: users[i].posts[j].topics) MyFile << k << '\n';
            }
            if (users[i].followers.empty()) MyFile << "0\n";
            else {
                MyFile << users[i].followers.size() << " ";
                for (int j = 0; j < users[i].followers.size(); j++) {
                    MyFile << users[i].followers[j];
                    if (j == users[i].followers.size() - 1) MyFile << '\n';
                    else MyFile << " ";
                }
            }
        }
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
    vector<user> extractInputFromMinifiedXML(string&s, SNA&sna){
        string temp;
        s= minifying(s);
        auto TAGS=sna.tags(s);
        auto DATA=sna.tagsData(s);
        for(auto&x:TAGS) cout<<x<<" ";
        cout<<endl;
        for(auto&x:DATA) cout<<x<<" ";
        cout<<endl;
        int sz=(int)DATA.size();
        vector<user>users;
        for(int i=0;i<sz;i++){
            user u;
            if(TAGS[i]=="users") continue;
            else if(TAGS[i]=="user"){
                int j;
                for(j=i+1;j<sz;j++, i=j-1){
                    if(TAGS[j]=="id") u.id=stoi(DATA[j]);
                    else if(TAGS[j]=="name") u.name=DATA[j];
                    else if(TAGS[j]=="posts" || TAGS[j]=="followers") continue;
                    else if(TAGS[j]=="post"){
                        post p;
                        int k;
                        for(k=j+1;k<sz;k++, j=k-1){
                            if(TAGS[k]=="body") p.body=DATA[k];
                            else if(TAGS[k]=="topics") continue;
                            else if(TAGS[k]=="topic") p.topics.push_back(DATA[k]);
                            else{                       //followers
                                u.posts.push_back(p);
                                break;
                            }
                        }
                        j=k-1;
                    }
                    else if(TAGS[j]=="follower"){
                        int k;
                        for(k=j+1;k<sz;k++, j=k-1){
                            if(TAGS[k]=="id") u.followers.push_back(stoi(DATA[k]));
                            else                        //followers
                                break;
                        }
                        j=k-1;
                    }
                    else{               // user
                        users.push_back(u);
                        break;
                    }
                    if(j==sz-1) users.push_back(u);
                }
                i=j-1;
            }
        }
        return users;
    }
    string getMostInfluencer(SNA&sna) {                             // O(n)
        int mx = 0, influencer = -1;
        for (int i = 1; i < sna.adj.size(); i++)
            if (sna.adj[i].size() > mx)
                mx = (int)sna.adj[i].size(), influencer = i;
        return sna.antiID[influencer];
    }
    string getMostActive(SNA&sna) {
        int mx = 0, active = -1;
        vector<int>freq(sna.adj.size());
        for (int i = 1; i < sna.adj.size(); i++)                   // O(n^2)
            for(int j:sna.adj[i]) {
                freq[j]++;
                if (freq[j] > mx)
                    mx = freq[j], active = j;
            }
        return sna.antiID[active];
    }
    vector<string>getMutualFollowers(string&a, string&b, SNA&sna){                // O(n^2)
        vector<string>mutuals;
        for(int x:adj[sna.ID[a]])
            for(int y:adj[sna.ID[b]])
                if(y==x && y!=sna.ID[a])
                    mutuals.push_back(sna.antiID[y]);
        return mutuals;
    }
    vector<string>suggestFollowers(string&a, SNA&sna){                          // O(n^2)
        vector<string>suggestedFollowers;
        unordered_map<int, int>temp;
        temp[sna.ID[a]]=-1;
        for(int x:adj[sna.ID[a]]) temp[x]=-1;
        for(int x:adj[sna.ID[a]])
            for(int y:adj[x])
                if(~temp[y])
                    suggestedFollowers.push_back(sna.antiID[y]), temp[y]=-1;
        return suggestedFollowers;
    }
    vector<pair<int, string>>postSearch(string &s, bool state, vector<user>&users, SNA&sna){             // state 0: topic, 1: body
        vector<pair<int, string>>postsFound;
        if(state) {     // search by body                               // O(n*m) n: number of users, m: number of posts
            for (auto &i: users)
                for (auto &j: i.posts)
                    if (j.body.find(s) != string::npos)
                        postsFound.emplace_back(i.id, j.body);
        }
        else{           // search by topic                              // O(n*m*t) n: #users, m: #posts, t: #topics
            for (auto &i: users)
                for (auto &j: i.posts)
                    for(auto &k: j.topics)
                        if (k.find(s) != string::npos)
                            postsFound.emplace_back(i.id, j.body);
        }
        return postsFound;
    }
    int dijkstra(int src, int tar, vector<vector<pair<int, int>>>&adj){
        vector<int>dist(adj.size(), OO);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>q;
        dist[src]=0;
        q.push({0, src});
        while(!q.empty()){
            int u=q.top().second, c=q.top().first;
            q.pop();
            if(u==tar) return c;
            if(c!=dist[u]) continue;
            for(auto p:adj[u]){
                int v=p.second, vc=p.first;
                if(dist[v]>dist[u]+vc) dist[v]=dist[u]+vc, q.push({dist[v], v});
            }
        }
        return -1;
    }
    void dfs(int from, int to, int curDist, int cost, vector<vector<pair<int, int>>>&dij, vector<bool>&vis){
        vis[from]=true;
        if(from == to && curDist==cost){
            cnt++;
        }
        for(pair<int, int> p:dij[from])
            if(!vis[p.second])
                dfs(p.second, to , curDist+p.first, cost, dij, vis);
        vis[from]=false;
    }
    void dfs2(int from, int to, int curDist, int cost, int passBy, bool state, vector<vector<pair<int, int>>>&dij, vector<bool>&vis){
        vis[from]=true;
        if(from == to && curDist==cost && state){
            cnt++;
        }
        for(pair<int, int> p:dij[from])
            if(!vis[p.second]){
                if(p.second==passBy) dfs2(p.second, to , curDist+p.first, cost, passBy, true, dij, vis);
                else dfs2(p.second, to, curDist+p.first, cost, passBy, state, dij, vis);
            }
        vis[from]=false;
    }
    string highestBetweenness(SNA&sna){
        /*
         * firstly get all shortest paths from all pairwise users                       DONE
         * secondly count them in a frequency map of pairs(now we got the denominator)  DONE
         * thirdly, for each user count number of the shortest path                     DONE
         *
         */
        vector<vector<pair<int, int>>>dij(sna.adj.size());
        for(int i=1;i<adj.size();i++)
            for(int j:adj[i])
                dij[j].push_back({1, i});
        map<pair<int, int>, int>shortestDistance;
        //        from, to, len
        map<tuple<int, int, int>, int>freqDen, freqNom;
        // here we get the shortest paths from all pairwise users and their frequencies.
        for(int i=1;i<adj.size();i++)
            for(int j=1;j<adj.size();j++)
                if(i!=j) {
                    auto temp=dij;
                    int cost=dijkstra(i, j, temp);
                    vector<bool>vis(dij.size(), false);
                    if(cost==-1) shortestDistance[{i, j}]=OO;       // it has no frequency, so I won't take it into consideration
                    else {
                        shortestDistance[{i, j}] = cost;
                        cnt=0;
                        dfs(i, j, 0, cost, dij, vis);
                        freqDen[tuple<int, int, int>(i, j, cost)]=cnt;
                    }
//                    cout<<"shortestDistance from " << i << " to " << j <<" is equal to "<<cost << " and they have freqDen of "<<freqDen[tuple<int, int, int>(i, j, cost)]<<endl;
                }
        //
        map<int, double>betweenness;
        double mx=0;
        int idx=-1;
        for(int id=1;id<adj.size();id++){
//            cout<<"for "<<id<<": ";
            for(int i=1;i<adj.size();i++)
                for(int j=1;j<adj.size();j++)
                    if(i!=j && j!=id) {
                        int cost=shortestDistance[{i, j}];
                        vector<bool>vis(adj.size(), false);
                        cnt=0;
                        dfs2(i, j, 0, cost, id, false, dij, vis);
                        freqNom[tuple<int, int, int>(i, j, cost)]=cnt;
//                        cout<<"shortestDistance from " << i << " to " << j <<" is equal to "<<cost << " and they have freqNom of "<<freqNom[tuple<int, int, int>(i, j, cost)]<<" ";
                        betweenness[id]+=double(freqNom[tuple<int, int, int>(i, j, cost)])/double(freqDen[tuple<int, int, int>(i, j, cost)]);
//                        cout<<betweenness[id]<<endl;
                        if(betweenness[id]>mx) mx=betweenness[id], idx=id;
                    }
        }
        return sna.antiID[idx];
    }
};

int main() {
    FIO
//    freopen("newOne.txt","r",stdin);
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    string s = "<users><user><id>1</id><name>Mohammed Farouk</name><posts><post><body>hi this is Mohammed Farouk, and I'm using facebook</body><topics><topic> work </topic></topics></post><post><body> I'm in love with codeforces </body><topics><topic> personal </topic></topics></post></posts><followers><follower><id>3</id></follower><follower><id>4</id></follower></followers></user>"
               "<user><id>2</id><name>Karim</name><posts><post><body>hi this is Karim, and I'm using instagram</body><topics><topic> work </topic></topics></post><post><body> hi this is Karim, and I'm using LinkedIn </body><topics><topic> work </topic></topics></post></posts><followers><follower><id>1</id></follower><follower><id>3</id></follower><follower><id>4</id></follower></followers></user>"
               "<user><id>3</id><name>Ali</name><posts><post><body>this is not a priority for me to interfere with others work.</body><topics><topic> personal </topic><topic> nightly e3trafat </topic></topics></post><post><body> hi this is Ali, and I'm using facebook </body><topics><topic> work </topic></topics></post><post><body> hi there this is Ali, and I'm using GitHub </body><topics><topic> work </topic></topics></post><post><body> hello, this is Ali, and I'm using Twitter </body><topics><topic> work </topic></topics></post><post><body> hi! is Ali, and I'm using LinkedIn </body><topics><topic> work </topic></topics></post></posts><followers><follower><id>2</id></follower><follower><id>4</id></follower></followers></user>"
               "<user><id>4</id><name>Marwan Ahmed</name><posts><post><body>I wish I had friends</body><topics><topic> personal </topic></topics></post></posts><followers><follower><id>1</id></follower><follower><id>2</id></follower></followers></user>";
    string s2="<users>\n"
              "    <user>\n"
              "        <id>1</id>\n"
              "        <name>Ahmed Ali</name>\n"
              "        <posts>\n"
              "            <post>\n"
              "                <body>\n"
              "                    Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
              "                </body>\n"
              "                <topics>\n"
              "                    <topic>\n"
              "                        economy\n"
              "                    </topic>\n"
              "                    <topic>\n"
              "                        finance\n"
              "                    </topic>\n"
              "                </topics>\n"
              "            </post>\n"
              "            <post>\n"
              "                <body>\n"
              "                    Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
              "                </body>\n"
              "                <topics>\n"
              "                    <topic>\n"
              "                        solar_energy\n"
              "                    </topic>\n"
              "                </topics>\n"
              "            </post>\n"
              "        </posts>\n"
              "        <followers>\n"
              "            <follower>\n"
              "                <id>2</id>\n"
              "            </follower>\n"
              "            <follower>\n"
              "                <id>3</id>\n"
              "            </follower>\n"
              "        </followers>\n"
              "    </user>\n"
              "    <user>\n"
              "        <id>2</id>\n"
              "        <name>Yasser Ahmed</name>\n"
              "        <posts>\n"
              "            <post>\n"
              "                <body>\n"
              "                    Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
              "                </body>\n"
              "                <topics>\n"
              "                    <topic>\n"
              "                        education\n"
              "                    </topic>\n"
              "                </topics>\n"
              "            </post>\n"
              "        </posts>\n"
              "        <followers>\n"
              "            <follower>\n"
              "                <id>1</id>\n"
              "            </follower>\n"
              "        </followers>\n"
              "    </user>\n"
              "    <user>\n"
              "        <id>3</id>\n"
              "        <name>Mohamed Sherif</name>\n"
              "        <posts>\n"
              "            <post>\n"
              "                <body>\n"
              "                    Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
              "                </body>\n"
              "                <topics>\n"
              "                    <topic>\n"
              "                        sports\n"
              "                    </topic>\n"
              "                </topics>\n"
              "            </post>\n"
              "        </posts>\n"
              "        <followers>\n"
              "            <follower>\n"
              "                <id>1</id>\n"
              "            </follower>\n"
              "        </followers>\n"
              "    </user>\n"
              "</users>";
    SNA sna=SNA();
    auto TAGS=sna.tags(sna.minifying(s));
    auto DATA=sna.tagsData(sna.minifying(s));
    auto users=sna.extractInputFromMinifiedXML(s, sna);
    sna.createFile(users);

    sna.takeInput(sna);

//    cout<<"The most Influencer is "<<sna.getMostInfluencer(sna)<<endl;
//    cout<<"The most Active is "<<sna.getMostActive(sna)<<endl;
//    string user1="Ali", user2="Marwan Ahmed";
//    vector<string>mutuals=sna.getMutualFollowers(user1, user2, sna);
//    cout<<"Mutual followers between "<<user1<<" & "<<user2<<": ";
//    for(auto&x:mutuals) cout<<x<<" ";
//    cout<<endl;
//    user1="Mohammed Farouk";
//    vector<string>sug=sna.suggestFollowers(user1, sna);
//    cout<<"Suggested followers for "<<user1<<": ";
//    for(auto&x:sug) cout<<x<<" ";
//    cout<<endl;
//    string topic="personal";
//    auto x=sna.postSearch(topic, false, users, sna);
//    cout<<"post Search by topic personal: ";
//    for(auto&xx:x) cout<<xx.first<<" "<<xx.second<<endl;
//    cout<<endl;
//    string word="facebook";
//    auto y=sna.postSearch(word, true, users, sna);
//    cout<<"post Search by word facebook: ";
//    for(auto&yy:y) cout<<yy.first<<" "<<yy.second<<endl;
//    cout<<endl;
    cout<<"Best For Betweenness is "<<sna.highestBetweenness(sna);
    return 0;
}
