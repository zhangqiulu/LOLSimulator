#ifndef PLAYER_H
#define PLAYER_H
#include <memory.h>


class Player
{
private:
	struct _account_info
	{
		_account_info()
		{
			memset(_username,0,sizeof(_username));
			memset(_password,0,sizeof(_password));
		}
		char _username[16];
		char _password[16];
	};	
};

#endif