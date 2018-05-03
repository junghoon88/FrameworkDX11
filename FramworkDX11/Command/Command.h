#pragma once
#include "stdafx.h"

class Command
{
public:
	friend class CommandManager;

protected:
	virtual void Execute(void) = 0;
	virtual void Undo(void) = 0;
	virtual void Redo(void) = 0;
};

class CommandManager
{
public:
	CommandManager() : maxCommand(100), curCommand(-1) {}
	~CommandManager() { ClearCommands(); }

	void AddCommand(Command* cmd)
	{
		//curCommand 가 맨 끝이 아닐때
		//즉, Redo 가 가능한 상태일때 Redo 영역을 지운다.
		while (curCommand < (int)oldCommands.size() - 1)
		{
			oldCommands.erase(oldCommands.begin() + (int)oldCommands.size() - 1);
		}

		//스택이 꽉차면 제일 앞에 데이터를 지운다.
		if (oldCommands.size() == maxCommand)
		{
			SAFE_DELETE(oldCommands[0]);
			oldCommands.erase(oldCommands.begin());
		}

		//스택 추가
		oldCommands.push_back(cmd);

		//현재위치를 제일 뒤로
		curCommand = oldCommands.size() - 1;

		cmd->Execute();
	}

	void UndoCommand(void)
	{
		if (oldCommands.size() == 0) return;
		if (curCommand == -1) return;

		oldCommands[curCommand--]->Undo();
	}

	void RedoCommand(void)
	{
		if (oldCommands.size() == 0) return;
		if (curCommand == oldCommands.size() - 1) return;

		oldCommands[++curCommand]->Redo();
	}

	void ClearCommands(void)
	{
		while (oldCommands.size() > 0)
		{
			SAFE_DELETE(oldCommands[0]);
			oldCommands.erase(oldCommands.begin());
		}
		curCommand = -1;
	}

	inline Command* GetUndoCommand(void)
	{
		if(curCommand > -1)
			return oldCommands[curCommand]; 
		
		return NULL;
	}
	inline Command* GetRedoCommand(void)
	{
		if(curCommand < (int)oldCommands.size() - 1)
			return oldCommands[curCommand+1]; 

		return NULL;
	}

private:
	vector<Command*> oldCommands;
	UINT maxCommand;
	int curCommand;
};

