#pragma once

/*
	iocpObject
*/
class IocpObject :public enable_shared_from_this<IocpObject>
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

	bool Register(IocpObjectRef iocpObject); // iocp핸들에 클라이언트 등록
	bool Dispatch(uint32 timeoutMs = INFINITE);

public:
	HANDLE _iocpHandle;
};

