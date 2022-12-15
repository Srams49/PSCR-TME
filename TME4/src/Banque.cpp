#include "Banque.h"

#include <iostream>

using namespace std;

namespace pr {

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
    //std::unique_lock<std::recursive_mutex> g(m);
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];;
    if (deb < cred){//nécissite un ordre de pour lock
        crediteur.getMutex().lock();// solution 1
        debiteur.getMutex().lock();
    }
    else{
        debiteur.getMutex().lock();
        crediteur.getMutex().lock();
    }
	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}
	crediteur.getMutex().unlock();// ne marche pas car debiter lock aussi ce mutex
    debiteur.getMutex().unlock();
}
size_t Banque::size() const {
    //std::unique_lock<std::recursive_mutex> g(m);
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) const {
	int bilan = 0;
	int id = 0;
    //std::unique_lock<std::recursive_mutex> g(m);
    for(const auto & compte : comptes){
        comptegetMutex().lock();
    }
	for (const auto & compte : comptes) {
		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		else{
			cout << "Compte " << id << " en positif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
    for(const auto & compte : comptes){
        comptegetMutex().unlock();
    }
	return bilan == attendu;
}
}