#ifndef CJIEBA_C_API_H
#define CJIEBA_C_API_H

#include <stdlib.h>

typedef void* Jieba;
Jieba NewJieba(const char* dict_path, const char* model_path, const char* user_dict_path, const char* idf_path, const char* stopword_path);
void FreeJieba(Jieba);

typedef struct {
  char* word;
  size_t len;
} CJiebaWord;

typedef struct {
  CJiebaWord* words;
  size_t nwords;
} CJiebaWordCollection;

CJiebaWordCollection* CutForSearch(Jieba handle, const char* sentence, size_t len);

void FreeWords(CJiebaWordCollection* collection);

#endif
