#include "GameCenter.h"
#include "Constant.h"
USING_NS_CC;
using namespace std;
GameCenter::GameCenter()
{

}

GameCenter::~GameCenter()
{

}

bool GameCenter::init()
{
	bool bRet = false;
	do 
	{

		bRet = true;
	} while (0);
	return bRet;
}

GameCenter* GameCenter::sharedGameCenter()
{
	if(!_instance){
		_instance = new GameCenter();
		_instance->init();
	}
	return _instance;
}

void GameCenter::purgeSharedGameCenter()
{
	CC_SAFE_RELEASE_NULL(_instance);
}

void GameCenter::shuffle()
{
	for(int i = 0 ; i < 52 ; i++){
		_allPoker[i] = i;
		_allPokerIsShow[i] = false;
	}
	for(int j = 0 ; j < 20 ; j++){
		for(int i = 0 ; i < 52 ; i++){
			int temp = _allPoker[i];
			int rdI = (int)(52.0f * rand() / (RAND_MAX + 1.0f)) ;
			if(rdI != i){
				_allPoker[i] = _allPoker[rdI];
				_allPoker[rdI] = temp;
			}
		}
	}
	for(int j = 0 ; j < 4 ; j++){
		for(int i = 0 ; i < 13 ; i++){
			for(int z = i + 1; z < 13 ; z++){
				if(_allPoker[i + j * 13] > _allPoker[z + j * 13]){
					int temp = _allPoker[i + j * 13];
					_allPoker[i + j * 13] = _allPoker[z + j * 13];
					_allPoker[z + j * 13] = temp;
				}
			}
		}
	}
	for(int i = 0 ; i < 8 ; i++){
		Constant::enablePoker[i] = 6 + 13 * (int)(i / 2);
	}
}

std::vector<int> GameCenter::getPlayerPoker( int pid )
{
	vector<int> re;
	if(pid > -1 && pid < 4){
		for(int i = 0 ; i < 13 ; i++){
			re.push_back(_allPoker[pid * 13 + i]);
		}
	}
	return re;
}

void GameCenter::showPoker( int p )
{
	_allPokerIsShow[p] = true;
	refreshAblePoker();
}

void GameCenter::showPokerWithList( std::vector<int> pl )
{
	int size = pl.size();
	for(int i = 0 ; i < size ; i++){
		_allPokerIsShow[pl.at(i)] = true;
	}
	refreshAblePoker();
}

void GameCenter::refreshAblePoker()
{
	for(int i = 0 ; i < 4 ; i++){
		while(Constant::enablePoker[i * 2] > 0 + 13 * i && _allPokerIsShow[Constant::enablePoker[i * 2]] ){
			Constant::enablePoker[i * 2]--;
		}
		while(Constant::enablePoker[i * 2 + 1] < 13 + 13 * i && _allPokerIsShow[Constant::enablePoker[i * 2 + 1]]){
			Constant::enablePoker[i * 2 + 1]++;
		}
	}
}

GameCenter* GameCenter::_instance = NULL;
