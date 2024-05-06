#include <bits/stdc++.h>
#include "node.h"
using namespace std;

string readFile(string filePath){
    fstream file(filePath, ios::in);
    string characters = "";
    string temp = "";
    while(getline(file, temp)){
        characters += temp + '\n';
    }
    characters.pop_back();
    return characters;
}

unordered_map< unsigned char, int > makeDirectory(string& characters){
    unordered_map< unsigned char, int > freq;
    for(char& c : characters){
        freq[ c ]++;
    }
    return freq;
}

struct compare{
	bool operator()(Node* l, Node* r){
		return l->freq > r->freq;
	}
};

Node* makeTree(unordered_map< unsigned char, int >& freq){
    priority_queue< Node*, vector< Node* >, compare > pq;

    for(auto[key, val] : freq){
        pq.push(createNode(key, val, nullptr, nullptr));
    }

    while(pq.size() != 1){
        Node* n1 = pq.top(); pq.pop();
        Node* n2 = pq.top(); pq.pop();
        int addedFreq = n1 -> freq + n2 -> freq;
        pq.push(createNode('\0', addedFreq, n1, n2));
    }

    return pq.top();

}

void makeHuffmanCodes(Node* root, string s, unordered_map< unsigned char, string >& huffmanCodes){
    if(!root -> left && !root -> right){
        huffmanCodes[ root -> ch ] = s;
        return;
    }
    makeHuffmanCodes(root -> left, s + "0", huffmanCodes);
    makeHuffmanCodes(root -> right, s + "1", huffmanCodes);
}

unordered_map< unsigned char, string > getHuffmanCodes(Node* root){
    unordered_map< unsigned char, string > huffmanCodes;
    makeHuffmanCodes(root, "", huffmanCodes);
    return huffmanCodes;
}

string compressFilesBits(unordered_map< unsigned char, string >& huffmanCodes, string& characters){
    string ans = "";
    for(char& c : characters){
        ans += huffmanCodes[ c ];
    }
    return ans;
}


void removeFile(string filePath){
    const string comand = "rm " + filePath;
    system(comand.c_str());
}

void prepareFilePath(string& filePath){
    int dotIndex = filePath.find('.');
    filePath =filePath.substr(0, dotIndex);
}

void makedir(string& filePath){
    string comand = "mkdir " + filePath;
    system(comand.c_str());
}

void saveEncyptedCode(string filePath, string& bits, int& numBytes){
    ofstream file(filePath, ios::binary);
    for (int i = 0; i < numBytes; ++i) {
        char byte = 0;
        for (int j = 0; j < 8; ++j) {
            if (i * 8 + j < (int)bits.length() && bits[i * 8 + j] == '1') {
                byte |= (1 << (7 - j));
            }
        }
        file.write(&byte, sizeof(byte));
    }
    file.close();
}

void saveDir(string filePath, unordered_map< unsigned char, int >& freq, unordered_map< unsigned char, string >& huffmanCodes, int additionlBits){
    fstream file(filePath, ios::out);
    file << huffmanCodes.size() << ' ' << additionlBits << '\n';
    for(auto[ key, val ] : huffmanCodes){
        file << (short int)key << " " << val << '\n';
    }
    file.close();
}

void save(string filePath, string& bits, unordered_map< unsigned char, int >& freq, unordered_map< unsigned char, string >& huffmanCodes){
    prepareFilePath(filePath);
    makedir(filePath);
    int numBytes = ((bits.length() + 7) / 8);
    saveEncyptedCode(filePath + "/encrypted", bits, numBytes);
    saveDir(filePath + "/dir", freq, huffmanCodes, (numBytes * 8) - (int)bits.size());
}

int main(int argc, char *argv[]){
    
    string characters = readFile(argv[ 1 ]);
    unordered_map< unsigned char, int > freq = makeDirectory(characters);
    Node* root = makeTree(freq);
    unordered_map< unsigned char, string > huffmanCodes = getHuffmanCodes(root);
    string compressBits = compressFilesBits(huffmanCodes, characters);
    removeFile(argv[ 1 ]);
    save(argv[ 1 ], compressBits, freq, huffmanCodes);

    return 0;
}