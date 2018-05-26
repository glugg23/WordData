#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "word.h"

int main() {
    std::ifstream randomWords("random.txt");
    std::vector<std::string> words;

    //Read words into vector so that we don't need to always scan the file
    std::string line;
    while(std::getline(randomWords, line)) {
        words.push_back(line);
    }

    randomWords.close();

    std::string longestWord;
    std::string shortestWord = "This is a just a very long string to make sure that it is actually changed";
    int total = 0;
    std::map<std::string, Word> wordMap;

    for(auto &word : words) {
        //Find longest word
        if(word.length() > longestWord.length()) {
            longestWord = word;
        }

        //Find shortest word
        if(word.length() < shortestWord.length()) {
            shortestWord = word;
        }

        //Used to find average
        total += word.length();

        //If word doesn't exist, add it to the map
        if(wordMap.count(word) == 0) {
            wordMap.insert(std::make_pair(word, Word(word)));

        } else {
            //Otherwise increment the value of that word
            wordMap.at(word).increment();
        }
    }

    std::cout << "Longest word: " << longestWord << ", at " << longestWord.length() << " characters" << std::endl;
    std::cout << "Shortest word: " << shortestWord << ", at " << shortestWord.length() << " characters" << std::endl;

    std::cout << "Average length of words: " << total / static_cast<double>(words.size()) << std::endl;

    Word mostCommon("");
    for(auto &pair : wordMap) {
        if(pair.second > mostCommon) {
            mostCommon = pair.second;
        }
    }

    std::cout << "Most common word: " << mostCommon.getWord() << ", at " << mostCommon.getCount() << " occurrences" << std::endl;

    return 0;
}