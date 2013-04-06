#include "AI.h"
#include "../Constant.h"
using namespace std;
AI::AI()
	:p_skipCount(0)
{
	p_allPoker.clear();
}

void AI::resetWithPoker( std::vector<int> ap)
{
	p_skipCount = 0;
	p_allPoker.clear();
	int size = ap.size();
	for(int i = 0 ; i < size ; i++){
		p_allPoker.push_back(ap.at(i));
	}
}

int AI::thisTimePoker()
{
	int re = -1;
	vector<int> can;
	can.clear();
	int ls = getLeftPokerCount();
	for(int i = 0 ; i < ls ; i++){
		int p = p_allPoker.at(i);
		for(int j = 0 ; j < 8 ; j++){
			if(p == Constant::enablePoker[j]){
				can.push_back(i);
				break;
			}
		}
	}
	if(can.empty()){
		p_skipCount++;
	}else{
		int size = can.size();
		int now = 0;
		if(size > 1){
			now = (int)(1.0f * size * rand() / (RAND_MAX + 1.0f));
		}
		re = p_allPoker.at(can.at(now));
		p_allPoker.erase(can.at(now) + p_allPoker.begin());
	}

	//int now = (int)(1.0f * ls *rand() / (RAND_MAX + 1.0f) );
	//	re = p_allPoker.at(now);
	//	p_allPoker.erase(now + p_allPoker.begin());

	return re;
}
