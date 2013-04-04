#ifndef __AI_H__
#define __AI_H__
#include <vector>
class AI
{
public:
	AI();

	void resetWithPoker(std::vector<int> ap);
	int thisTimePoker();

	inline std::vector<int> getLeftPoker(){return p_allPoker;};
	inline int getLeftPokerCount(){return p_allPoker.size();};
	inline int getSkipCount(){return p_skipCount;};

protected:
	std::vector<int> p_allPoker;
	int p_skipCount;
};
#endif //__AI_H__