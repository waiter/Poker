#ifndef __GAME_CENTER_H__
#define __GAME_CENTER_H__
#include "cocos2d.h"
#include <vector>
class GameCenter : public cocos2d::CCObject
{
public:
	GameCenter();
	~GameCenter();
	bool init();

	void shuffle();
	// 0 - 3
	std::vector<int> getPlayerPoker(int pid);
	void showPoker(int p);
	void showPokerWithList(std::vector<int> pl);

	static GameCenter* sharedGameCenter();
	static void purgeSharedGameCenter();
private:
	static GameCenter* _instance;

	int _allPoker[52];
	bool _allPokerIsShow[52];

	void refreshAblePoker();
};
#endif //__GAME_CENTER_H__