extern "C" {
    #include "jieba.h"
}

#include "cppjieba/MixSegment.hpp"
#include "cppjieba/KeywordExtractor.hpp"

using namespace std;

extern "C" {

Jieba NewJieba(const char* dict_path, const char* hmm_path, const char* user_dict) {
  Jieba handle = (Jieba)(new cppjieba::MixSegment(dict_path, hmm_path, user_dict));
  return handle;
}

void FreeJieba(Jieba handle) {
  cppjieba::MixSegment* x = (cppjieba::MixSegment*)handle;
  delete x;
}

char** Cut(Jieba handle, const char* sentence) {
  cppjieba::MixSegment* x = (cppjieba::MixSegment*)handle;
  vector<string> words;
  x->Cut(sentence, words);
  char ** res = (char**)malloc(sizeof(char*) * (words.size() + 1));
  for (size_t i = 0; i < words.size(); i++) {
    res[i] = (char*)malloc(sizeof(char) * (words[i].length() + 1));
    strcpy(res[i], words[i].c_str());
  }
  res[words.size()] = '\0';
  return res;
}

void FreeWords(char** words) {
  char** x = words;
  while (x && *x) {
    free(*x);
    *x = NULL;
    x ++;
  }
  free(words);
}

Extractor NewExtractor(const char* dict_path,
      const char* hmm_path,
      const char* idf_path,
      const char* stop_word_path,
      const char* user_dict_path) {
  Extractor handle = (Extractor)(new cppjieba::KeywordExtractor(dict_path, 
          hmm_path, 
          idf_path,
          stop_word_path,
          user_dict_path));
  return handle;
}

void FreeExtractor(Extractor handle) {
  cppjieba::MixSegment* x = (cppjieba::MixSegment*)handle;
  delete x;
}

char** Extract(Extractor handle, const char* sentence, int topn) {
  cppjieba::KeywordExtractor* x = (cppjieba::KeywordExtractor*)handle;
  vector<string> words;
  x->Extract(sentence, words, topn);
  char ** res = (char**)malloc(sizeof(char*) * (words.size() + 1));
  for (size_t i = 0; i < words.size(); i++) {
    res[i] = (char*)malloc(sizeof(char) * (words[i].length() + 1));
    strcpy(res[i], words[i].c_str());
  }
  res[words.size()] = '\0';
  return res;
}

}
