#include <stdio.h>
#include <ctype.h>
#include "hash.h"
#define ERROR_MESSAGE "Failed to read the inverted-index into memory\nAborting\n"

/*
 * Gets the number of words in the inverted-index file.
 */
unsigned int getIndexSize(FILE *file) {
    char buffer[300];
    unsigned int size = 0;
    
    while(fscanf(file, "%s", buffer) == 1) { /*goes through each string in the file*/
        if(strcmp(buffer, "<list>") == 0) { /*counts the number of distinct words*/
            while(fscanf(file, "%s", buffer) == 1) {        
                if(strcmp(buffer, "</list>") == 0) {
                    size++;
                    break;
                }
            }
        }
    }
     
    rewind(file); /*puts pointer in the file to point to the very beginning*/
    return size*2; /*size is twice the number of words to reduce hash collisions*/
}

/*
 * Converts all the characters in the string into lowercase
 * characters.
 */
void toLowerCase(char *string) {
    int i;
    for(i = 0; string[i] != '\0'; i++)
        string[i] = tolower(string[i]);
}

/*
 * Reads the entries in the inverted-index file and stores
 * them into memory in the form of a hash table.
 * Returns 1 if the function succeeds, 0 otherwise.
 */
int readFile(FILE *file, HashTablePtr table) {
    WordListPtr temp;
    char buffer[300];
    char *word;
    while(fscanf(file,"%s",buffer) == 1){	/*Goes through the file while there are still words*/
       if(strcmp(buffer,"<list>") == 0){	/*If we have reached a new word in the list of words*/
            if(fscanf(file,"%s",buffer) == 1){	/*Then simply obtain the next word if there is one*/
                word = malloc(sizeof(char) * (strlen(buffer)+1));	/*malloc space for the word holder*/
                word = strcpy(word,buffer);		/*Copy the word that we have obtained from the file into the variable*/	
                temp = tableInsert(table,word);	/*Insert it into the hash table*/
                if(temp != NULL) {
                    temp->files = makeFileList();	/*At that point in the table create a new filelist*/
                    if(temp->files != NULL) {	
                        while(fscanf(file,"%s",buffer) == 1){	/*Continue obtaining file names*/
                            if(strcmp(buffer,"</list>") == 0){	/*No more file name exit*/
                                break;
                            }
                            word = malloc(sizeof(char) * (strlen(buffer)+1));	/*malloc space for the file name*/
                            word = strcpy(word,buffer);		/*Copy the word into the variable*/
                            fscanf(file, "%s", buffer);		/*obtain the next word which is the count*/
                            int count = atoi(buffer);		/*convert the count to a number*/
                            if(!FLInsert(temp->files,word,count))	/*Insert it into the count for the file*/
                                return 0;
                        }
                    } else
                        return 0;
                } else
                    return 0;
            }
        }
    }
    return 1;
}

/*
 * Gets all the file names in an array of FileList pointers and stores them 
 * in an array of strings. The file name is stored into the array by emulating
 * XOR to prevent duplication. Returns the resulting array.
 */
char** UNION(char **result, int *result_size, FileListPtr *files, int size) {
    FileListPtr temp = files[0];
    while(temp != NULL) { /*gets all the file names in the first FileListPtr and stores them into the result array*/
        if(*result_size == 0)
            result = malloc(sizeof(char *));
        else
            result = realloc(result, sizeof(char *)*((*result_size)+1));
        result[*result_size] = temp->file_name;
        (*result_size)++;
        temp = temp->next;
    }
    int i;
    for(i = 1; i < size; i++) { /*gets the file names from the rest of the FileList pointers and stores them into the result array*/
        temp = files[i];
        while(temp != NULL) {
            int j;
            for(j = 0; j < *result_size; j++) { /*checks if a file name is already in the result*/
               if(strcmp(result[j], temp->file_name) == 0)
                   break;
            }
            if(j == *result_size) { /*add the file name into the result if it's not in the result*/
                result = realloc(result, sizeof(char *)*((*result_size)+1));
                result[*result_size] = temp->file_name;
                (*result_size)++;
            }
            temp = temp->next;
        }
    }
    return result;
}

/*
 * Checks if a file name in union_files is in all of the file lists. If it is,
 * then that file name will be stored in the result array. This emulates a logical AND.
 * Returns the size of the result array.
 */
char** AND(char **union_files, int union_size, FileListPtr *files, int files_size, char **result, int *result_size) {
    int i;
    for(i = 0; i < union_size; i++) { /*checks all of the file names in union_files*/
        int count = 0; /*the number of times a file name occurs in a list of files*/
        int j;
        for(j = 0; j < files_size; j++) { /*for all the file lists in the files array*/
            FileListPtr temp = files[j];
            while(temp != NULL) { /*goes through a file list*/
                if(strcmp(union_files[i], temp->file_name) == 0)
                    count++; /*if a file name occurred in a file list, increment count*/
                temp = temp->next;
            }
        }
        if(count == files_size) { /*if a file name occurred in all of the file lists*/
            if(*result_size == 0)
                result = malloc(sizeof(char *));
            else
                result = realloc(result, sizeof(char *)*((*result_size)+1));
            result[*result_size] = union_files[i]; /*store into the result*/
            (*result_size)++;
        }
    }
    return result;
}

/*
 * Prints all the file names in an array of strings.
 */
void print(char **files, int size) {
    int i;
    if(size == 1)
        printf("%d file was found: %s\n", size, files[0]);
    else {
        printf("%d files were found: %s,", size, files[0]);
        for(i = 1; i < size-1; i++)
            printf(" %s,", files[i]);
        printf(" and %s\n", files[i]);
    }
}

int main(int argc, char *argv[]) {
    if(argc != 2) { /*Only 2 arguments are allowed*/
        printf("Usage: ./search <inverted-index file name>\nAborting\n");
        return 1;
    }

    FILE *index_file = fopen(argv[1], "r");
    if(index_file == NULL) { /*checks if the inverted-index file exists*/
        printf("Failed to read file %s or this file does not exist\nAborting\n", argv[1]);
        return 1;
    }
    
    HashTablePtr hash_table = makeHashTable(getIndexSize(index_file));
    if(hash_table == NULL) { /*if it fails to malloc memory for the hash table*/
        printf(ERROR_MESSAGE);
        fclose(index_file);
        return 1;
    }
    
    if(!readFile(index_file, hash_table)) { /*tries to read the file*/
        printf(ERROR_MESSAGE); /*if it fails, print the error message and destroy the hash table*/
        destroyTable(hash_table);
        fclose(index_file);
        return 1;
    }

    while(1) { /*keeps looping until user quits*/
        printf("Enter sa <term/s>... to get files that contain all the term/s\n");
        printf("Enter so <term/s>... to get files that contain 1 or more of the term/s\n");
        printf("Enter q to quit\n> ");
        
        char buffer[300]; /*holds user input*/
        fgets(buffer, sizeof(buffer), stdin); /*puts user input into buffer*/
        buffer[strlen(buffer)-1] = '\0'; /*removes new line character (\n)*/
        toLowerCase(buffer);
        
        if(strcmp(buffer,"quit") == 0 || strcmp(buffer,"q") == 0) {
            destroyTable(hash_table); /*if user quits, free allocated memory*/
            fclose(index_file);
            printf("Program exited\n");
            return 0; /*breaks out of while loop and returns successful exit to main*/
        }

        FileListPtr *file_list = NULL; /*an array of FileList pointers*/
        int size = 0; /*size of the file_list array*/
        char **result = NULL; /*an array of the resulting file names*/
        int result_size = 0; /*the size of the result array*/
        char *token = strtok(buffer, " ");
        if(strcmp(token, "sa") == 0) { /*if the user wants all the files that contain all the terms*/
            while((token = strtok(NULL, " ")) != NULL) { /*goes through all the terms*/
                WordListPtr temp = tableGet(hash_table, token);
                if(temp == NULL) { /*if a term is not in any file, then no files contain all the terms*/
                    if(file_list != NULL) {
                        free(file_list);
                        file_list = NULL;
                    }
                    break;
                }
                if(size == 0)
                    file_list = malloc(sizeof(FileListPtr));
                else
                    file_list = realloc(file_list, sizeof(FileListPtr)*(size+1));
                file_list[size] = temp->files; /*puts a linked list of files into the file_list array*/
                size++;
            }
            if(file_list != NULL) { /*if files were found*/
                int files_size = 0;
                char **union_files = UNION(union_files, &files_size, file_list, size); /*puts all the unique file names into the union_files array*/
                result = AND(union_files, files_size, file_list, size, result, &result_size); /*puts all the file names that are in all the linked list of the file_list array into the result array*/ 
                free(file_list);
                if(union_files != NULL)
                    free(union_files);
            }
        } else if(strcmp(token, "so") == 0) { /*if the user wants all the files that contain one or more of the terms*/
            while((token = strtok(NULL, " ")) != NULL) {
                WordListPtr temp = tableGet(hash_table, token);
                if(temp != NULL) { /*if there are files that contain a term*/
                    if(size == 0)
                        file_list = malloc(sizeof(FileListPtr));
                    else
                        file_list = realloc(file_list, sizeof(FileListPtr)*(size+1));
                    file_list[size] = temp->files; /*puts a linked list of files into the file_list array*/
                    size++;
                }
            }
            if(file_list != NULL) { /*if files were found*/
                result = UNION(result, &result_size, file_list, size); /*puts all the unique file names into the result array*/
                free(file_list);
            }
        } else
            printf("Invalid input\n");
        if(result == NULL) /*checks if there were files found*/
            printf("No files were found\n");
        else {
            char x;
            print(result, result_size); /*prints the file names*/
            free(result);
            printf("Press ENTER to continue...");
            scanf("%c", &x);
        }
    }
}
