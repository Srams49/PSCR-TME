#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <forward_list>

namespace TME3{
    template<typename iterateur>
    
    size_t count (iterateur begin, iterateur end){
        size_t count = 0;
        while(end != begin){
            count++;
            begin++;
        }
        return count;
    }
    template<typename iterateur, typename T>
    size_t count_if_equal (iterateur begin, iterateur end,T &val){
        size_t count = 0;
        while(end != begin){
            if (val== *begin){
                count++;
            }
            begin++;
        }
        return count;
    }


}



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
            HashMap(size_t taille = 11001):seau_(taille),size_(0),end_(seau_.end()),vit_(seau_.begin()),lit_(vit_->begin()){}

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
                end_ = seau_.end();
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
            typename Table::iterator end_;
            typename Table::iterator vit_;
            typename std::forward_list < Entry >::iterator lit_;
            typename std::forward_list < Entry >::iterator & operator++(){
                lit_++; 
                if (*lit_==nullptr){
                    vit_++;
                    while(*vit_!=nullptr && vit_!=end_){
                        vit_++;
                    }
                    lit_=vit_->begin();
                }
            }
            //bool operator!=(const iterator &other)  
            Entry & operator*(){
                if (*lit_ == nullptr){
                   ++; 
                }
                return *lit_;
            }
            
    };
}
