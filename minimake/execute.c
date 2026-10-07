#include "make.h"

struct command *what_command(char *string,struct command *com){
    if (string == NULL || com == NULL)
        return NULL;
    while (com!=NULL&&strcmp(string,com->name))
        com = com->next; 
    return com;
    }

int execute(struct command *exect, struct command *root,struct variable *var){
    if (exect == NULL){
        printf("minimake: *** No targets.  Stop.\n");
        return 2;
        }
    struct command *option = NULL;
    for (size_t t = 0; t<exect->option_nbr;t++){
        option = what_command(exect->option[t],root);
        if (option!=NULL){
            if (execute(option,root,var))
                return 2;
        	}
        else{
            FILE *f = fopen(exect->option[t], "r");
            if (f==NULL){
                printf("minimake: *** No rule to make target '%s', needed by '%s'.  Stop.\n",exect->option[t],exect->name);
                return 2;
                }
            fclose(f);
        	}
    	}
    for (size_t i = 0;i<exect->executable_nbr;i++){
		printf("%s\n",exect->executable[i]);
		fflush(stdout);
        pid_t pid = fork();
        if (pid<0){
            return 2;
        	}
        if (pid== 0){
    		execl("/bin/sh","sh","-c",exect->executable[i],NULL);
    		exit(1);
			}
        int status;
        waitpid(pid, &status,0);
        if (!WIFEXITED(status)||WEXITSTATUS(status) != 0)
            return 2;
    	}
    return 0;
	}
	