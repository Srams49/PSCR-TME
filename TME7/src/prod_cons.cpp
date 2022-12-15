#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>



using namespace std;
using namespace pr;

void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c)) {
		stack->push(c);
	}
}

void consomateur (Stack<char> * stack) {
	while (true) {
		char c = stack->pop();
		cout << c << flush ;
		if(stack->isEmpty())
			std::cout<<"\n";
	}
}

	Stack<char> * s;

void finito(int sig){
	s->~Stack();
	std::cout<<"C'est FINITO"<<std::endl;
	exit(69);
}
	
	


int main () {
	    struct sigaction st;
        sigset_t sig_proc;
        sigemptyset(&sig_proc);
        st.sa_mask=sig_proc;
        st.sa_flags=0;
        st.sa_handler = finito;
		sigaction(SIGINT,&st,NULL);

		void * LaMap;
		LaMap=mmap(NULL,sizeof(Stack<char>),PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_SHARED,-1,0);
		s= new (LaMap)Stack<char>();
	//sem_t* sem1 =sem_open("/monsem1",O_CREAT|O_EXCL|O_RDWR,0600,0);
	//sem_t* sem2 =sem_open("/monsem2",O_CREAT|O_EXCL|O_RDWR,0600,1);

	pid_t pp = fork();
	if (pp==0) {
		producteur(s);
		return 0;
		/*for(int i=0;i<10;i++){
			sem_wait(sem1);
			cout<<"Pong"<<endl;
			sem_post(sem2);
		}
			if(sem_close(sem1)==0)cout<<"BIEN CLOSE SEM1 FORK1"<<endl;
			if(sem_close(sem2)==0)cout<<"BIEN CLOSE SEM2 FORK1"<<endl;*/
		//return 0;
	}

	pid_t pc = fork();
	if (pc==0) {
		consomateur(s);
		return 0;
		/*for(int i=0;i<10;i++){
			sem_wait(sem2);
			cout<<"Ping"<<endl;
			sem_post(sem1);
		}
			if(sem_close(sem1)==0)cout<<"BIEN CLOSE SEM1 FORK2"<<endl;
			if(sem_close(sem2)==0)cout<<"BIEN CLOSE SEM2 FORK2"<<endl;*/
		//return 0;
	}
	//if(sem_close(sem1)==0)cout<<"BIEN CLOSE SEM1 Main"<<endl;
	//if(sem_close(sem2)==0)cout<<"BIEN CLOSE SEM2 Main"<<endl;

	wait(0);
	wait(0);
	//sem_unlink("/monsem1");
	//sem_unlink("/monsem2");

	s->~Stack();
	return 0;
}
