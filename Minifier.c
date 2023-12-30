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
#include <ctype.h>
#include <math.h>

// Read whole file into memory and return char*
char* file_read(char* filepath)
{
	FILE* file = fopen(filepath, "r");
	if (file == NULL) {
		printf("Error: File not found\n");
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);

	char* buffer = (char*)malloc(sizeof(char) * (size + 1));
	if (buffer == NULL) {
		printf("Error: Memory allocation failed\n");
		exit(1);
	}

	fread(buffer, 1, size, file);
	buffer[size] = '\0';

	fclose(file);
	return(buffer);
}

// Remove inline and multi-line C-style comments
char* remove_comments(char* buffer)
{
	char* new_buffer = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	if (new_buffer == NULL) {
		printf("Error: Memory allocation failed\n");
		exit(1);
	}

	int i = 0;
	int j = 0;
	while (buffer[i] != '\0') {
		if (buffer[i] == '/' && buffer[i + 1] == '/') {
			while (buffer[i] != '\n') {
				i++;
			}
		}
		else if (buffer[i] == '/' && buffer[i + 1] == '*') {
			i += 2;
			while (buffer[i] != '*' || buffer[i + 1] != '/') {
				i++;
			}
			i += 2;
		}
		else {
			new_buffer[j] = buffer[i];
			j++;
			i++;
		}
	}
	new_buffer[j] = '\0';
	return(new_buffer);
}

// Minify C code
// Note: Add newline to `new_code` for MACRO and #include statements
char* minify_c_code(char* code)
{
	char* new_code = (char*)malloc(sizeof(char) * (strlen(code) + 1));
	if (new_code == NULL) {
		printf("Error: Memory allocation failed\n");
		exit(1);
	}

	int i = 0;
	int j = 0;

	while (code[i] != '\0') {
		// Handle identifier
		if (isalpha(code[i]) || isdigit(code[i]) || code[i] == '_') {
			char* temp = (char*)malloc(sizeof(char) * (52)); // 52 is the max length of an identifier
			int temp_i = 0;
			while (isalpha(code[i]) || isdigit(code[i]) || code[i] == '_') {
				temp[temp_i++] = code[i];
				i++;
			}
			temp[temp_i] = '\0';

			// Append temp to `new_code`
			for (int k = 0; k < temp_i; k++) {
				new_code[j] = temp[k];
				j++;
			}
			const char keywords[] = "auto|break|case|char|const|"
				"continue|default|do|double|else|"
				"enum|extern|float|for|goto|"
				"if|int|long|register|return|"
				"short|signed|sizeof|static|struct|"
				"switch|typedef|union|unsigned|void|"
				"volatile|while";
			int isKeyword = 0;
			for(int startOfWord = 0, k = 0; keywords[k]; k++) {
				if(keywords[k] != '|')
					continue;

				if(!memcmp(keywords + startOfWord , temp,k - startOfWord)) {
					isKeyword = 1;
					break;
				}
				startOfWord = k;
			}
			if(isKeyword) {
				while (code[i] == ' ' || code[i] == '\t' || code[i] == '\n' || code[i] == '\r')
					i++;
				if(code[i] != '(')
					new_code[j++] = ' ';
			} else { // Other identifier or maybe user-defined var, types, etc
					 // Skip whitespaces after the identifier
				while (code[i] == ' ' || code[i] == '\t' || code[i] == '\n' || code[i] == '\r') {
					i++;
				}
				if (isalpha(code[i]) || isdigit(code[i]) || code[i] == '_') {
					new_code[j++] = ' ';
					new_code[j++] = code[i];
					i++;
				}
			}

			// Free temp
			free(temp);
		}
		else if (code[i] == '#') {
			while (code[i] != '\n') {
				new_code[j] = code[i];
				i++;
				j++;
			}
			new_code[j] = '\n';
			j++;
		}
		else if (code[i] == ' ' || code[i] == '\n' || code[i] == '\r' || code[i] == '\t') {
			i++;
		}
		else if (code[i] == '{' || code[i] == '}') {
			new_code[j] = code[i];
			j++;
			i++;
		}
		else if (code[i] == ';') {
			new_code[j] = code[i];
			j++;
			i++;
			while (code[i] == ' ' || code[i] == '\n' || code[i] == '\t') {
				i++;
			}
		}
		else if (code[i] == '(' || code[i] == ')') {
			new_code[j] = code[i];
			j++;
			i++;
		}
		else if (code[i] == ',') {
			new_code[j] = code[i];
			j++;
			i++;
			while (code[i] == ' ' || code[i] == '\n' || code[i] == '\t') {
				i++;
			}
		}
		else {
			if (code[i] == '\'') { // Single quote char
				i++;	   			   // convert it to a number for shorter code
				if (code[i] != '\\') {
					sprintf(new_code+j, "%d", code[i]);
					j += ceil(log10(code[i+1]));
					i += 2;
				}
				else {
					int num = -1;
					i++;
					if ( code[i] == 'a')
						num = '\a';
					else if ( code[i] == 'b')
						num = '\b';
					else if ( code[i] == 'e')
						num = '\e';
					else if ( code[i] == 'f')
						num = '\f';
					else if ( code[i] == 'n')
						num = '\n';
					else if ( code[i] == 'r')
						num = '\r';
					else if ( code[i] == 't')
						num = '\t';
					else if ( code[i] == 'v')
						num = '\v';
					else if ( code[i] == '\\')
						num = '\\';
					else if ( code[i] == '\'')
						num = '\'';
					else if ( code[i] == '"')
						num = '\"';
					else if ( code[i] == '?')
						num = '\?';
					// octal/hexadecimal escapes
					else if ( code[i] >= '0' && code[i] <= '7' || code[i] == 'x' || code[i] == 'u' || code[i] == 'U' ) {
						int base = code[i] >= '0' && code[i] <= '7' ? 16 : 8;
						num = 0;
						while (code[i] != '\'') {
							num = num * base + code[i] - '0';
							i++;
						}
						i--;
					}
					if (num == -1){
						fprintf(stderr,"\e[101;37mERROR\e[m: invalid escape sequence: \\%c",code[i]);
						exit(1);
					}
					printf("\e[104;37mLOG\e[m: code+i==%s \n",code+i);
					sprintf(new_code+j, "%d", num);
					printf("\e[104;37mLOG\e[m: j==%s \n",new_code+j);
					i += 2;
					j += (num != 0 ? ceil(log10(num+1)) : 1);
				}
			}
			if (code[i] == '"') { // Write string double quotes
				new_code[j] = code[i];
				i++;
				j++;

				while (code[i] != '"') {
					new_code[j] = code[i];
					j++;
					i++;
				}
				new_code[j] = code[i];
				j++;
				i++;
			}
			else {
				new_code[j] = code[i];
				j++;
				i++;
			}
		}
	}

	new_code[j] = '\0';

	return(new_code);
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
		if (file == NULL) {
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
