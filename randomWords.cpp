#include <iostream>
#include <fstream>
#include <vector>
#include <random>

int main() {
    //Read in all words from dictionary file
    std::ifstream dictionary("dictionary.txt");
    std::vector<std::string> words;

    std::string line;
    while(std::getline(dictionary, line)) {
        words.push_back(line);
    }

    //Create a random number generator to pick random element from words
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, words.size() - 1);

    std::ofstream output("random.txt");

    //Write 1 million random words to file
    for(int i = 0; i < 1000000; ++i) {
        //Dirty fix to stop the last line being a newline
        if(i == 1000000 - 1) {
            output << words[dist(rng)];

        } else {
            output << words[dist(rng)] << '\n';
        }
    }

    return 0;
}