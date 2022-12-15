#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "rsleep.h"

int vie = 3;

void decrement(int signal){
    vie --;
    if(!vie ){std::cout<<"Je me meurt, moi "<<getpid()<<std::endl;std::cout<<"Oh non, je me meurt, moi "<<getpid()<<std::endl;
        exit(1);}
    else{
        std::cout<<"Il me reste "<<vie<<" moi "<<getpid()<<std::endl;return ;
    }
    return ;
}

void meurt(int signal){
    std::cout<<"J'ai tuer mon fil seul la mort peut me delivre de ce tourment "<<std::endl;
    exit(0);
}

void attaque(pid_t adversaire){
    std::cout<<"j'attaque "<<getpid()<<std::endl;

    sigset_t sig_proc;
    struct sigaction action;

    sigemptyset(&sig_proc);

    action.sa_mask=sig_proc;

    action.sa_flags=0;
    
    action.sa_handler = decrement;
    
    sigaction(SIGINT, &action,NULL);

    if(-1 == kill(adversaire, SIGINT)){
        std::cout<<"Echec et mat "<<adversaire<<std::endl;
        exit(0);
    };
    
    randsleep();
}

void defense(){
    std::cout<<"je defend "<<getpid()<<std::endl;
    sigset_t sig_proc;
    struct sigaction action;

    sigemptyset(&sig_proc);

    action.sa_mask=sig_proc;

    action.sa_flags=0;
    
    action.sa_handler = SIG_IGN;

    sigaction(SIGINT, &action,NULL);

    randsleep();
}

void combat(pid_t adversaire){
    std::cout<<"Pare et pret "<<getpid()<<std::endl;
    randsleep();

    while(true){
        attaque(adversaire);
        defense();
    }
}

int main(){

    sigset_t sig_proc;

    struct sigaction action;

    sigemptyset(&sig_proc);

    action.sa_mask=sig_proc;

    action.sa_flags=0;
    
    action.sa_handler = SIG_IGN;

    sigaction(SIGINT, &action,NULL);


    sigset_t sig_proc_;
    struct sigaction action_;

    sigemptyset(&sig_proc_);

    action_.sa_mask=sig_proc_;

    action_.sa_flags=0;
    
    action_.sa_handler = meurt;

    sigaction(SIGCHLD, &action_,NULL);
    
    pid_t papa = getpid();
    pid_t filton;

    filton = fork();

    srand(time(NULL));
    
    if( ! filton ){
        combat(papa);    
    }
    else{
        combat(filton);
    }
    return 1;
}
