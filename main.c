#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// E++ (E Plus Plus) Advanced Language Engine
// Version 1.0.0 - Developed by Sohel

#define MAX_CODE_SIZE 1024
#define MAX_VARS 100
#define MAX_VAR_NAME 50
#define MAX_STRING_LEN 256

// Token Types for Lexer
typedef enum {
    TOKEN_PRINT,
    TOKEN_VAR,
    TOKEN_IF,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_EQUALS,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_SEMICOLON,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char lexer_value[MAX_STRING_LEN];
} Token;

// Variable Storage Structure for Symbol Table
typedef struct {
    char name[MAX_VAR_NAME];
    double value;
    char string_value[MAX_STRING_LEN];
    int is_string;
} Variable;

Variable symbol_table[MAX_VARS];
int var_count = 0;

// Function to set or update a numeric variable
void set_variable(const char* name, double value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            symbol_table[i].value = value;
            symbol_table[i].is_string = 0;
            return;
        }
    }
    if (var_count < MAX_VARS) {
        strcpy(symbol_table[var_count].name, name);
        symbol_table[var_count].value = value;
        symbol_table[var_count].is_string = 0;
        var_count++;
    }
}

// Function to set or update a string variable
void set_string_variable(const char* name, const char* str_val) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            strcpy(symbol_table[i].string_value, str_val);
            symbol_table[i].is_string = 1;
            return;
        }
    }
    if (var_count < MAX_VARS) {
        strcpy(symbol_table[var_count].name, name);
        strcpy(symbol_table[var_count].string_value, str_val);
        symbol_table[var_count].is_string = 1;
        var_count++;
    }
}

// Function to get variable value
Variable* get_variable(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return &symbol_table[i];
        }
    }
    return NULL;
}

// Lexer: Tokenize source code
Token get_next_token(char** src) {
    Token token;
    token.lexer_value[0] = '\0';

    while (**src != '\0' && isspace(**src)) {
        (*src)++;
    }

    if (**src == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    // Keywords or Identifiers
    if (isalpha(**src) || **src == '_') {
        int i = 0;
        while (isalnum(**src) || **src == '_') {
            token.lexer_value[i++] = **src;
            (*src)++;
        }
        token.lexer_value[i] = '\0';

        if (strcmp(token.lexer_value, "print") == 0) {
            token.type = TOKEN_PRINT;
        } else if (strcmp(token.lexer_value, "var") == 0) {
            token.type = TOKEN_VAR;
        } else if (strcmp(token.lexer_value, "if") == 0) {
            token.type = TOKEN_IF;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        return token;
    }

    // Numbers
    if (isdigit(**src)) {
        int i = 0;
        while (isdigit(**src) || **src == '.') {
            token.lexer_value[i++] = **src;
            (*src)++;
        }
        token.lexer_value[i] = '\0';
        token.type = TOKEN_NUMBER;
        return token;
    }

    // Strings
    if (**src == '"') {
        (*src)++; // Skip opening quote
        int i = 0;
        while (**src != '"' && **src != '\0') {
            token.lexer_value[i++] = **src;
            (*src)++;
        }
        token.lexer_value[i] = '\0';
        if (**src == '"') (*src)++; // Skip closing quote
        token.type = TOKEN_STRING;
        return token;
    }

    // Operators and Symbols
    char c = **src;
    (*src)++;

    switch (c) {
        case '=':
            if (**src == '=') {
                (*src)++;
                token.type = TOKEN_EQUALS;
                strcpy(token.lexer_value, "==");
            } else {
                token.type = TOKEN_ASSIGN;
                strcpy(token.lexer_value, "=");
            }
            break;
        case '+': token.type = TOKEN_PLUS; strcpy(token.lexer_value, "+"); break;
        case '-': token.type = TOKEN_MINUS; strcpy(token.lexer_value, "-"); break;
        case '*': token.type = TOKEN_MULTIPLY; strcpy(token.lexer_value, "*"); break;
        case '/': token.type = TOKEN_DIVIDE; strcpy(token.lexer_value, "/"); break;
        case '(': token.type = TOKEN_LPAREN; strcpy(token.lexer_value, "("); break;
        case ')': token.type = TOKEN_RPAREN; strcpy(token.lexer_value, ")"); break;
        case '{': token.type = TOKEN_LBRACE; strcpy(token.lexer_value, "{"); break;
        case '}': token.type = TOKEN_RBRACE; strcpy(token.lexer_value, "}"); break;
        case ';': token.type = TOKEN_SEMICOLON; strcpy(token.lexer_value, ";"); break;
        default:
            token.type = TOKEN_UNKNOWN;
            token.lexer_value[0] = c;
            token.lexer_value[1] = '\0';
            break;
    }

    return token;
}

// Expression Evaluator for Math and Variables
double evaluate_expression(char** src) {
    Token t = get_next_token(src);
    double result = 0;

    if (t.type == TOKEN_NUMBER) {
        result = atof(t.lexer_value);
    } else if (t.type == TOKEN_IDENTIFIER) {
        Variable* v = get_variable(t.lexer_value);
        if (v != null && !v->is_string) {
            result = v->value;
        } else {
            printf("E++ Runtime Error: Undefined variable '%s'\n", t.lexer_value);
        }
    }

    // Check for binary operations (+, -, *, /)
    Token op = get_next_token(src);
    if (op.type == TOKEN_PLUS || op.type == TOKEN_MINUS || op.type == TOKEN_MULTIPLY || op.type == TOKEN_DIVIDE) {
        Token next_t = get_next_token(src);
        double next_val = 0;
        if (next_t.type == TOKEN_NUMBER) {
            next_val = atof(next_t.lexer_value);
        } else if (next_t.type == TOKEN_IDENTIFIER) {
            Variable* v = get_variable(next_t.lexer_value);
            if (v != null && !v->is_string) next_val = v->value;
        }

        if (op.type == TOKEN_PLUS) result += next_val;
        else if (op.type == TOKEN_MINUS) result -= next_val;
        else if (op.type == TOKEN_MULTIPLY) result *= next_val;
        else if (op.type == TOKEN_DIVIDE) {
            if (next_val != 0) result /= next_val;
            else printf("E++ Error: Division by zero!\n");
        }
    } else {
        // Put back or step back if not operator (simplified for prototype)
    }

    return result;
}

// Interpreter Engine: Execute statements line by line
void interpret_eplusplus(char* code) {
    char* ptr = code;

    while (*ptr != '\0') {
        Token token = get_next_token(&ptr);

        if (token.type == TOKEN_EOF) break;

        // Print Statement: print("Hello") or print x
        if (token.type == TOKEN_PRINT) {
            Token lparen = get_next_token(&ptr);
            if (lparen.type == TOKEN_LPAREN) {
                Token val_token = get_next_token(&ptr);
                if (val_token.type == TOKEN_STRING) {
                    printf("E++ Output -> %s\n", val_token.lexer_value);
                } else if (val_token.type == TOKEN_IDENTIFIER) {
                    Variable* v = get_variable(val_token.lexer_value);
                    if (v != null) {
                        if (v->is_string) printf("E++ Output -> %s\n", v->string_value);
                        else printf("E++ Output -> %f\n", v->value);
                    } else {
                        printf("E++ Error: Variable '%s' not found\n", val_token.lexer_value);
                    }
                } else if (val_token.type == TOKEN_NUMBER) {
                    printf("E++ Output -> %s\n", val_token.lexer_value);
                }
                Token rparen = get_next_token(&ptr);
            }
        }
        // Variable Declaration: var x = 10; or var name = "Sohel";
        else if (token.type == TOKEN_VAR) {
            Token var_name = get_next_token(&ptr);
            if (var_name.type == TOKEN_IDENTIFIER) {
                char name[MAX_VAR_NAME];
                strcpy(name, var_name.lexer_value);

                Token assign = get_next_token(&ptr);
                if (assign.type == TOKEN_ASSIGN) {
                    Token val_token = get_next_token(&ptr);
                    if (val_token.type == TOKEN_STRING) {
                        set_string_variable(name, val_token.lexer_value);
                    } else if (val_token.type == TOKEN_NUMBER) {
                        set_variable(name, atof(val_token.lexer_value));
                    }
                }
            }
        }
    }
}

int main() {
    printf("========================================\n");
    printf("=== E++ Language Engine Initialized ====\n");
    printf("========================================\n");

    // E++ Sample Multi-command Script
    char eplusplus_program[] = 
        "var appName = \"Flashgram\";"
        "var version = 2.0;"
        "print(\"Welcome to E++ Programming Language!\");"
        "print(appName);"
        "print(version);";

    printf("Executing E++ Program Script...\n\n");
    interpret_eplusplus(eplusplus_program);

    printf("\n========================================\n");
    printf("Execution Completed Successfully.\n");
    printf("========================================\n");

    return 0;
}
