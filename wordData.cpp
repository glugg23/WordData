#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <thread>
#include <future>

#include "word.h"

void longestWordFunc(std::promise<std::string> &&longestWord, const std::vector<std::string> &words) {
    std::string currentWord;

    for(auto &word : words) {
        if(word.length() > currentWord.length()) {
            currentWord = word;
        }
    }

    longestWord.set_value(currentWord);
}

void shortestWordFunc(std::promise<std::string> &&shortestWord, const std::vector<std::string> &words) {
    //Large string to force a change when reading in the first word
    std::string currentWord = "1234567890123456789012345678901234567890123456789012345678901234567890";

    for(auto &word : words) {
        if(word.length() < currentWord.length()) {
            currentWord = word;
        }
    }

    shortestWord.set_value(currentWord);
}

void getAverage(std::promise<double> &&average, const std::vector<std::string> &words) {
    int total = 0;

    for(auto &word : words) {
        total += word.length();
    }

    average.set_value(static_cast<double>(total) / words.size());
}

int main() {
    std::ifstream randomWords("random.txt");
    std::vector<std::string> words;

    //Read words into vector so that we don't need to always scan the file
    std::string line;
    while(std::getline(randomWords, line)) {
        words.push_back(line);
    }

    randomWords.close();

    std::map<std::string, Word> wordMap;

    //Make a promise to return a string
    std::promise<std::string> longestWordPromise;
    //Set this value to go into here
    auto longestWordFuture = longestWordPromise.get_future();
    //Call the thread by moving the promise into it
    std::thread longestWordThread(longestWordFunc, std::move(longestWordPromise), std::ref(words));

    std::promise<std::string> shortestWordPromise;
    auto shortestWordFuture = shortestWordPromise.get_future();
    std::thread shortestWordThread(shortestWordFunc, std::move(shortestWordPromise), std::ref(words));

    std::promise<double> averagePromise;
    auto averageFuture = averagePromise.get_future();
    std::thread averageThread(getAverage, std::move(averagePromise), std::ref(words));

    for(auto &word : words) {
        //If word doesn't exist, add it to the map
        if(wordMap.count(word) == 0) {
            wordMap.insert(std::make_pair(word, Word(word)));

        } else {
            //Otherwise increment the value of that word
            wordMap.at(word).increment();
        }
    }

    longestWordThread.join();
    shortestWordThread.join();
    averageThread.join();

    //Get the value back from each future
    std::string longestWord = longestWordFuture.get();
    std::string shortestWord = shortestWordFuture.get();
    double average = averageFuture.get();

    std::cout << "Longest word: " << longestWord << ", at " << longestWord.length() << " characters" << std::endl;
    std::cout << "Shortest word: " << shortestWord << ", at " << shortestWord.length() << " characters" << std::endl;

    std::cout << "Average length of words: " << average << std::endl;

    Word mostCommon("");
    for(auto &pair : wordMap) {
        if(pair.second > mostCommon) {
            mostCommon = pair.second;
        }
    }

    std::cout << "Most common word: " << mostCommon.getWord() << ", at " << mostCommon.getCount() << " occurrences" << std::endl;

    return 0;
}