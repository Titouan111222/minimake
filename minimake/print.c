#include "make.h"

struct variable *what_variable(char *string, struct variable *var, size_t count){
    if (string==NULL||var==NULL)
        return NULL;
    while (var!=NULL && count!=0){
        if (!strcmp(string, var->name))
            return var;
        var = var->next;
        count--;
        }
    return NULL;
    }

char *replace(char *line, struct variable *var, size_t count){
    if (line == NULL)
        return NULL;
    size_t capacity = 64;
    char *res = malloc(capacity);
    if (res == NULL)
        return NULL;
    size_t i = 0;
    size_t j = 0;
    size_t start;
    size_t len;
    char *name = NULL;
    struct variable *repl = NULL;
    char *tmp = NULL;
    while (*(line+i)!='\0') {
        if (*(line+i)=='$'&&*(line+i+1)=='{'){
            i += 2;
            start = i;
            while (*(line+i)!='\0'&&*(line+i)!='}')
                i++;
            len = i-start;
            name = strndup(line+start,len);
            if (name== NULL){
                free(res);
                return NULL;
                }
            repl = what_variable(name,var,count);
            free(name);
            if (repl!=NULL&&repl->valeur!= NULL){
                len = strlen(repl->valeur);
                while (j+len+1>= capacity) {
                    capacity *= 2;
                    tmp = realloc(res,capacity);
                    if (tmp == NULL){
                        free(res);
                        return NULL;
                        }
                    res = tmp;
                    }
                memcpy(res+j,repl->valeur,len);
                j +=len;
                }
            if (*(line+i)=='}')
                i++;
            }
        else{
            while (j + 2 >= capacity){
                capacity *= 2;
                tmp = realloc(res, capacity);
                if (tmp == NULL){
                    free(res);
                    return NULL;
                    }
                res = tmp;
                }
            *(res+j)=*(line+i);
            j++;
            i++;
            }
        }
    *(res+j) ='\0';
    return res;
    }

char *clean(char *line, struct variable *var){
    if (line == NULL)
        return NULL;
    size_t count = 0;
    struct variable *parcour = var;
    while (parcour!=NULL){
        count++;
        parcour = parcour->next;
        }
    char *res = replace(line,var,count);
    return res;
    }

void print_make(struct variable *var,struct command *com,struct command *com_joli){
    struct variable *root_var = var;
    size_t count = 0;
    char * tmp = NULL;
    printf("# variables\n");
    while (var!=NULL){
        tmp = replace(var->valeur,root_var,count);
        printf("\'%s\' = \'%s\'\n",var->name,tmp);
        free(tmp);
        count++;
        var = var->next;
        }
    printf("\n# rules\n"); 
    while(com!=NULL){
        printf("(%s):",com_joli->name);
        for (size_t t = 0;t<com_joli->option_nbr;t++)
            printf(" [%s]",com_joli->option[t]);
        printf("\n");
        for (size_t t = 0;t<com->executable_nbr;t++)
            printf("\t\'%s\'\n",com->executable[t]);
        com = com->next;
        com_joli = com_joli->next;
        }
    }