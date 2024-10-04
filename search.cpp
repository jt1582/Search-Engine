#include "include/search.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  // TODO student
  string cleanedToken = "";
  bool hasLetter = false;
  int startIndex = 0;
  int endIndex = token.size() - 1;

  while (startIndex <= endIndex && ispunct(token[startIndex])) {
    startIndex++;
  }
  while (endIndex >= startIndex && ispunct(token[endIndex])) {
    endIndex--;
  }
  for (int i = startIndex; i <= endIndex; i++) {
    char c = token[i];
    cleanedToken += tolower(c);
    if (isalpha(c)) {
      hasLetter = true;
    }
  }
  if (!hasLetter) {
    return "";
  }
  return cleanedToken;
}

set<string> gatherTokens(const string& text) {
  // TODO student
  set<string> tokens;
  stringstream ss(text);
  string token;

  while (ss >> token) {
    string cleanedToken = cleanToken(token);
    if (!cleanedToken.empty()) {
      tokens.insert(cleanedToken);
    }
  }
  return tokens;
}

int buildIndex(const string& filename, map<string, set<string>>& index) {
  // TODO student
  ifstream file(filename);
  if (!file.is_open()) {
    return 0;
  }
  string url;
  string content;
  int pageCount = 0;
  while (getline(file, url) && getline(file, content)) {
    set<string> tokens = gatherTokens(content);
    for (const string& token : tokens) {
      index[token].insert(url);
    }
    pageCount++;
  }
  return pageCount;
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  // TODO student
  stringstream ss(sentence);
  string term;
  set<string> result;
  bool isFirstTerm = true;
  while (ss >> term) {
    char modifier = '\0';
    if (term[0] == '+' || term[0] == '-') {
      modifier = term[0];
      term = term.substr(1);
    }
    string cleanedTerm = cleanToken(term);
    set<string> termMatches;
    if (index.find(cleanedTerm) != index.end()) {
      termMatches = index.at(cleanedTerm);
    }
    if (isFirstTerm == true) {
      result = termMatches;
      isFirstTerm = false;
    } else {
      if (modifier == '+') {
        set<string> tempResult;
        set_intersection(result.begin(), result.end(), termMatches.begin(),
                         termMatches.end(),
                         inserter(tempResult, tempResult.begin()));
        result = tempResult;
      } else if (modifier == '-') {
        set<string> tempResult;
        set_difference(result.begin(), result.end(), termMatches.begin(),
                       termMatches.end(),
                       inserter(tempResult, tempResult.begin()));
        result = tempResult;
      } else {
        set<string> tempResult;
        set_union(result.begin(), result.end(), termMatches.begin(),
                  termMatches.end(), inserter(tempResult, tempResult.begin()));
        result = tempResult;
      }
    }
  }

  return result;
}

void searchEngine(const string& filename) {
  // TODO student
  map<string, set<string>> index;
  int numPages = buildIndex(filename, index);
  if (numPages == 0) {
    cout << "Invalid filename." << endl;
    cout << "Stand by while building index..." << endl;
    cout << "Indexed 0 pages containing 0 unique terms" << endl;
  } else {
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << numPages << " pages containing " << index.size()
         << " unique terms" << endl;
  }

  string query;
  while (true) {
    cout << "Enter query sentence (press enter to quit): ";
    getline(cin, query);
    if (query.empty()) {
      cout << "Thank you for searching!" << endl;
      break;
    }
    set<string> matches = findQueryMatches(index, query);
    cout << "Found " << matches.size() << " matching pages" << endl;
    for (string url : matches) {
      cout << url << endl;
    }
  }
}
