/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
	char *delimeters;
	char *tokenStream;
};

typedef struct TokenizerT_ TokenizerT;
int offset = 0;

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *separators, char *ts) {
	TokenizerT *result = NULL;
	if((result = malloc(sizeof *result)) != NULL) {
		(*result).delimeters = malloc(strlen(separators) * sizeof(char) + sizeof(char));
		(*result).tokenStream = malloc(strlen(ts) * sizeof(char) + sizeof(char));
		if((*result).delimeters == NULL || (*result).tokenStream == NULL) {
			free((*result).delimeters); free((*result).tokenStream);
			free(result);
			return NULL;
		} else {
			result.delimeters = strcpy(result.delimeters, separators);
			result.tokenStream = strcpy(result.tokenStream, ts);
			return &result;
		}
	}
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {
	free((*tk).delimeters);
	free((*tk).tokenStream);
}

int contains(char *str, char c) {
	int result = 0;
	int i;
	for(i = 0; str[i] != '\0'; i++) {
		if(c == str[i]) {
			result = 1;
			break;
		}
	}
	return result;
}

char *copy(char *str1, char *str2, int start, int end) {
	int i;
	for(i = 0; i < (end-start+1); i++)
		str1[i] = str2[i+start];
	return str1;
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk) {
	char *result = NULL;
	int start;
	int end = 0;
	while(contains((*tk).delimeters, (*tk).tokenStream[offset]))
		offset++;
	start = offset;
	while((*tk).tokenStream[offset] != '\0' || !(contains((*tk).delimeters, (*tk).tokenStream[offset]))) {
		offset++;
		end = offset;
	}
	if(end != 0) {
		result = malloc((end-start+1) * sizeof(char));
		if(result != NULL)
			result = copy(result, (*tk).tokenStream, start, end);
	}
	return result;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
	if(argc != 3) {
		printf("Usage: tokenizer delimeters string\n");
		return 1;
	} else {
		TokenizerT *tokenizer = TKCreate(argv[1], argv[2]);
		char *token;
		while((token = TKGetNextToken(tokenizer)) != NULL) {
			printf("%s\n", token);
			free(token);
		}
		TKDestroy(tokenizer);
		return 0;
	}
}
