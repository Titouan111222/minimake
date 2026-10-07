#include "make.h"

void destroy_input(struct input *make){
    if (make!=NULL){
        if (make->f!= NULL)
            free(make->f);
        if (make->command_name!=NULL)
            free(make->command_name);
        free(make);
        }
    }

int main(int argc,char * argv[]){
    struct input *make = malloc(sizeof(struct input));
    if (make == NULL)
        return 2;
    make->p = 0;
    make->f= NULL;
    make->command_name = NULL;
    struct variable *var = NULL;
    struct command *com = NULL;
    int count = 1;
    while (argc>count){
        if (!strcmp(*(argv+count),"-h")){
            printf("Usage: minimake [OPTIONS] [TARGETS]\n");
            printf("    OPTIONS can be:\n");
            printf("        \"-h\"            to display the helper\n");
            printf("        \"-p\"            to print the rules and variables\n");
            printf("        \"-f\" <filename> to change the make config file, \"Makefile\" by default\n");
            printf("    TARGETS are target name of makefile rule to execute\n");
            destroy_input(make);
            return 0;
            }
        else if (!strcmp(*(argv+count),"-p"))
            make->p = 1;
        else if (!strcmp(*(argv+count),"-f")){
            count++;
            if (argc==count){
                destroy_input(make);
                return 2;
                }
            if (make->f!=NULL)
                free(make->f);
            make->f = strdup(*(argv+count)); 
            }
        else{
            if (make->command_name!=NULL)
                free(make->command_name);
            make->command_name = strdup(*(argv+count)); 
            }
        count++;
        }
    if (make->f==NULL)
        make->f = strdup("Makefile");
    FILE *verif = fopen(make->f,"r");
    if (verif == NULL){
        destroy_input(make);
        return 3;
        }
    fclose(verif);
    if (!parse(make->f,&com,&var)){
        free_variable(var);
        free_command(com);
        destroy_input(make);
        return 2;
        }
    struct variable *var_joli = NULL;
    struct command *com_joli = NULL;
    if (!parse_mais_plus_joli(make->f,&com_joli,&var_joli,var)){
        free_variable(var);
        free_command(com);
        free_variable(var_joli);
        free_command(com_joli);
        destroy_input(make);
        return 2;
        }
    if (make->p)
        print_make(var,com,com_joli);
    else{
        struct command *exect = com_joli;
        if (make->command_name != NULL){
            while (exect!=NULL&&strcmp(make->command_name,exect->name))
                exect = exect->next;
                }
        if (exect == NULL){
            printf("minimake: *** No targets.  Stop.\n");
            free_variable(var_joli);
            free_command(com_joli);
            free_variable(var);
            free_command(com);
            destroy_input(make);
            return 2;
            }
        if (execute(exect,com_joli,var_joli)){
            free_variable(var_joli);
            free_command(com_joli);
            free_variable(var);
            free_command(com);
            destroy_input(make);
            return 2;
            }
        }
    free_variable(var_joli);
    free_command(com_joli);
    free_variable(var);
    free_command(com);
    destroy_input(make);
    return 0;
    }

