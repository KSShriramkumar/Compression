#ifndef HUFFMAN
#define HUFFMAN

#include <string>
#include <map>
#include <set>
#include <iostream>
#include <queue>
#include <bits/stdc++.h>
using std::cout, std::endl, std::string;
using std::map, std::set, std::pair, std::make_pair;

// a node of the huffman tree. Already written, observe what is going on.
class tree{
public:
    string value = "┐"; // ┐/0/1/character. for now we just use just ┐.
    tree *left = nullptr, *right = nullptr;
    int node_no;
    int nodes;
    tree(int node){node_no = node;}
    tree(char value,int node): node_no(node) ,value(1, value){}
    tree(tree *left): left(left){}
    tree(tree *left, tree *right,int node_no): left(left), right(right), node_no(node_no){}
    static tree *merge_trees(tree *left, tree *right, int node_no){
        return new tree(left, right,node_no);
    }

    void print(const string &prefix="", bool isLeftChild=false){
        cout << prefix << (prefix==""?"└──":(isLeftChild? "├─0":"└─1"));
        // cout << "value:" << value << "|" << endl;
        if (isLeaf()) cout << "─[" << value << "]" << endl; else cout << value << endl;
        if (left) left->print(prefix + (isLeftChild?"│  ":"   "), true);
        if (right) right->print(prefix + (isLeftChild?"│  ":"   "), false);
    }
    bool isLeaf() const{ return !(left && right); }
    void get_codes(map<char, string> &map_in_progress, const string &prefix="") const{
        if (left){
            if (left->isLeaf()) map_in_progress[left->value[0]] = prefix + "0";
            else left->get_codes(map_in_progress, prefix + "0");
        }
        if (right){ 
            if (right->isLeaf()) map_in_progress[right->value[0]] = prefix + "1";
            else right->get_codes(map_in_progress, prefix + "1");
        }
    }
    ~tree() {
        delete left; delete right;
    }
};

class Huffman_tree{
    tree* root;
    int no_nodes = 0;
};
// this returns the huffman tree using passed map of character to frequency
tree *huffman(const map<char, double> &freq_map)
{   
    tree* new_node;
    int node = 0;
    std::pair<double,tree*> node1,node2;
    std::priority_queue<std::pair<double,tree*>,std::vector<std::pair<double,tree*>>,std::greater<std::pair<double,tree*>>> heap;
    for(auto x:freq_map){
        new_node = new tree(x.first,node++);
        heap.push(make_pair(x.second,new_node));
    }

    while(heap.size() > 1){
        node1 = heap.top();heap.pop();
        node2 = heap.top();heap.pop();

        heap.push(std::make_pair(node1.first+node2.first,tree::merge_trees(node1.second,node2.second,node++)));
    }

    heap.top().second->nodes = node;
    return heap.top().second;
}

// compute the frequencies of characters in the string. For part 3, make sure to skip over backreferences and also set highest priority to the special character.
map<char, double> get_freqs(const string &buf) 
{ 
    map<char,double> freq_map;

    for(int i = 0; i < buf.size(); i++){
        freq_map[buf[i]]++;
    }
 

    return freq_map; 

}

void get_encoding(tree* huffman, string code, map<char,string>& encode){
    if(!huffman) return;
    if(huffman->isLeaf()){
        encode[huffman->value[0]] = code;
        return;
    }
    else{
        string temp;
        if(huffman->left){
            temp = code + '0';
            get_encoding(huffman->left,temp,encode);
        }
        if(huffman->right){
            temp = code + '1';
            get_encoding(huffman->right,temp,encode);
        }
    }
    return;
}


void get_char_map(tree* root, std::vector<char>& freq_map){
    if(root){
        freq_map[root->node_no] = root->value[0];
        get_char_map(root->right,freq_map);
        get_char_map(root->left,freq_map);
    }
}

void inorder(tree* root, std::vector<int>& in_order){
    if(!root) return;

    inorder(root->left,in_order);
    in_order.push_back(root->node_no);
    inorder(root->right,in_order);

}

void preorder(tree* root, std::vector<int>& pre_order){
    if(!root) return;

    pre_order.push_back(root->node_no);
    preorder(root->left,pre_order);
    preorder(root->right,pre_order);

}
#endif