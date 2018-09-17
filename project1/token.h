#ifndef __TOKEN_H__
#define __TOKEN_H__

struct tokenStr;
typedef struct tokenStr Token;

Token * TokenInit(const char *str, const char *del);
void TokenConstr(Token *token, const char *str, const char *del);
void TokenDestr(Token *tok);
void TokenDelete(Token *this);
const char* TokenNext(Token* this);
char* TokenNextCpy(Token* this);
const char* TokenPeek(Token* token);
char* TokenPeekCpy(Token* this);
int TokenHasTokens(Token* this);
int TokenCountTokens(Token* this);
int TokenNumTokens(Token* this);
void TokenReset(Token* this);
int TokenContains(Token* this, const char* str);
const char * const * TokenTokens(Token *this);
char** TokenTokensCpy(Token* this);
void TokenPopulateArray(Token* this, char *array[]);
void TokenPrint(Token *this);

#endif
