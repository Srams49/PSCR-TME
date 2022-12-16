
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include "rsleep.h"
#include <string>
#include <signal.h>
#define SLEEP 5

using namespace std;

    int onsecasseoupas;
    
    void achtung(int sig){
        onsecasseoupas = 0;
    }

    void sohn(int sig){
        onsecasseoupas = 1;
    }

    int wait_till_pid(pid_t pid, unsigned int nb_sec) {

        std::cout<<"on attend "<<pid<<std::endl;
                
        pid_t p = getpid();
        
        sigset_t signal_alarm;
        struct sigaction alarme;

        sigset_t signal_child;
        struct sigaction child;

        sigset_t signal_suspendu;


        sigemptyset(&signal_alarm);
        alarme.sa_mask=signal_alarm;
        alarme.sa_flags=0;
        alarme.sa_handler = achtung;

        sigaction(SIGALRM,&alarme,NULL);

        sigemptyset(&signal_child);
        child.sa_mask=signal_child;
        child.sa_flags=0;
        child.sa_handler = sohn;

        sigaction(SIGCHLD,&child,NULL);

        alarm(nb_sec);
            
        
        sigfillset(&signal_suspendu);
        sigdelset(&signal_suspendu,SIGCHLD);
        sigdelset(&signal_suspendu,SIGALRM);

        while( p != pid){
            sigsuspend(&signal_suspendu);
            if( onsecasseoupas == 0){
                
                std::cout<<"on en alarm"<<std::endl;
                return 0;
            }
            else{
                p = wait(NULL);
            }
        }

        std::cout<<"on en wait de "<<p<<std::endl;
        return (int) p;
    }


    int main(){
        pid_t i;
        std::cout<<"on en wait de "<<getpid<<std::endl;
        if(fork()){
            return 1;
        }
        else {
            if (! (i = fork())){
                sleep(SLEEP);
                return 1;
            }
            else {
                wait_till_pid(i,SLEEP-4);
                wait_till_pid(i,SLEEP+6);
                return 1;
            }
        }

    }
