#ifndef PLAYER_H
#define PLAYER_H
#include <string.h>


class Player
{
public:
	Player()
	{

	}
	Player(const Player& p)
	{
		_player_count = p._player_count;
	}
private:
	typedef struct _account_info
	{
		_account_info()
		{
			memset(_username,0,sizeof(_username));
			memset(_password,0,sizeof(_password));
		}
		_account_info& operator=(const _account_info& ai)
		{
			if(this != &ai)
			{
				memset(_username,0,sizeof(_username));
				memset(_password,0,sizeof(_password));
				strcpy(_username,ai._username);
				strcpy(_password,ai._password);
				
			}
			return *this; 
		}
		const unsigned int MAXLENGTH = 20;
		char _username[MAXLENGTH];
		char _password[MAXLENGTH];
	} player_info;	

	player_info _player_count;
};

#endif