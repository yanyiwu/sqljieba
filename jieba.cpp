extern "C" {
    #include "jieba.h"
}

#include <string>
#include <vector>
#include <regex>
#include "cppjieba/Jieba.hpp"

using std::string;
using std::vector;

extern "C" {

Jieba NewJieba(const char* dict_path, const char* model_path, const char* user_dict_path, const char* idf_path, const char* stopword_path) {
  Jieba handle = (Jieba)(new cppjieba::Jieba(dict_path, model_path, user_dict_path, idf_path, stopword_path));
  return handle;
}

void FreeJieba(Jieba handle) {
  cppjieba::Jieba* x = (cppjieba::Jieba*)handle;
  delete x;
}

CJiebaWordCollection* CutForSearch(Jieba handle, const char* sentence, size_t len) {
  cppjieba::Jieba* x = (cppjieba::Jieba*)handle;
  vector<string> words;

  string s(sentence, len);
  // Only keep letters, numbers, and Chinese characters
  std::regex e1("[^\u2e80-\u3000\u3021-\ufe4fa-zA-Z0-9_]");
  s = std::regex_replace(s, e1, " ");
  // Combine multiple spaces to a single one
  std::regex e2(" +");
  s = std::regex_replace(s, e2, " ");

  x->CutForSearch(s, words);

  vector<size_t> ind;
  ind.reserve(words.size() / 2 + 1);
  for (size_t i = 0; i < words.size(); i++) {
    if (words[i] != " ") {
      ind.push_back(i);
    }
  }

  size_t nwords = ind.size();
  CJiebaWord* res = new CJiebaWord[nwords];

  for (size_t i = 0; i < nwords; i++) {
    size_t idx = ind[i];
    size_t len = words[idx].size();
    res[i].word = new char[len + 1];
    std::copy(words[idx].begin(), words[idx].end(), res[i].word);
    res[i].word[len] = 0;
    res[i].len = len;
  }

  CJiebaWordCollection* collection = new CJiebaWordCollection;
  collection->words = res;
  collection->nwords = nwords;

  return collection;
}

void FreeWords(CJiebaWordCollection* collection) {
  for (size_t i = 0; i < collection->nwords; i++) {
    delete [] collection->words[i].word;
  }
  delete [] collection->words;
  delete collection;
}

} // extern "C"
