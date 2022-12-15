#pragma once
#include <mutex>
#include <condition_variable>

namespace pr{
    class Barrier{
            
        private:    
            int nbjob;
            int compteur;
            mutable std::mutex m;
            std::condition_variable cond;
        public:

            Barrier(int n): nbjob(n), compteur(0){}

            void done(){
                std::unique_lock<std::mutex> lg(m);
                ++compteur;
                if(compteur == nbjob){
                    cond.notify_all();
                }
            };

            void waitFor(){
                 std::unique_lock<std::mutex> lg(m);
                while(compteur < nbjob){

                    cond.wait(lg);
                }
            };
    };

}