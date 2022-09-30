#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <forward_list>

namespace pr {
    
    template<typename Clef, typename Valeur>
    

    class HashMap{
        public:
            struct Entry
            {
                const Clef key;
                Valeur val;

                Entry(const Clef &key, const Valeur &val):key(key),val(val){}
            };

        private:
            typedef std::vector<std::forward_list < Entry > > Table;
            Table seau_;
            size_t size_;

        public:
            HashMap(size_t taille = 11001):seau_(taille),size_(taille){}

            Valeur* getVal(const Clef &key){
                size_t couper = std::hash<Clef>()(key);
                size_t modul = couper%seau_.size();
                for(Entry &e:seau_[modul]){
                    if(e.key == key){
                        return &e.val;
                    }
                }
                return nullptr;
            }

            bool put(const Clef &key, const Valeur &val){
                size_t couper = std::hash<Clef>()(key);
                size_t modul = couper%seau_.size();
                for(Entry &e:seau_[modul]){
                    if(e.key == key){
                        e.val = val;
                        return true;
                    }
                }
                size_ ++;
                seau_[modul].emplace_front(key,val);
                return false;
            }

            size_t getSize() const{
                return size_;
            }

            std::vector<std::pair<Clef,Valeur>> copieVector(){
                std::vector<std::pair<Clef,Valeur>> v ;
                v.reserve(getSize()); 
                for (typename Table::iterator p = seau_.begin(); p != seau_.end(); p++ ){// Autre syntaxe for (auto(list:seau)){
                    //for (const auto & e : *p)
                    for(auto e = p->begin(); e != p->end(); e++){// for (auto(Entry:list)){
                        v.push_back(std::pair<Clef,Valeur>(e->key,e->val));//p devient Entry
                    }
                }
                return v;
            }
            
    };
}