#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
using namespace std::literals;

int main(){
    std::vector<std::string> words;
    std::ifstream ifs("words.txt");
    std::string word;
    while(ifs >> word){
        int left = 0, right = 0;
        bool leftFound = false;
        for(int i = 0; i < word.size(); i++){
            if((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z')){
                if(!leftFound){
                    leftFound = true;
                    left = i;
                }
                right = i;
                if(word[i] >= 'A' && word[i] <= 'Z'){
                    word[i] -= ('A' - 'a');  // 全部变成小写
                }
            }
        }
        if(!leftFound) continue;
        word = word.substr(left, right-left+1);
        bool isValid = true;
        for(int i = 0; i < word.size(); i++){
            if(word[i] < 'a' || word[i] > 'z'){
                isValid = false;
            }
        }
        if(isValid) words.push_back(word);
    }
    std::sort(words.begin(), words.end());
    auto last = std::unique(words.begin(), words.end());
    words.erase(last, words.end());
    // for(const auto& word: words){
    //     std::cout << word << std::endl;
    // }
    std::cout << words.size() << std::endl;

    std::ofstream ofs("words/all.txt", std::ios::ate);
    std::copy(words.begin(), words.end(), std::ostream_iterator<std::string>(ofs, "\n"));

    auto begin = words.begin();
    for(int i = 0; i < 26; i++){
        char c = 'a' + i;
        std::string fileName = "words/"s + c + ".txt"s;
        std::ofstream ofs(fileName, std::ios::ate);
        auto iter = begin;
        for(; iter < words.end(); ++iter){
            if((*iter)[0] != c){
                break;
            }
        }
        std::copy(begin, iter, std::ostream_iterator<std::string>(ofs, "\n"));
        begin = iter;
    }
    return 0;
}
