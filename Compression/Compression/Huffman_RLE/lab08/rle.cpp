#include "rle.h"
#include <math.h>
// Constructor
rle::rle(int gamma): gamma(gamma){}
string to_Bin(int i,int gamma){
    int j = 0;
    string out;
    while(i != 0){
        out = char((i & 1) + '0') +out;
        i /= 2;
    }
    for(int j = out.size(); j < (int)(gamma); j++){
        out = '0' + out;
    }
    return out;
}
int to_Int(string input){
    int curr = 1;
    int sum = 0;
    for(int i = input.size()-1;i >=0;i--){
        sum += (curr*(input[i] - '0'));
        curr *=2;
    }
    return sum;
}
// Convert integer to gamma-bit binary string
string rle::f(int i)
{   
    int gamma_goes = i / long(pow(2,gamma) - 1);
    int gamma_remainder = i % long(pow(2,gamma) - 1);

    
    // std::cout << gamma_goes << ":";
    string ones(gamma,'1');
    string zeros(gamma,'0');
    if(gamma_remainder == 0 && gamma_goes == 0) return zeros;
    

    string out = "";
    if(gamma_remainder == 0){gamma_goes--;}
    for(int i = 0; i < gamma_goes; i++){
        out = out + ones + zeros;
    }   
    if(gamma_remainder == 0){
        out = out + ones;
        return out;
    }

    out = out + to_Bin(gamma_remainder,gamma);

    return out;  
}

// Convert gamma-bit binary string to integer
int rle::g(string i)
{ 
    int sum = 0;
    for(int j = 0; j< i.size(); j+=4){
        sum += to_Int(i.substr(j,gamma));
    }
    return sum;
}

// Compress the input file
string rle::compress(string input_string)
{
    bool is_one = false;
    int count = 1;
    string out;
    if(input_string[0] == '1') is_one = true;
    else{
        out = string(gamma,'0');
    }
    // std::cout << input_string.size() << input_string;
    for(int i = 1; i < input_string.size();i++){
        if(is_one){
            if(input_string[i] == '0'){
                is_one= !is_one;
                out = out + f(count);
                count = 1;
            }
            else{
                count++;
            }
        }
        else{
            if(input_string[i] == '1'){
                is_one= !is_one;
                out = out + f(count);
                count = 1;
            }
            else{
                count++;
            }
        }
        // std::cout <<i << ":"<< count << std::endl;
    }
    
    out = out + f(count);
    return "1" + out;
}

// Decompress the input file
string rle::decompress(string input_string)
{   
    
    string out = "";
    char curr = '1';
 
    for(int i = 1; i < input_string.size(); i+=gamma){
        // std::cout << i << ":" << input_string.size() << std::endl;
        out =  out + string(g(input_string.substr(i,gamma)),curr) ;
        if(curr == '1') curr = '0';
        else curr = '1';
    }
    return out; 

}