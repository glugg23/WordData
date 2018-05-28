#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <thread>
#include <future>

//#include "word.h"

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

void mostCommonFunc(std::promise<std::tuple<std::string, int>> &&mostCommon, const std::map<std::string, int> &words) {
    std::tuple<std::string, int> mostCommonTuple("", 0);
    for(auto &pair : words) {
        if(pair.second > std::get<int>(mostCommonTuple)) {
            mostCommonTuple = pair;
        }
    }

    mostCommon.set_value(mostCommonTuple);
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

    std::map<std::string, int> wordMap;

    //Find map of all unique words with their count
    for(auto &word : words) {
        //If word doesn't exist, add it to the map
        if(wordMap.count(word) == 0) {
            wordMap.insert(std::make_pair(word, 1));

        } else {
            //Otherwise increment the value of that word
            wordMap.at(word)++;
        }
    }

    std::promise<std::tuple<std::string, int>> mostCommonPromise;
    auto mostCommonFuture = mostCommonPromise.get_future();
    std::thread mostCommonThread(mostCommonFunc, std::move(mostCommonPromise), std::ref(wordMap));

    longestWordThread.join();
    shortestWordThread.join();
    averageThread.join();
    mostCommonThread.join();

    //Get the value back from each future
    std::string longestWord = longestWordFuture.get();
    std::string shortestWord = shortestWordFuture.get();
    double average = averageFuture.get();
    std::tuple<std::string, int> mostCommon = mostCommonFuture.get();

    std::cout << "Longest word: " << longestWord << ", at " << longestWord.length() << " characters" << std::endl;
    std::cout << "Shortest word: " << shortestWord << ", at " << shortestWord.length() << " characters" << std::endl;

    std::cout << "Average length of words: " << average << std::endl;

    std::cout << "Most common word: " << std::get<std::string>(mostCommon) << ", at " << std::get<int>(mostCommon) << " occurrences" << std::endl;

    return 0;
}