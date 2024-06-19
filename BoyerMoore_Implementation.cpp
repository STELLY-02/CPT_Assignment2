#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Initialize the occurrence table for the bad character heuristic
// occ[c] is the last position of character c in the pattern
void bmInitocc(const string &pattern, vector<int> &occ) {
    int alphabetsize = 256; // Assuming extended ASCII character set
    occ.assign(alphabetsize, -1); // Initialize all positions to -1
    for (int j = 0; j < pattern.size(); ++j) {
        occ[pattern[j]] = j; // Record the last occurrence of each character
    }
}

// Preprocess the pattern for the good suffix heuristic
// This function calculates the f and s arrays used for the good suffix rule
// calculates the rightmost borders of the suffixes of the pattern

// f array: indicates the rightmost borders of the suffixes
// s array: provides the shift values based on the good suffix heuristic
void bmPreprocess1(const string &pattern, vector<int> &s, vector<int> &f) {
    int m = pattern.size();
    //starting from the last character of the pattern
    int i = m, j = m + 1;
    f[i] = j; // Initialize f[m] to m + 1

    // Compute the f and s arrays from right to left
    while (i > 0) {
        // Move j to the rightmost border of suffix starting at position i
        while (j <= m && pattern[i - 1] != pattern[j - 1]) {
            if (s[j] == 0) s[j] = j - i; // If s[j] is not already set, set it to the shift value
            j = f[j]; // Move to the next widest border
        }
        i--; j--;
        f[i] = j; // Set f[i] to the rightmost border position
    }
}

// Finalize the good suffix table s using the f array
// ensures that all necessary shifts for the good suffix heuristic are correctly set
void bmPreprocess2(const string &pattern, vector<int> &s, vector<int> &f) {
    int m = pattern.size();
    int j = f[0]; // Initialize j to the rightmost border of the entire pattern
    for (int i = 0; i <= m; ++i) {
        if (s[i] == 0) s[i] = j; // If s[i] is not set, set it to j
        if (i == j) j = f[j]; // Move to the next widest border
    }
}

// Wrapper function to preprocess the pattern for both heuristics
void bmPreprocess(const string &pattern, vector<int> &s, vector<int> &occ) {
    int m = pattern.size();
    vector<int> f(m + 1, 0); // Initialize f array
    s.assign(m + 1, 0); // Initialize s array

    bmInitocc(pattern, occ); // Initialize occurrence table
    bmPreprocess1(pattern, s, f); // Compute f and partial s
    bmPreprocess2(pattern, s, f); // Finalize s using f
}

// Print details of each iteration
void printIterationDetails(const string &text, const string &pattern, int i, int j, int bcShift, int gsShift, const string &situation) {
    cout << "Text:    " << text << endl;
    cout << "Pattern: " << string(i, ' ') << pattern << endl;
    cout << "Mismatch at pattern index " << j << " (text character '" << text[i + j] << "' != pattern character '" << pattern[j] << "')" << endl;
    cout << "Bad Character Shift: " << bcShift << endl;
    cout << "Good Suffix Shift: " << gsShift << endl;
    cout << "Situation: " << situation << endl;
    cout << "Shift applied: " << max(gsShift, bcShift) << endl;
    cout << "-----------------------------------" << endl << endl;
}

// Perform the Boyer-Moore search
void boyerMooreSearch(const string &text, const string &pattern) {
    int n = text.size();
    int m = pattern.size();

    vector<int> occ; // Occurrence table for bad character heuristic
    vector<int> s; // Good suffix shift table

    bmPreprocess(pattern, s, occ); // Preprocess the pattern

    int i = 0; // Start position in the text
    while (i <= n - m) {
        int j = m - 1; // Start position in the pattern
        while (j >= 0 && pattern[j] == text[i + j]) {
            j--; // Move left in the pattern and text while characters match
        }

        // Print the current alignment
        cout << "text\t: " << text << endl;
        cout << "pattern\t: " << string(i, ' ') << pattern << endl;

        if (j < 0) { // Pattern found
            cout << "===============================" << endl;
            cout << "Pattern found at index " << i << endl;
            cout << "===============================" << endl << endl;
            i += s[0]; // Shift the pattern by the full length of a good suffix
        } else {
            // Calculate shifts for bad character and good suffix heuristics
            int bcShift = max(1, j - occ[text[i + j]]);
            int gsShift = s[j + 1];
            cout << "-----------------------------------" << endl;
            cout << "Mismatch at index " << j << " ('" << pattern[j] << "' != '" << text[i + j] << "')" << endl;
            cout << "Bad Character shift\t:\t | " << bcShift << endl;
            cout << "Good Suffix shift\t:\t | " << gsShift << endl;
            cout << "-----------------------------------" << endl << endl << endl;
            i += max(gsShift, bcShift); // Shift by the maximum of the two shifts
        }
    }

    cout << endl;
}

int main() {
    // Example 1: More suitable with bad character heuristic
    string text1 = "ABCDEFGHIJK";
    string pattern1 = "IJK";
    boyerMooreSearch(text1, pattern1);

    // Example 2: More suitable with good suffix heuristic
    string text2 = "CBBCBBDBBABB";
    string pattern2 = "ABB";
    boyerMooreSearch(text2, pattern2);

    return 0;
}

