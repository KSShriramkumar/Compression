#include "huffman-tree.cpp"
#include "constants.h"

// given a compressed binary string, convert it to the original string using the huffman tree
string inflate(const string &compressed, const tree *huffman_tree)
{   
    const tree* curr = huffman_tree;
    std::string out;
    for(int i = 0; i < compressed.size(); i++){
        if(curr->isLeaf()){
            out = out +  curr->value ; 
            curr = huffman_tree;
        }    
        if(compressed[i] == '0'){curr = curr->left;}
        else{curr = curr->right;}
    }
    return out;
}

// given a string, compress it using the huffman tree and return the resulting binary string
string deflate(const string &buf, tree* &huffman_tree)
{
    map<char,std::string> encoding;
    huffman_tree = huffman(get_freqs(buf));
    // huffman_tree->print();
    get_encoding(huffman_tree,"",encoding);
    // for(auto x: encoding){
    //     std::cout << x.first << ":" << x.second << std::endl;
    // }
    string out;
    for(int i =  0; i < buf.size();i++){
        out = out + encoding[buf[i]];
    }
    return out;
}