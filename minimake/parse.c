#include "make.h"

//---------------------------------------------verifi le makefile------------------
int is_variable(char * line){
    if (line == NULL)
        return 0;
    size_t ind = 0;
    while (*(line+ind)==' ')
        ind++;
    if (*(line+ind)=='=')
        return 2;
    while (*(line+ind)!='\n'&&*(line+ind)!=' '&&*(line+ind)!='='&&*(line+ind)!=':'&&*(line+ind)!='#'&&*(line+ind)!='\r'&&*(line+ind) != '\0')
        ind++;
    while (*(line+ind)==' ')
        ind++;
    if (*(line+ind)!='=')
        return 0;
    ind++;
    while (*(line+ind)==' ')
        ind++;
    if (*(line+ind)=='#'||*(line+ind)=='\n'||*(line+ind)=='\r'||*(line+ind) == '\0')
        return 2;
    return 1;
    }

int is_command(char *line){
    if (line == NULL)
        return 0;
    size_t ind = 0;
    if (*(line+ind)=='\n'||*(line+ind)=='\r'||*(line+ind) == '\0'||*(line+ind) == '#')
        return 0;
    if (*(line+ind)==':')
        return 0;
    while (*(line+ind)!='\n'&&*(line+ind)!=':'&&*(line+ind)!='='&&*(line+ind)!=' '&&*(line+ind)!='#'&&*(line+ind)!='\r'&&*(line+ind) != '\0')
        ind++;
    if (*(line+ind)!=':')
        return 0;
    ind++;
    while (*(line+ind)!='\n'&&*(line+ind)!=':'&&*(line+ind)!='='&&*(line+ind)!='#'&&*(line+ind)!='\r'&&*(line+ind) != '\0')
        ind++;
    if (*(line+ind)!='\n'&&*(line+ind)!='#'&&*(line+ind)!='\r'&&*(line+ind) != '\0')
        return 0;
    return 1;
    }

int is_executable(char *line){
    if (line == NULL)
        return 0;
    size_t ind = 0;
    if (*(line+ind)=='\t'){
        ind++;
        while (*(line+ind)==' ')
            ind++;
        if (*(line+ind)=='\n'||*(line+ind)=='#'||*(line+ind)=='\r'||*(line+ind) == '\0')
            return 0;
        return 1;
        }
    if (is_rien(line))
        return 2;
    return 0;
    }

int is_rien(char * line){
    if (line == NULL)
        return 0;
    size_t ind = 0;
    while (*(line+ind)==' ')
        ind++;
    if (*(line+ind) == '\n'||*(line+ind) == '\0'||*(line+ind)=='\r'||*(line+ind) == '#')
        return 1;
    return 0;
    }

int all_good(char *file){
    FILE *make = fopen(file, "r");
    if (make == NULL)
        return 3;
    char *line = NULL;
    size_t n = 0;
    int correct = 0;
    int var;
    while (getline(&line, &n, make) != -1){
        var = is_variable(line);
        if (var != 1){
            if (var == 2){
                free(line);
                fclose(make);
                return 0;
                }
            if (is_command(line))
                correct = 1;
            else if (is_executable(line)== 1){
                if (!correct){
                    free(line);
                    fclose(make);
                    return 0;
                    }
                }
            else if (is_rien(line))
                correct = 0;
            else{
                free(line);
                fclose(make);
                return 0;
                }
            }
        else 
            correct = 0;
        }
    free(line);
    fclose(make);
    return 1;
    }

//---------------------------------------------verifi le makefile------------------

//---------------------------------------------free------------------
void free_variable(struct variable * root){
    if (root!=NULL){
        struct variable *parcour = root->next;
        while (parcour!=NULL){
            free(root->name);
            free(root->valeur);
            free(root);
            root = parcour;
            parcour = parcour->next;
            }
        free(root->name);
        free(root->valeur);
        free(root);
        }
    }

void free_command(struct command * root){
    if (root!=NULL){
        struct command *parcour = root->next;
        while (parcour!=NULL){
            free(root->name);
            for (size_t t = 0;t<root->option_nbr;t++)
                free(root->option[t]);
            free(root->option);
            if (root->executable!=NULL){
                for (size_t i = 0;i<root->executable_nbr;i++)
                    free(root->executable[i]);
                free(root->executable);
                }
            free(root);
            root = parcour;
            parcour = parcour->next;
            }
        free(root->name);
        for (size_t t = 0;t<root->option_nbr;t++)
            free(root->option[t]);
        free(root->option);
        if (root->executable!=NULL){
                for (size_t i = 0;i<root->executable_nbr;i++)
                    free(root->executable[i]);
                free(root->executable);
                }
        free(root);
        }
    }
//---------------------------------------------free------------------

//---------------------------------------------sparse le makefile------------------
char **parse_in_chars(char * line,size_t ind,size_t * nbr){
    char ** new = NULL;
    size_t size;
    while (*(line+ind)!='\n'&&*(line+ind)!='\r'&&*(line+ind)!='#'&&*(line+ind)!='\0'){
        size = 0;
        while (*(line+ind)==' ')
            ind++;
        while (*(line+ind+size)!='\n'&&*(line+ind+size)!='\0'&&*(line+ind+size)!='\r'&&*(line+ind+size)!=' '&&*(line+ind+size)!='#')
            size++;
        if (size!=0){
            (*nbr)++;
            new = realloc(new,(*nbr)*sizeof(char *));
            new[(*nbr)-1] = malloc((size+1)*sizeof(char));
            new[(*nbr)-1][size] = '\0';
            for (size_t  i = 0;i<size;i++){
                new[(*nbr)-1][i] = *(line+ind+i);
                }
            }
        ind += size;
        }
    return new;
    }

int parse_execut(struct command * root,char * line){
    if (root == NULL|| line == NULL ||is_executable(line)!=1)
        return 0;
    struct command * parcour = root;
    size_t ind = 0;
    size_t size = 0;
    while (parcour->next!=NULL)
        parcour = parcour->next;
    parcour->executable = realloc(parcour->executable,(parcour->executable_nbr + 1) * sizeof(char *));
    while (*(line+ind)=='\t'||*(line+ind)==' ')
        ind++;
    while (*(line+ind+size)!='\n'&&*(line+ind+size)!='\r'&&*(line+ind+size)!='\0'&&*(line+ind+size)!='#')
        size++;
    parcour->executable[parcour->executable_nbr] = malloc((size+1)*sizeof(char));
    strncpy(parcour->executable[parcour->executable_nbr],(line+ind),size);
    parcour->executable[parcour->executable_nbr][size] = '\0';
    parcour->executable_nbr+=1;
    return 1;
    }

struct command * parse_command(struct command * root,char * line){
    if (line == NULL)
        return root;
    struct command *new = malloc(sizeof(struct command));
    if (new == NULL)
        return root;
    size_t size = 0;
    size_t ind = 0;
    while (*(line+ind)==' ')
        ind++;
    while (*(line+ind+size)!=':')
        size++;
    new->size_name = size +1;
    new->name = malloc(new->size_name*sizeof(char));
    for (size_t i = 0;i<size;i++)
        new->name[i] = *(line+i+ind);
    new->name[size] = '\0';
    ind+=size+1;
    new->option_nbr= 0;
    new->option = parse_in_chars(line,ind,&(new->option_nbr));
    new->executable_nbr = 0;
    new->executable = NULL;
    new->next = NULL;
    if (root == NULL)
        return new;
    struct command *parcour = root;
    while (parcour->next != NULL)
        parcour = parcour->next;
    parcour->next = new;
    return root;
    }

struct variable * parse_variable(struct variable * root,char * line){
    if (line == NULL)
        return root;
    struct variable *new = malloc(sizeof(struct variable));
    if (new == NULL)
        return root;
    size_t size = 0;
    size_t ind = 0;
    size_t sizetwo = 0;
    while (*(line+ind)==' ')
        ind++;
    while (*(line+ind+size)!=' '&&*(line+ind+size)!='=')
        size++;
    new->size_name = size +1;
    new->name = malloc(new->size_name*sizeof(char));
    for (size_t i = 0;i<size;i++)
        new->name[i] = *(line+i+ind);
    new->name[size] = '\0';
    ind+=size;
    while (*(line+ind)==' ')
        ind++;
    ind++;
    while (*(line+ind)==' ')
        ind++;
    while (*(line+ind+sizetwo)!='\n'&&*(line+ind+sizetwo)!='#'&&*(line+ind+sizetwo)!='\0'&&*(line+ind+sizetwo)!='\r')
        sizetwo++;
    new->size_valeur = sizetwo +1;
    new->valeur = malloc(new->size_valeur*sizeof(char));
    for (size_t i = 0;i<sizetwo;i++)
        new->valeur[i] = *(line+i+ind);
    new->valeur[sizetwo] = '\0';
    new->next = NULL;
    if (root == NULL)
        return new;
    struct variable *parcour = root;
    while (parcour->next != NULL)
        parcour = parcour->next;
    parcour->next = new;
    return root;
    }

int parse(char * file,struct command ** com,struct variable ** var){
    if (!all_good(file))
        return 0;
    FILE * make = fopen(file,"r");
    size_t n = 0;
    char *lineptr = NULL;
    char *lineptrtwo= NULL;
    int correct = 0;
    int correct2 = 1;
    while (lineptrtwo!=NULL||getline(&lineptr,&n,make)!=-1){
        if (lineptrtwo!=NULL){
            free(lineptr);
            lineptr = lineptrtwo;
            lineptrtwo = NULL;
            }
        correct = is_variable(lineptr);
        if (correct == 0){
            correct = is_command(lineptr);
            if (correct == 1){
                *com = parse_command(*com,lineptr);
                while(correct2&&getline(&lineptrtwo,&n,make)!=-1)
                    correct2 = parse_execut(*com,lineptrtwo);
                correct2 = 1;
                }
            }
        else
            *var = parse_variable(*var,lineptr);
        }
    if (lineptrtwo!=NULL)
        free(lineptrtwo);
    free(lineptr);
    fclose(make);
    return 1;
    }


int parse_mais_plus_joli(char *file,struct command **com,struct variable **var,struct variable *wash){
    FILE *make = fopen(file, "r");
    char *lineptr = NULL;
    size_t n = 0;
    char *line = NULL;
    while (getline(&lineptr, &n, make) != -1) {
        line = clean(lineptr, wash);
        if (line== NULL){
            free(lineptr);
            fclose(make);
            return 0;
            }
        if (is_variable(line))
            *var = parse_variable(*var,line);
        else if (is_command(line))
            *com = parse_command(*com,line);
        else if (is_executable(line)==1)
            parse_execut(*com, line);
        free(line);
        }
    free(lineptr);
    fclose(make);
    return 1;
    }
//---------------------------------------------sparse le makefile------------------