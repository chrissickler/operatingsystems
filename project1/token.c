#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "vector.h"

struct tokenStr {
    int pos;
    int length;
    char **elements;
}

Token * tokenInit(const char *str, const char *del) {
    Token *token = malloc(sizeof(Token));

    TokenConstr(token, str, del);

    return token;
}

void TokenConstr(Token *token, const char *str, const char *del) {
    if (token == NULL) {
        return;
    }
    Vector* vec = VectorInit(1);
    char *buf = strdup(str);
    char* tok = strtok(buf,del);
    while(tok != NULL) {
        VectorPush(vec,strdup(tok));
        tok = strtok(NULL,del);
    }
}

void TokenDestr(Token *tok) {
    if (tok == NULL) {
        return;
    }
    int size = tok->length;
    char **strs = tok->elements;
    for (int i = 0; i < size; i++) {
        free(strs[i]);
    }
    free(strs);
}

void TokenDelete(Token *this) {
    if(this == NULL)
        return;
    TokenDestr(this);
    free(this);
}

const char* TokenNext(Token* this) {
    if( this == NULL )
        return NULL;
    if(this->pos >= this->length)
    {
        return NULL;
    }
    char* output = this->elements[this->pos];
    this->pos++;
    return output;
}

char* TokenNextCpy(Token* this) {
    if( this == NULL )
        return NULL;
    char *next = TokenNext(this);
    return (next==NULL)? NULL : strdup(next);
}

const char* TokenPeek(Token* token) {
    int len = token->length;
    if(token->pos >= len) {
        return 0x0;
    }
    char* output = token->elements[token->pos];
    return output;
}

char* TokenPeekCpy(Token* this) {
    char *peek = TokenPeek(this);
    return (peek==NULL)? NULL : strdup(peek);
}

int TokenHasTokens(Token* this) {
    if(this->pos < this->length)
    {
        return 1;
    }
    return 0;
}

int TokenCountTokens(Token* this) {
    return this->length - this->pos;
}

int TokenNumTokens(Token* this) {
    return this->length;
}

void TokenReset(Token* this) {
    this->pos = 0;
}

int TokenContains(Token* this, const char* str) {
    int len = this->length;
    for(int i = 0; i < len; i++) {
        if(strcmp(str,this->elements[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

const char * const * TokenTokens(Token *this) {
    return this->elements;
}

char** TokenTokensCpy(Token* this) {
    char** output = malloc(sizeof(char*) * this->length);
    int len = this->length;
    for(int i = 0; i < len; i++)
    {
        output[i] = strdup(this->elements[i]);
    }
    return output;
}

void TokenPopulateArray(Token* this, char *array[]) {
    int len = this->length;
    for(int i = 0; i < len; i++)
    {
        array[i] = strdup(this->elements[i]);
    }
}

void TokenPrint(Token *this) {
    int len = this->length;
    for(int i = 0; i < len; i++) {
        printf("Token #%d: %s\n", i, this->elements[i]);
    }
}





