/*
 * Program: ngrams.cpp
 * -----------------------------------------------------
 * This program generates seemingly logical but de facto
 * random text from a text input file. The user must
 * specify the number of words to use as word groups
 * that form the basis for generating the random text.
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include "simpio.h"
#include "console.h"
#include "map.h"
#include "vector.h"
#include "random.h"

using namespace std;


/* Function prototypes */
string generateRandomText(Map<Vector<string>, Vector<string> > &collection, int numWords);
void updateNGram(Map<Vector<string>, Vector<string> > &collection, Vector<string> &window, string &next);
void readFile(Map<Vector<string>, Vector<string> > &collection, ifstream &in, int n);


/* Driver program */
int main() {

    cout << "Welcome to random writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << "\n";

    /* File must be a valid filename and N must be an
    integer not less than 2 */
    ifstream in;
    int n;
    while (true) {
        string filename = getLine("Input file name? ");
        in.open(filename.c_str());
        if (!in.fail()) break;
    }
    while (true) {
        n = getInteger("Value of N? ");
        if (n >= 2) break;
    }

    Map<Vector<string>, Vector<string> > collection;

    readFile(collection, in, n);

    /* Prompt user for new creation of text
    unless they want to quit */
    while (true) {
        cout << "\n";
        int numWords = getInteger("# of random words to generate (0 to quit): ");
        if (numWords == 0) break;
        cout << generateRandomText(collection, numWords) << endl;
    }

    cout << "Exiting.";

    return 0;
}


/*
 * Function readFile
 * Usage: readFile(wordmap, ifstream, n)
 * -----------------------------------------------------
 * Function reads in a file from an input file stream,
 * creates an initial nGram and works with a sliding
 * window to update word groups and the words that follow
 * in the text (Map: vector of word group -> vector of
 * words that follow). The function wraps around to
 * ensure that the last word groups also have respective
 * words that follow.
 *
 */

void readFile(Map<Vector<string>, Vector<string> > &collection, ifstream &in, int n) {

    Vector<string> window;

    string word;
    string next;

    // Initial N gram
    for (int i = 0; i < n; i++) {
        in >> word;
        window.add(word);
    }

    while (true) {
        in >> next;
        if (in.fail()) break;
        updateNGram(collection, window, next);
    }

    /* To enable wrap-around, repeat the procedure
    another N times */
    in.clear();
    in.seekg(0, in.beg);
    for (int i = 0; i< n; i++) {
        in >> next;
        if (in.fail()) break;
        updateNGram(collection, window, next);
    }

}


/*
 * Function updateNGram
 * Usage: updateNGram(wordmap, textwindow, nextword)
 * -----------------------------------------------------
 * Helper function to avoid duplicate code in readFile
 * function. It adds a new word to the sliding window
 * used for reading the text inputted by the user and
 * removes the first word so that the number of words
 * stay the same.
 *
 */

void updateNGram(Map<Vector<string>, Vector<string> > &collection, Vector<string> &window, string &next) {

    collection[window].add(next);
    window.remove(0);
    window.add(next);

}


/*
 * Function: generateRandomText
 * Usage: output = generateRandomText(wordmap, numWords)
 * -----------------------------------------------------
 * The function creates random output text from a
 * supplied map of random word groups and the desired
 * number of words to generate.
 *
 */

string generateRandomText(Map<Vector<string>, Vector<string> > &collection, int numWords) {

    string output = "... ";

    // Pick random start
    Vector<Vector<string> > keys = collection.keys();
    int randomInt = randomInteger(0, keys.size() - 1);

    // Use sliding window for text
    Vector<string> window = keys[randomInt];
    output += (window[0] + " " + window[1]);

    for (int i = 0; i < numWords - 2; i++) {
        randomInt = randomInteger(0, collection[window].size() - 1);
        string next = collection[window][randomInt];
        output += (" " + next);
        window.remove(0);
        window.add(next);
    }

    output += " ...";

    return output;
}
