/*
 * Name: C Minifier
 * Description: A C program to remove comments and minimize the code (remove whitespace and summarize code)
 * Date: 2022-02-11
 * Author: Max Base
 * Repository: https://github.com/BaseMax/C-Minifier
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Read whole file into memory and return char*
char* file_read(char* filepath)
{
    FILE* file = fopen(filepath, "r");
    if (file == NULL)
    {
        printf("Error: File not found\n");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(sizeof(char) * (size + 1));
    if (buffer == NULL)
    {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);
    return buffer;
}

// Remove inline and multi-line C-style comments
char* remove_comments(char* buffer)
{
    char* new_buffer = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    if (new_buffer == NULL)
    {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }

    int i = 0;
    int j = 0;
    while (buffer[i] != '\0')
    {
        if (buffer[i] == '/' && buffer[i + 1] == '/')
        {
            while (buffer[i] != '\n')
            {
                i++;
            }
        }
        else if (buffer[i] == '/' && buffer[i + 1] == '*')
        {
            i += 2;
            while (buffer[i] != '*' || buffer[i + 1] != '/')
            {
                i++;
            }
            i += 2;
        }
        else
        {
            new_buffer[j] = buffer[i];
            j++;
            i++;
        }
    }
    new_buffer[j] = '\0';
    return new_buffer;
}

// Minify C code
char* minify_c_code(char* code)
{
    char* new_code = (char*)malloc(sizeof(char) * (strlen(code) + 1));
    if (new_code == NULL)
    {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }

    return new_code;
}

// Start point
int main(int argc, char** argv)
{
    if(argc == 1) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    char* buffer = file_read(argv[1]);
    buffer = remove_comments(buffer);
    buffer = minify_c_code(buffer);

    if(argc == 3) {
        FILE* file = fopen(argv[2], "w");
        if (file == NULL)
        {
            printf("Error: Cannot create '%s' file!\n", argv[2]);
            exit(1);
        }
        fprintf(file, "%s", buffer);
        fclose(file);
    }
    else {
        printf("%s", buffer);
    }

    return 0;
}
