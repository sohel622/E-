#ifndef EPLUSPLUS_H
#define EPLUSPLUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Core Definitions for E++ Language Engine
#define MAX_VAR_NAME 50
#define MAX_STRING_LEN 256
#define MAX_VARS 100

// Token Types for Lexer and Parser
typedef enum {
    TOKEN_PRINT,
    TOKEN_VAR,
    TOKEN_IF,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_ASSIGN,
    TOKEN_SEMICOLON,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

// Token Structure
typedef struct {
    TokenType type;
    char lexer_value[MAX_STRING_LEN];
} Token;

// Variable Data Structure
typedef struct {
    char name[MAX_VAR_NAME];
    double value;
    char string_value[MAX_STRING_LEN];
    int is_string;
} Variable;

// Function Declarations
void set_variable(const char* name, double value);
void set_string_variable(const char* name, const char* str_val);
Variable* get_variable(const char* name);
Token get_next_token(char** src);
void interpret_eplusplus(char* code);

#endif // EPLUSPLUS_H
