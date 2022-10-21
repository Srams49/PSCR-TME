#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>

namespace pr {
void poolWorker(Queue<Job>& queue ){
		while(true){
			Job*j=queue.pop();
			if(j==nullptr){break;}
			j->run();
			delete j;
		}
	}
class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
	
public:
	Pool(int qsize) ;
	void start (int nbthread){
		for(int i=0;i<nbthread;i++){
			threads.emplace_back(poolWorker,queue);
		}
	};
	void submit (Job * job){
		//threads.emplace_back(job);
	};
	void stop(){
		queue.SetBlocking(false);
		for(auto& t:threads){
			t.join();
		}
	} ;
	~Pool() ;
};

}
