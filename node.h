struct Node{
    unsigned char ch;
    int freq;
    Node* left;
    Node* right;
};

Node* createNode(unsigned char ch, int freq, Node* left, Node* right){
    Node* node = new Node();
    node -> ch = ch;
    node -> freq = freq;
    node -> left = left;
    node -> right = right;
    return node;
}

