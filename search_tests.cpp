#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

TEST(CleanToken, PunctuationAtBothEnds) {
  string text = "...hello...";
  string expected = "hello";

  EXPECT_THAT(cleanToken(text), StrEq(expected)) << "text=\"" << text << "\"";
}
TEST(CleanToken, PunctuationInMiddleNotAtEnds) {
  string text = "hel-lo";
  string expected = "hel-lo";

  EXPECT_THAT(cleanToken(text), StrEq(expected)) << "text=\"" << text << "\"";
}
TEST(CleanToken, PunctuationInMiddleAndAtStart) {
  string text = "...hel-lo";
  string expected = "hel-lo";

  EXPECT_THAT(cleanToken(text), StrEq(expected)) << "text=\"" << text << "\"";
}
TEST(CleanToken, PunctuationInMiddleAndAtEnd) {
  string text = "hel-lo...";
  string expected = "hel-lo";

  EXPECT_THAT(cleanToken(text), StrEq(expected)) << "text=\"" << text << "\"";
}

TEST(GatherTokens, SpacesAtBeginning) {
  string text = "   hello world";
  set<string> expected = {"hello", "world"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, SpacesAtEnd) {
  string text = "hello world   ";
  set<string> expected = {"hello", "world"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, MultipleSpacesBetweenTokens) {
  string text = "hello    world";
  set<string> expected = {"hello", "world"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(BuildIndex, TinyTxt) {
  map<string, set<string>> index;
  string filename = "data/tiny.txt";

  int numPages = buildIndex(filename, index);
  EXPECT_EQ(numPages, 4);
}

TEST(BuildIndex, InvalidFilename) {
  map<string, set<string>> index;
  string filename = "data/nonexistent.txt";
  int numPages = buildIndex(filename, index);
  EXPECT_EQ(numPages, 0);
}

TEST(FindQueryMatches, FirstTermNotInMap) {
  map<string, set<string>> index = {{"apple", {"url1", "url2"}},
                                    {"banana", {"url2"}}};

  set<string> expected = {};

  EXPECT_THAT(findQueryMatches(index, "orange"), ContainerEq(expected))
      << "query=\"orange\"";
}

TEST(FindQueryMatches, PlusModifierTermNotInMap) {
  map<string, set<string>> index = {{"apple", {"url1", "url2"}},
                                    {"banana", {"url2"}}};
  set<string> expected = {};

  EXPECT_THAT(findQueryMatches(index, "apple +orange"), ContainerEq(expected))
      << "query=\"apple +orange\"";
}

TEST(FindQueryMatches, MinusModifierTermNotInMap) {
  map<string, set<string>> index = {{"apple", {"url1", "url2"}},
                                    {"banana", {"url2"}}};
  set<string> expected = {"url1", "url2"};

  EXPECT_THAT(findQueryMatches(index, "apple -orange"), ContainerEq(expected))
      << "query=\"apple -orange\"";
}

TEST(FindQueryMatches, UnmodifiedTermNotInMap) {
  map<string, set<string>> index = {{"apple", {"url1", "url2"}},
                                    {"banana", {"url2"}}};
  set<string> expected = {"url1", "url2"};

  EXPECT_THAT(findQueryMatches(index, "apple orange"), ContainerEq(expected))
      << "query=\"apple orange\"";
}
