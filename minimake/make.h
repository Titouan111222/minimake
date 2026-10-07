#ifndef MAKE_H
#define MAKE_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

struct input {
    char * f;
    char * command_name;
    int p;
    };

struct variable {
    char * name;
    char * valeur;
    size_t size_name;
    size_t size_valeur;
    struct variable * next;
    };

struct command {
    char * name;
    char ** option;
    char ** executable;
    size_t size_name;
    size_t option_nbr;
    size_t executable_nbr;
    struct command * next;
    };

//---parse---
//verif-
int is_variable(char * line);
int is_command(char *line);
int is_executable(char *line);
int is_rien(char * line);
int all_good(char * file);
//verif
//free-
void free_variable(struct variable * root);
void free_command(struct command * root);
//free
//create-
char **parse_in_chars(char * line,size_t ind,size_t * nbr);
int parse_execut(struct command * root,char * line);
struct command * parse_command(struct command * root,char * line);
struct variable * parse_variable(struct variable * root,char * line);
int parse(char * file,struct command ** com,struct variable ** var); //main_function
int parse_mais_plus_joli(char * file,struct command ** com,struct variable ** var,struct variable *wash);//second_main_function
//create
//---parse---
//---execute---
struct command *what_command(char *string,struct command *com);
int execute(struct command *exect, struct command *root,struct variable *var); //main_function
//---execute---
//---print---
struct variable *what_variable(char *string,struct variable *var,size_t count);
char *replace(char *line, struct variable *var,size_t count);
char *clean(char *line, struct variable *var);
void print_make(struct variable *var,struct command *com,struct command *com_joli);//main_function
//---print---
//---make---=== main prog
void destroy_input(struct input *make);
//---make---
#endif