#include "Banque.h"
#include <stdlib.h> 
#include <iostream>

using namespace std;

const int NB_THREAD = 10;

void exo1_q1(pr::Banque& bank){
	int j,i,m,sleepy;
	for (int w =0;w<1000;w++){
		i = rand()%bank.size();
		j = rand()%bank.size();
		m = rand()%101 -1;
		sleepy = rand()%21;
		bank.transfert(i,j,m);
		std::this_thread::sleep_for (std::chrono::milliseconds(sleepy));
	}
}

void exo1_q4(pr::Banque& bank){
	int j,i,m,sleepy;
	for (int w =0;w<1000;w++){
		i = rand()%bank.size();
		j = rand()%bank.size();
		m = rand()%101 -1;
		sleepy = rand()%21;
		bank.transfert(i,j,m);
		std::this_thread::sleep_for (std::chrono::milliseconds(sleepy));
	}
}

int main () {
	vector<thread> threads;

	pr::Banque banque_de_France(5,10);
	
    banque_de_France.comptabiliser(50);
	
    for(int i=0; i<NB_THREAD;i++){
		threads.push_back(thread(exo1_q1, std::ref(banque_de_France)));
	}


	for (auto & t : threads) {
		t.join();
	}

	cout<<endl;
	
    banque_de_France.comptabiliser(50);
	// TODO : tester solde = NB_THREAD * JP
	return 0;
}
/*
Question 2: le lock est suffisant car le mutex est placer lors du test ainsi il est impossible que 2 test sois fait en même temps.
*/