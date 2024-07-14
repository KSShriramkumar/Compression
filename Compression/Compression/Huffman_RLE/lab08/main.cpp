#include "huffman-tree.cpp"
#include "huffman-zip.cpp"
#include "rle.cpp"
#include "constants.h"
#define ulg unsigned long int
using std::cout, std::endl, std::fstream;
using std::string, std::ios;

string next_block(fstream &f){
    string block; char c;
    for (int i = 0; i < BLOCK_SIZE; i++){
        if (f.eof()) break;
        f.get(c);
        block += c;
    } return block;
}

int main(int argc, char **argv){
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <input-file> <part>" << endl;
        return 1;
    }
    string infile = argv[1];
    fstream inf(infile);

    string block, comp, recon;
    ulg insz = 0, outsz = 0;

    // utilities for compression
    tree *huffman_tree;
    rle rlenc(4);

    int part = atoi(argv[2]);
    bool use_huffman=false, use_rle=false;
    switch (part) {
        case 2: use_huffman = true;
        case 1: use_rle = false;
    }

    while (!inf.eof()) {
        block = next_block(inf);
        insz += block.size();

        // compression
        comp = block;
        
        if (use_huffman) {
            comp = deflate(comp, huffman_tree);
            fstream tree_huffman("huffman_tree.txt");
            tree_huffman.clear();
            std::vector<char>char_map(huffman_tree->nodes);
            get_char_map(huffman_tree,char_map);

            std::vector<int> in_order;std::vector<int> pre_order;

            inorder(huffman_tree,in_order);

            preorder(huffman_tree,pre_order);

            for(int i = 0; i < huffman_tree->nodes;i++){
                tree_huffman << char_map[i] << " ";
            }
            tree_huffman << std::endl;
            for(int i = 0; i < huffman_tree->nodes;i++){
                tree_huffman << in_order[i] << " ";
            }
            tree_huffman<< std::endl;
            for(int i = 0; i < huffman_tree->nodes;i++){
            tree_huffman << pre_order[i] << " ";
            }
            tree_huffman<< std::endl;
            tree_huffman.close();

            fstream out("comps/huffman_comp.txt", ios::app); out << comp;
        }

        if (use_rle) {
            comp = rlenc.compress(comp);
            fstream out("comps/rle.txt", ios::app); out << comp;
        }

        outsz += comp.size();

        // decompression
        recon = comp;

        if (use_rle) {
            recon = rlenc.decompress(recon);
            fstream out("decomps/d1.txt", ios::app); out << recon;
        }

        if (use_huffman) {
            recon = inflate(recon, huffman_tree);
            fstream out("decomps/d2.txt", ios::app); out << recon;
        }

        cout << (block == recon ? "Success!" : "Failure!") << endl;
    }
    // print gain in bits
    if (part != 1) cout << "Ratio of final/original: " << outsz << "/" << 8*insz << " = " << 100.0 * ((double)outsz / (8*(double)insz)) << "\%\n";
    else cout << "Ratio of final/original: " << outsz << "/" << insz << " = " << 100.0 * ((double)outsz / ((double)insz)) << "\%\n";
}