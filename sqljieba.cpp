#include <vector>
#include <string>
#include <regex>
#include <my_config.h>
#include <m_ctype.h>
#include <mysql/plugin_ftparser.h>
#include <cppjieba/Jieba.hpp>

using cppjieba::Jieba;
using cppjieba::Word;

static const char* DICT_PATH      = "/usr/share/dict/jieba.dict.utf8";
static const char* MODEL_PATH     = "/usr/share/dict/hmm_model.utf8";
static const char* USER_DICT_PATH = "/usr/share/dict/user.dict.utf8";
static const char* IDF_PATH       = "/usr/share/dict/idf.utf8";
static const char* STOPWORD_PATH  = "/usr/share/dict/stop_words.utf8";

static const Jieba* jieba_handle = NULL;

/*
  sqljieba interface functions:

  Plugin declaration functions:
  - sqljieba_plugin_init()
  - sqljieba_plugin_deinit()

  Parser descriptor functions:
  - sqljieba_parse()
  - sqljieba_parser_init()
  - sqljieba_parser_deinit()
*/


/*
  Initialize the plugin at server start or plugin installation.
  NOTICE: when the DICT_PATH, HMM_MODEL, USER_DICT_PATH not found, NewJieba would log the error and exit without return anything .

  SYNOPSIS
    sqljieba_plugin_init()

  DESCRIPTION
    Does nothing.

  RETURN VALUE
    0                    success
    1                    failure (cannot happen)
*/

static int sqljieba_plugin_init(void *arg)
{
  jieba_handle = new Jieba(DICT_PATH, MODEL_PATH, USER_DICT_PATH, IDF_PATH, STOPWORD_PATH);
  return 0;
}

static int sqljieba_plugin_deinit(void *arg)
{
  if (jieba_handle) {
    delete jieba_handle;
  }
  return 0;
}


/*
  Initialize the parser on the first use in the query

  SYNOPSIS
    sqljieba_parser_init()

  DESCRIPTION
    Does nothing.

  RETURN VALUE
    0                    success
    1                    failure (cannot happen)
*/

static int sqljieba_parser_init(MYSQL_FTPARSER_PARAM *param)
{
  return 0;
}


/*
  Terminate the parser at the end of the query

  SYNOPSIS
    sqljieba_parser_deinit()

  DESCRIPTION
    Does nothing.

  RETURN VALUE
    0                    success
    1                    failure (cannot happen)
*/

static int sqljieba_parser_deinit(MYSQL_FTPARSER_PARAM *param)
{
  return 0;
}


/*
  Parse a document or a search query.

  SYNOPSIS
    sqljieba_parse()
      param              parsing context

  DESCRIPTION
    This is the main plugin function which is called to parse
    a document or a search query. The call mode is set in
    param->mode.  This function simply splits the text into words
    and passes every word to the MySQL full-text indexing engine.

  RETURN VALUE
    0                    success
    1                    failure (cannot happen)
*/

static int sqljieba_parse(MYSQL_FTPARSER_PARAM *param)
{
  // If Jieba is not found, use the built-in parser
  if (!jieba_handle) {
    return param->mysql_parse(param, param->doc, param->length);
  }

  // Convert C const char* to C++ string
  std::string doc(param->doc, param->length);
  // Segmented words
  // cpjieba::Word contains the offset information
  std::vector<Word> words;
  jieba_handle->CutForSearch(doc, words, true);

  // Used in boolean mode
  MYSQL_FTPARSER_BOOLEAN_INFO bool_info =
    { FT_TOKEN_WORD, 0, 0, 0, 0, 0, ' ', 0 };

  for (size_t i = 0; i < words.size(); i++) {
    // Skip white spaces
    if (words[i].word[0] == ' ')
      continue;

    // According to some use cases the MySQL will crash if we add
    // a word that is too long. Hence we manually remove words that are
    // more than 100 bytes
    if (words[i].word.size() > 100)
      continue;

    // Set offset and add the word
    bool_info.position = words[i].offset;
    param->mysql_add_word(param, param->doc + words[i].offset, words[i].word.size(), &bool_info);
  }

  return 0;
}


/*
  Plugin type-specific descriptor
*/

static struct st_mysql_ftparser sqljieba_descriptor =
{
  MYSQL_FTPARSER_INTERFACE_VERSION, /* interface version      */
  sqljieba_parse,                   /* parsing function       */
  sqljieba_parser_init,             /* parser init function   */
  sqljieba_parser_deinit            /* parser deinit function */
};

/*
  Plugin library descriptor
*/

mysql_declare_plugin(sqljieba)
{
  MYSQL_FTPARSER_PLUGIN,      /* type                            */
  &sqljieba_descriptor,       /* descriptor                      */
  "sqljieba",                 /* name                            */
  "github.com/yanyiwu",       /* author                          */
  "Jieba Full-Text Parser",   /* description                     */
  PLUGIN_LICENSE_GPL,
  sqljieba_plugin_init,       /* init function (when loaded)     */
  sqljieba_plugin_deinit,     /* deinit function (when unloaded) */
  0x0001,                     /* version                         */
  NULL,                       /* status variables                */
  NULL,                       /* system variables                */
  NULL,
  0,
}
mysql_declare_plugin_end;
