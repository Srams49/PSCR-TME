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
            
    };
}