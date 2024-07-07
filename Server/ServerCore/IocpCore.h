#pragma once

/*
	iocpObject
*/
class IocpObject
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0)abstract;

};


/*
	IocpCore
*/
class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() { return _iocpHandle; }

	bool Register(class IocpObject* iocpObject); // iocp�ڵ鿡 Ŭ���̾�Ʈ ���
	bool Dispatch(uint32 timeoutMs = INFINITE);

public:
	HANDLE _iocpHandle;
};

extern IocpCore GIocpCore;

