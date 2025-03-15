#include "ladder.h"

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    int len1 = str1.size(), len2 = str2.size();
    if (abs(len1 - len2) > 1) return false;

    int diff_count = 0, i = 0, j = 0;
    while (i < len1 && j < len2) {
        if (str1[i] != str2[j]) {
            if (++diff_count > d) return false;
            if (len1 > len2) ++i;
            else if (len1 < len2) ++j;
            else { ++i; ++j; }
        } else { ++i; ++j; }
    }
    return diff_count + (len1 - i) + (len2 - j) <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file) throw runtime_error("Unable to open " + file_name);
    string word;
    while (file >> word) word_list.insert(word);
    file.close();
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) return {};

    queue<vector<string>> ladder_queue;
    set<string> visited;
    ladder_queue.push({begin_word});
    visited.insert(begin_word);

    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();

        for (const string& word : word_list) {
            if (is_adjacent(last_word, word) && !visited.count(word)) {
                vector<string> new_ladder = ladder;
                new_ladder.push_back(word);
                if (word == end_word) return new_ladder;
                ladder_queue.push(new_ladder);
                visited.insert(word);
            }
        }
    }
    return {};
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) cout << "No word ladder found.\n";
    else for (const string& word : ladder) cout << word << (word == ladder.back() ? "\n" : " ");
}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    cout << (generate_word_ladder("cat", "dog", word_list).size() == 4 ? "Test Passed\n" : "Test Failed\n");
}
