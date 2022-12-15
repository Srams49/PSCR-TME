#pragma once

#include <cstring> // size_t,memset
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <sys/mman.h>
#include <mutex>
#include <iostream>

namespace pr {
using namespace std;
#define STACKSIZE 100

template<typename T>
class Stack {
	std::mutex M;
	T tab [STACKSIZE];
	size_t sz;
	sem_t* cons,*prod;
public :
	Stack () : sz(0) { 
		memset(tab,0,sizeof tab) ;
		cons=sem_open("/monsemSTACK1",O_CREAT|O_EXCL|O_RDWR,0600,0);
		prod=sem_open("/monsemSTACK2",O_CREAT|O_EXCL|O_RDWR,0600,STACKSIZE);

	}
	~Stack(){
		sem_close(cons);
		sem_close(prod);
		sem_unlink("/monsemSTACK1");
		sem_unlink("/monsemSTACK2");
	}
	T pop () {
		
		// bloquer si vide
		sem_wait(cons);
		M.lock();
		T toret = tab[--sz];

		sem_post(prod);
		M.unlock();
		return toret;
	}

	void push(T elt) {

		//bloquer si plein
		sem_wait(prod);

		M.lock();

		tab[sz++] = elt;

		sem_post(cons);

		M.unlock();

	}

	bool isEmpty(){return sz==0;}
};

}
