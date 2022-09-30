#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
//ajout de cette include pour utiliser les vecteur
#include <vector>
#include "hash.h"

int main () {
	using namespace std;
	using namespace std::chrono;
	/* Q3/Q4
	//declaration du vecteur et initialisation a vide
	vector< pair<string,int> > doublon = {};
	
	int i;
	*/

	pr::HashMap<string,int> * hash = new pr::HashMap<string,int>();

	int *tmp;

	ifstream input = ifstream("./WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		/*Q3/Q4
		for( i =0; i<doublon.size(); i++){
			if ( word.compare( (doublon.at(i)).first ) == 0 ){
				(doublon.at(i)).second ++;
				break;
			}
		}
		if (i == doublon.size() ){
			
			doublon.push_back( make_pair(word,1) );
		}
		*/
		tmp = (*hash).getVal(word);

		if(tmp == nullptr){
			hash->put(word,1);
		}
		else{
			*tmp += 1;
		}
		nombre_lu++;

	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;

	string war = "war";
	
	string peace = "peace";
	
	string toto = "toto";

	int *nbwar = (*hash).getVal(war);

	int *nbpeace = (*hash).getVal(peace);

	int *nbtoto = (*hash).getVal(toto);
	/* Q3/Q4
	for( i =0; i<doublon.size(); i++){
			if ( war.compare( (doublon.at(i)).first ) == 0 ){
			
				nbwar = (doublon.ast(i)).second;
			
			}
			else if (peace.compare( (doublon.at(i)).first ) == 0){

				nbpeace = (doublon.at(i)).second;
			
			}
			else if (toto.compare( (doublon.at(i)).first ) == 0 )
			{
				nbtoto = (doublon.at(i)).second;
			}
			
		}
	*/
	cout << "Nombre d'occurrence de war " << ((nbwar == nullptr) ?  0 :*nbwar) << endl;
	cout << "Nombre d'occurrence de peace " << ((nbpeace == nullptr) ?  0 :*nbpeace )<< endl;
	cout << "Nombre d'occurrence de toto " << ((nbtoto == nullptr) ?  0 :*nbtoto) << endl;

	std::vector<std::pair<string,int>> vector = (*hash).copieVector();

	std::sort(vector.begin(), vector.end(), [] (const pair<string,int> a, const pair<string,int> b) { return a.second > b.second;});

	for (int i = 0 ; i<10;i++){
		cout << "Mot numero " <<i+1 <<" en frequence est " <<  vector.at(i).first <<" avec "<<vector.at(i).second<<" occurence" <<endl;
	}

    return 0;
}

/*
	Question 1: On trouve 566193 mot lorsque l'on excute le programe pour la premier fois

	Question 2: On trouve 20333 mot différent

	Question 3: On trouve 298 d'occurrence de war, 114 d'occurrence de peace  et 0 d'occurrence de toto 

	Question 4: La complexité de cette algoritme est affreuse. On aurait pu utiliser un array ou forward_list pour un resulta similaire, on peut aussi utiliser les map 

	Question 5: 
*/

