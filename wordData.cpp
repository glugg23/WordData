#include <iostream>
#include <fstream>
#include <map>
#include <future>

std::string longestWordFunc(const std::map<std::string, int> &words) {
    std::string currentWord;

    for(auto &word : words) {
        if(word.first.length() > currentWord.length()) {
            currentWord = word.first;
        }
    }

    return currentWord;
}

std::string shortestWordFunc(const std::map<std::string, int> &words) {
    //Large string to force a change when reading in the first word
    std::string currentWord = "1234567890123456789012345678901234567890123456789012345678901234567890";

    for(auto &word : words) {
        if(word.first.length() < currentWord.length()) {
            currentWord = word.first;
        }
    }

    return currentWord;
}

double getAverage(const std::map<std::string, int> &words) {
    int totalCharacters = 0;
    int totalWords = 0;

    for(auto &word : words) {
        totalCharacters += (word.first.length() * word.second);
        totalWords += word.second;
    }

    return static_cast<double>(totalCharacters) / totalWords;
}

std::tuple<std::string, int> mostCommonFunc(const std::map<std::string, int> &words) {
    std::tuple<std::string, int> mostCommonTuple;
    for(auto &pair : words) {
        if(pair.second > std::get<int>(mostCommonTuple)) {
            mostCommonTuple = pair;
        }
    }

    return mostCommonTuple;
}

int main() {
    std::ifstream randomWords("random.txt");
    std::map<std::string, int> wordMap;

    //Read words into vector so that we don't need to always scan the file
    std::string line;
    while(std::getline(randomWords, line)) {
        if(wordMap.count(line) == 0) {
            wordMap.insert(std::make_pair(line, 1));

        } else {
            //Otherwise increment the value of that word
            wordMap.at(line)++;
        }
    }

    randomWords.close();

    //Shows error "No matching function" but compiles and runs fine
    auto longestWordThread = std::async(std::launch::async, longestWordFunc, std::ref(wordMap));
    auto shortestWordThread = std::async(std::launch::async, shortestWordFunc, std::ref(wordMap));
    auto averageThread = std::async(std::launch::async, getAverage, std::ref(wordMap));
    auto mostCommonThread = std::async(std::launch::async, mostCommonFunc, std::ref(wordMap));

    //Get the value back from each future
    std::string longestWord = longestWordThread.get();
    std::string shortestWord = shortestWordThread.get();
    double average = averageThread.get();
    std::tuple<std::string, int> mostCommon = mostCommonThread.get();

    std::cout << "Longest word: " << longestWord << ", at " << longestWord.length() << " characters" << std::endl;
    std::cout << "Shortest word: " << shortestWord << ", at " << shortestWord.length() << " characters" << std::endl;
    std::cout << "Average length of words: " << average << std::endl;
    std::cout << "Most common word: " << std::get<std::string>(mostCommon) << ", at " << std::get<int>(mostCommon) << " occurrences" << std::endl;

    return 0;
}