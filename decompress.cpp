#include <bits/stdc++.h>
#include "node.h"
using namespace std;

const string ENCRYPTED = "encrypted";
const string DIR = "dir";

unordered_map< unsigned char, int > CodesSize;

int additionalBits;

unordered_map< string, unsigned char > getHuffmanCodes(string filePath){
    unordered_map< string, unsigned char > huffmanCodes;
    fstream file(filePath, ios::in);
    int n; file >> n >> additionalBits;
    short int c;
    string temp;
    for(int i = 0; i < n; ++i){
        file >> c >> temp;
        huffmanCodes[ temp ] = (char)c;
        CodesSize[ (char)c ] = (int)temp.size();
    }
    file.close();
    return huffmanCodes;

}

void addNewLeaf(Node* curr, int i, string& bits, char& c){
    if(i == (int)bits.size()){
        curr -> ch = c;
        return;
    }
    if(bits[ i ] == '0'){
        if(!curr -> left){
            curr -> left = createNode('\0', 0, nullptr, nullptr);
        }
        addNewLeaf(curr -> left, i + 1, bits, c);
    }
    else{
        if(!curr -> right){
            curr -> right = createNode('\0', 0, nullptr, nullptr);
        }
        addNewLeaf(curr -> right, i + 1, bits, c);
    }
}

Node* makeTree(string filePath){
    Node* root = createNode('\0', 0, nullptr, nullptr);
    unordered_map< string, unsigned char > huffmanCodes = getHuffmanCodes(filePath + "/dir");
    for(auto[ a, b ] : huffmanCodes){
        string bits = a;
        char c = b;
        addNewLeaf(root, 0, bits, c);
    }
    return root;
}

string getEncodedBits(string filePath){
    ifstream file(filePath + "/" + ENCRYPTED, ios::binary);
    stringstream ss;
    ss << file.rdbuf();
    string fileContent = ss.str();
    string bits;
    for (char byte : fileContent) {
        for (int j = 7; j >= 0; --j) {
            bits += ((byte >> j) & 1) ? '1' : '0';
        }
    }
    return bits.substr(0, bits.size() - additionalBits);
}

void findLetter(Node* root, int i, string& bits, string& text){
    if(!root -> left && !root -> right){
        text += root -> ch;
        return;
    }
    if(bits[ i ] == '0'){
        findLetter(root -> left, i + 1, bits, text);
    }
    else{
        findLetter(root -> right, i + 1, bits, text);
    }
}

string decode(Node* root, string& bits){
    string text = "";
    int i = 0;
    while(i < (int)bits.size()){
        findLetter(root, i, bits, text);
        i += CodesSize[ text.back() ];
    }
    return text;
}

void save(string filePath, string& s){
    fstream file(filePath + ".txt", ios::out);
    file << s;
    file.close();
}

void removeFile(string filePath){
    const string comand = "rm " + filePath;
    system(comand.c_str());
}

void removeFiles(string& dirPath){
    removeFile(dirPath + "/" + ENCRYPTED);
    removeFile(dirPath + "/" + DIR);
}

void removeDir(string dirPath){
    removeFiles(dirPath);
    const string comand = "rmdir " + dirPath;
    system(comand.c_str());
}

int main(int argc, char *argv[]){

    Node* root = makeTree(argv[ 1 ]);
    string bits = getEncodedBits(argv[ 1 ]);
    string decodedText = decode(root, bits);
    removeDir(argv[ 1 ]);
    save(argv[ 1 ], decodedText);


    return 0;
}
