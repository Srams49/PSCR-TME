#pragma once
#include <thread>
#include <mutex>

namespace pr {

class Compte {
	mutable mutex m;
	int solde;
public :
	Compte(int solde=0):solde(solde) {}
	Compte(const Compte & other);
	void crediter (unsigned int val) ;
	bool debiter (unsigned int val) ;
	int getSolde() const  ;
};

}
