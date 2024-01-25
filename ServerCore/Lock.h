#pragma once
#ifndef _LOCK_
#define _LOCK_ 

#include "Types.h"

/*------------------
*	RW SpinLock
-------------------*/

/*-----------------
*
* [WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
* Write Flag (Excluvie Lock Owner ThreadID)
* Read Flag (Shard Lock Count)
------------------*/


// W -> W (o)
// W -> R (o)
// R -> W (X)
class Lock
{
	enum : uint32
	{
		AQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};
public:
	void WriteLock(const char* name);
	void WriteUnLock(const char* name);
	void ReadLock(const char* name);
	void ReadUnLock(const char* name);
private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};


/*------------------
*	LockGuards
-------------------*/
class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const char* name) 
		: _lock(lock), _name(name) {
		_lock.ReadLock(name);
	}
	~ReadLockGuard() {
		_lock.ReadUnLock(_name);
	}
private:
	Lock& _lock;
	const char* _name;
};


class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) 
		: _lock(lock), _name(name) {
		_lock.WriteLock(name);
	}
	~WriteLockGuard() {
		_lock.WriteUnLock(_name);
	}
private:
	Lock& _lock;
	const char* _name;
};

#endif