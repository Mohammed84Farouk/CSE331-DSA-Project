#include <fstream>
#include <queue>
#include <vector>

using namespace std;

// Node of huffman tree
struct Node
{
    char data;
    int freq;
    string code;
    Node *left, *right;

    Node()
    {
        left = nullptr;
        right = nullptr;
    }
};

class Huffman
{
private:
    Node *root;
    vector<Node *> arr;
    fstream inFile, outFile;
    string inFileName, outFileName;
    struct Compare
    {
        bool operator()(Node *left, Node *right)
        {
            return (left->freq) > (right->freq);
        }
    };
    priority_queue<Node *, vector<Node *>, Compare> minHeap;

    // initially create vector of nodes contains the first 128 chars in ASCII table
    void createArr()
    {
        for (int i = 0; i < 128; i++)
        {
            arr.push_back(new Node());
            arr[i]->data = i;
            arr[i]->freq = 0;
        }
    }

    void createMinHeap()
    {
        char c;
        inFile.open(inFileName, ios::in);
        inFile.get(c);
        // Computing freq of each char appearing in the input file
        while (!inFile.eof())
        {
            arr[c]->freq++;
            inFile.get(c);
        }
        inFile.close();
        // Pushing the Nodes of each char into the minHeap
        for (int i = 0; i < 128; i++)
        {
            if (arr[i]->freq > 0)
            {
                minHeap.push(arr[i]);
            }
        }
    }

    /*
        To creating huffman tree:
        - pop the first two nodes from the minHeap
          (nodes that have the min freq)
        - create a new node with:
            -- left child = the node of the lowest freq
            -- right child = the another node
            -- freq = the sum of the two nodes freq
        - push the new node into the minHeap
        - repeat the previous three steps until remaining only one node in the minHeap
          (this last node is the root of the tree)
    */
    void createTree()
    {
        Node *left, *right;
        // Making a copy from minHeap to create huffman tree
        priority_queue<Node *, vector<Node *>, Compare> temp(minHeap);
        while (temp.size() != 1)
        {
            left = temp.top();
            temp.pop();

            right = temp.top();
            temp.pop();

            root = new Node();
            root->freq = left->freq + right->freq;
            root->left = left;
            root->right = right;

            temp.push(root);
        }
    }

    /*
        Creating code for each char appearing in the input file by traversing the tree:
        if(!leaf node)
            label the edge to the left child as 0
            label the edge to the right child as 1
    */
    void createCodes(Node *r, string str)
    {
        if (r == nullptr)
            return;
        if (r->left == nullptr && r->right == nullptr)
        {
            r->code = str;
            return;
        }

        createCodes(r->left, str + '0');
        createCodes(r->right, str + '1');
    }

    // To convert bits to decimal values
    int binToDec(string s)
    {
        return stoi(s, nullptr, 2);
    }

    /*
        To create the encoded file:
        - fristly save information of huffman tree to be able to decode the file
          (minHeap size)(ASCII code of char)(created code of that char in 128 bits)
            -- minHeap size
            -- ASCII code of each char appearing in the input file
            -- created code of each char but each in size = 128 bits
               (128 bits = (127 - code length) 0s + 1 + code)
               (these 128 bits will be saved as 16 decimal values)
        - secondly save each char by created code instead of ASCII code
            -- read char by char from the input file
            -- put created code of that char in string
            -- save every 8 bits in the string as 1 decimal value
            -- if(remaining bits in the string != 8 bits) padding with 0s and append its count
    */
    void createEncodedFile()
    {
        inFile.open(inFileName, ios::in);
        outFile.open(outFileName, ios::out | ios::binary);
        string encodedString = "";
        string s = "";

        // Saving minHeap size
        encodedString += (char)minHeap.size();
        // Making a copy of minHeap
        priority_queue<Node *, vector<Node *>, Compare> temp(minHeap);
        while (!temp.empty())
        {
            Node *curr = temp.top();
            // Saving ASCII code of the char
            encodedString += curr->data;
            // Making created code in the form: (padding wiht 0s)(1)(code) that equals to 128 bits
            s.assign(127 - curr->code.length(), '0'); // padding with 0s
            s += '1';
            s += curr->code;
            // Saving 128 bits as 16 decimal values, each 8 bits represent 1 decimal value
            encodedString += (char)binToDec(s.substr(0, 8));
            for (int i = 0; i < 15; i++)
            {
                s = s.substr(8);
                encodedString += (char)binToDec(s.substr(0, 8));
            }
            temp.pop();
        }
        s.clear();

        // Reading char by char from the input file
        char c;
        inFile.get(c);
        while (!inFile.eof())
        {
            s += arr[c]->code;
            // Saving every 8 bits as 1 decimal value
            while (s.length() > 8)
            {
                encodedString += (char)binToDec(s.substr(0, 8));
                s = s.substr(8);
            }
            inFile.get(c);
        }
        // if(remaining bits < 8) padding with 0s
        int count0 = 8 - s.length();
        if (s.length() < 8)
        {
            s.append(count0, '0');
        }
        encodedString += (char)binToDec(s);
        // Saving number of appended 0s
        encodedString += (char)count0;

        // Writing the encodedString string to the output file
        // outFile.write(encodedString.c_str(), encodedString.size());
        outFile << encodedString;
        inFile.close();
        outFile.close();
    }

    string decToBin(int decValue)
    {
        string temp = "", binValue = "";
        while (decValue > 0)
        {
            temp += (decValue % 2 + '0');
            decValue /= 2;
        }
        binValue.append(8 - temp.length(), '0');
        for (int i = temp.length() - 1; i >= 0; i--)
        {
            binValue += temp[i];
        }
        return binValue;
    }

    void reconstructTree(char ASCII_Code, string path)
    {
        Node *current = root;
        for (int i = 0; i < (int)path.length(); i++)
        {
            if (path[i] == '0')
            {
                if (current->left == nullptr)
                {
                    current->left = new Node();
                }
                current = current->left;
            }
            else if (path[i] == '1')
            {
                if (current->right == nullptr)
                {
                    current->right = new Node();
                }
                current = current->right;
            }
        }
        current->data = ASCII_Code;
    }

    // Reading the file to reconstruct the Huffman tree
    void getTree()
    {
        inFile.open(inFileName, ios::in | ios::binary);
        // Reading size of MinHeap
        unsigned char size;
        inFile.read(reinterpret_cast<char *>(&size), 1);
        root = new Node();
        // next (size * (1 + 16)) characters contain (ASCII code + huffman code in decimal) of each char
        for (int i = 0; i < size; i++)
        {
            char ASCII_Code;
            inFile.read(&ASCII_Code, 1);
            unsigned char huffmanDecimalCode[16];
            inFile.read(reinterpret_cast<char *>(huffmanDecimalCode), 16);

            // converting decimal characters into their binary equivalent to obtain code
            string huffmanBinaryCode = "";
            for (int i = 0; i < 16; i++)
            {
                huffmanBinaryCode += decToBin(huffmanDecimalCode[i]);
            }
            // Removing padding by ignoring first (127 - code length) '0's and next '1' character
            int j = 0;
            while (huffmanBinaryCode[j] == '0')
            {
                j++;
            }
            huffmanBinaryCode = huffmanBinaryCode.substr(j + 1);
            // Adding node with ASCII_Code data and huffmanBinaryCode to the huffman tree
            reconstructTree(ASCII_Code, huffmanBinaryCode);
        }
        inFile.close();
    }

    void createDecodedFile()
    {
        inFile.open(inFileName, ios::in | ios::binary);
        outFile.open(outFileName, ios::out);
        // Reading size
        unsigned char size;
        inFile.read(reinterpret_cast<char *>(&size), 1);
        // Reading count at the end of the file which is number of bits appended to make final value 8 bits
        inFile.seekg(-1, ios::end);
        char count0;
        inFile.read(&count0, 1);
        // Ignoring the information of huffman tree (1 + 17 * size)
        inFile.seekg(1 + 17 * size, ios::beg);

        // Reading the remaining file
        vector<unsigned char> encodedVector;
        unsigned char c;
        inFile.read(reinterpret_cast<char *>(&c), 1);
        while (!inFile.eof())
        {
            encodedVector.push_back(c);
            inFile.read(reinterpret_cast<char *>(&c), 1);
        }

        Node *current = root;
        string path;
        for (int i = 0; i < (int)encodedVector.size() - 1; i++)
        {
            // Converting decimal values to its equivalent 8-bit binary code
            path = decToBin(encodedVector[i]);
            if (i == (int)encodedVector.size() - 2)
            {
                path = path.substr(0, 8 - count0);
            }
            // Traversing huffman tree and appending data to the file
            for (int j = 0; j < (int)path.size(); j++)
            {
                if (path[j] == '0')
                {
                    current = current->left;
                }
                else
                {
                    current = current->right;
                }

                if (current->left == nullptr && current->right == nullptr)
                {
                    outFile << (current->data);
                    current = root;
                }
            }
        }
        inFile.close();
        outFile.close();
    }

public:
    // Constructor
    Huffman(string inFileName, string outFileName)
    {
        this->inFileName = inFileName;
        this->outFileName = outFileName;
        createArr();
    }
    // Compressing input file
    void compress()
    {
        createMinHeap();
        createTree();
        createCodes(root, "");
        createEncodedFile();
    }
    // Decrompressing input file
    void decompress()
    {
        getTree();
        createDecodedFile();
    }
};
