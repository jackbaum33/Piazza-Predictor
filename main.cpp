#include <iostream>
#include "csvstream.hpp"
#include <sstream>
#include <map>
#include <set>
#include <cmath>
using namespace std;

/*
steps to do:
1. determine debug mode or not
2. process all the posts 
(use a map with each word that stores word as key and num times as val)
3. record the necessary values
4. run testing data and predict
5. print output
*/

set<string> unique_words(const string &str)
{
  istringstream source(str);
  set<string> words;
  string word;
  while (source >> word)
  {
    words.insert(word);
  }
  return words;
}

class Classifier
{
private:
  std::map<string, int> numTimesWordInPost;
  std::map<string, int> postLabels;
  std::map<string, std::map<string, int>> numWordsPerLabel;
  std::set<string> uniqueWordsTotal;
  std::map<string, double> labelLogPriorProbabilities;
  std::map<string, std::map<string, double>> logLikelihoodForLabel;
  bool debug;
  int numPosts;
  int vocabSize;
  void printTrainInfo();

public:
  Classifier(bool debugMode);
  void trainData(const string &trainFile);
  void testData(const string &testFile);
};

Classifier::Classifier(bool debugMode)
    : debug(debugMode)
{
  numPosts = 0;
  vocabSize = 0;
}

void Classifier::trainData(const string &trainFile)
{
  string currentLabel;
  string currentPost;
  std::map<string, string> currentRow;
  csvstream trainReader(trainFile);
  std::set<string> uniqueWordsInPost;

  if (debug)
  {
    cout << "training data:" << endl;
  }
  while (trainReader >> currentRow)
  {
    currentLabel = currentRow["tag"];
    currentPost = currentRow["content"];
    if (debug)
    {
      cout << "  label = " << currentLabel << ", content = " 
      << currentPost << endl;
    }
    numPosts++;
    uniqueWordsInPost = unique_words(currentPost);
    for (auto currentWord : uniqueWordsInPost)
    {
      numTimesWordInPost[currentWord]++;
      uniqueWordsTotal.insert(currentWord);
      numWordsPerLabel[currentLabel][currentWord]++;
    }
    postLabels[currentLabel]++;
  }
  vocabSize = uniqueWordsTotal.size();
  printTrainInfo();
}

void Classifier::printTrainInfo()
{
  bool alreadyPrintedHeader = false;
  cout << "trained on " << numPosts << " examples" << endl;
  if (debug)
  {
    cout << "vocabulary size = " << vocabSize << endl;
  }
  cout << endl;
  if (debug)
  {
    cout << "classes:" << endl;
  }
  for (auto label : postLabels)
  {
    double currentLogPriorProbability = log(postLabels[label.first] / (numPosts * 1.0));
    labelLogPriorProbabilities[label.first] = currentLogPriorProbability;
    if (debug)
    {
      cout << "  " << label.first << ", " << postLabels[label.first] 
      << " examples, log-prior = "
           << currentLogPriorProbability << endl;
    }
  }
  for (auto label : postLabels)
  {
    for (auto wordPair : numWordsPerLabel[label.first])
    {
      if (!alreadyPrintedHeader && debug)
      {
        cout << "classifier parameters:" << endl;
        alreadyPrintedHeader = true;
      }

      // first check if it is anywhere in the data
      if (uniqueWordsTotal.find(wordPair.first) == uniqueWordsTotal.end())
      {
        double currentLogLikelihood = log(1.0 / numPosts);
        logLikelihoodForLabel[label.first][wordPair.first] = currentLogLikelihood;
      }
      // then check if it shows up in posts with current label
      else if (numWordsPerLabel[label.first][wordPair.first] == 0)
      {
        double currentLogLikelihood = log(numTimesWordInPost[wordPair.first] 
        / (numPosts * 1.0));
        logLikelihoodForLabel[label.first][wordPair.first] = currentLogLikelihood;
      }
      // otherwise we know it is containd in posts with current label
      else
      {
        double currentAmount = numWordsPerLabel[label.first][wordPair.first] 
        / ((postLabels[label.first] * 1.0));
        double currentLogLikelihood = log(currentAmount);
        logLikelihoodForLabel[label.first][wordPair.first] = currentLogLikelihood;
      }
      if (debug)
      {
        cout << "  " << label.first << ":" << wordPair.first << ", count = "
             << numWordsPerLabel[label.first][wordPair.first] << ", log-likelihood = "
             << logLikelihoodForLabel[label.first][wordPair.first] << endl;
      }
    }
  }
  if (debug)
    cout << endl;
}

void Classifier::testData(const string &testFile)
{
  std::map<string, string> currentRow;
  csvstream testReader(testFile);
  cout << "test data:" << endl;
  int numCorrect = 0;
  int numTests = 0;
  while (testReader >> currentRow)
  {
    numTests++;
    string currentLabel;
    string currentPost;
    std::map<string, double> chanceForEachLabel;
    double currentLogProbability;
    double highestProbability;
    string currentHighestLabel = (*postLabels.begin()).first;
    currentLabel = currentRow["tag"];
    currentPost = currentRow["content"];
    string originalPost = currentPost;
    std::set<string> wordsInPost = unique_words(currentPost);
    for (auto label : postLabels)
    {
      currentLogProbability = labelLogPriorProbabilities[label.first];
      for (auto currentWord : wordsInPost)
      {
         if (uniqueWordsTotal.find(currentWord) == uniqueWordsTotal.end())
      {
        currentLogProbability+=log(1.0 / numPosts);
      }
      else if (numWordsPerLabel[label.first][currentWord] == 0)
      {
          currentLogProbability+=log(numTimesWordInPost[currentWord] 
          / (numPosts * 1.0));
      }
      else
      {
        
        currentLogProbability+=log(numWordsPerLabel[label.first][currentWord] 
        / ((postLabels[label.first] * 1.0)));
      }
    }
    chanceForEachLabel[label.first] = currentLogProbability;
    }
    highestProbability = chanceForEachLabel[currentHighestLabel];
    for (auto label : postLabels)
    {
      if (chanceForEachLabel[label.first] > highestProbability)
      {
        currentHighestLabel = label.first;
        highestProbability = chanceForEachLabel[label.first];
      }
    }
    if (currentHighestLabel == currentLabel)
    {
      numCorrect++;
    }
    cout << "  correct = " << currentLabel << ", predicted = " << currentHighestLabel
         << ", log-probability score = " << 
         chanceForEachLabel[currentHighestLabel] << endl;
    cout << "  content = " << originalPost << endl
         << endl;
  }
  cout << "performance: " << numCorrect << " / " << numTests
       << " posts predicted correctly" << endl;
}

int main(int argc, char *argv[])
{
  cout.precision(3);

  if (argc != 3 && argc != 4)
  {
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
    return 1;
  }
  if (argc == 4)
  {
    string debugger = argv[3];
    if (!(debugger == "--debug"))
    {
      cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
      return 1;
    }
  }

  bool debugMode;
  if (argc == 4)
    debugMode = true;
  else
    debugMode = false;

  ifstream trainVerifier;
  trainVerifier.open(argv[1]);
  if (!trainVerifier.is_open())
  {
    cout << "Error opening file: " << argv[1] << endl;
    return 1;
  }
  trainVerifier.close();

  ifstream testVerifier;
  testVerifier.open(argv[2]);
  if (!testVerifier.is_open())
  {
    cout << "Error opening file: " << argv[2] << endl;
    return 1;
  }
  testVerifier.close();

  Classifier processor(debugMode);

  try{
    processor.trainData(argv[1]);
  }
  catch(const csvstream_exception c)
  {
    c.what();
    return 1;
  }
  try{
processor.testData(argv[2]);
  }
  catch(const csvstream_exception c)
  {
    c.what();
    return 1;
  }
}
