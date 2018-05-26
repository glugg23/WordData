#pragma once

#include <iostream>

class Word {
private:
    std::string word;
    int count = 1;

public:
    explicit Word(std::string word) : word(std::move(word)) {}

    std::string getWord() {
        return word;
    }

    int getCount() {
        return count;
    }

    void increment() {
        ++count;
    }

    bool operator==(const Word &rhs) const {
        return word == rhs.word;
    }

    bool operator!=(const Word &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const Word &rhs) const {
        return count < rhs.count;
    }

    bool operator>(const Word &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Word &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Word &rhs) const {
        return !(*this < rhs);
    }
};