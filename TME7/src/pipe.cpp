#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string.h>

int main (int argc, char ** argv) {
    int pfd [2];
    pipe(pfd);

    argv[0]=0;
    int i,j;
    char* argv1[10]={0};//premiere decomposition
    char* argv2[10]={0};//deuxieme decomposition

    for(i=1;i<argc;i++){
        argv1[i-1]=argv[i]; 
        if(strcmp(argv[i],"|")==0){
            argv[i]=0;
            argv1[i-1]=0;
            break;
        }
    }

    i++;
    for(j=0;i<argc;j++,i++){
        argv2[j]=argv[i];
    }
    argv[j]=0;
    
 if((fork())==0){
        dup2(pfd[1],STDOUT_FILENO);//l'ecriture correspond au Out de ce fork la sortie de ce exec sera donc lu au bout du pipe
        
        close (pfd[1]);
        close (pfd[0]);

        execv(argv1[0],argv1);//execv pour utiliser un tableau
        return 0;
    }

    if((fork())==0){
        dup2(pfd[0],STDIN_FILENO);//lit donc la sortie du premier fils 
        //STDIN du fork2  = STDOUT du fork1 
        close (pfd[0]);
        close (pfd[1]);
        execv(argv2[0],argv2);
        return 0;
    }

    close (pfd[0]);//fermeture des pipes dans le main sinon les autres ne peuvent pas se fermer.
    close (pfd[1]);
    wait(0);//attend les deux processus
    wait(0);
    return 0;
    }
