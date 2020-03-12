#pragma once
template <class Owner, typename ... Args>
class Delegate
{
private:

	struct X {};
	typedef int (X::*Func)();

	X *_ptr;
	Func _func;

public:

	Delegate()
		: _ptr(0), _func(0)
	{
	}

	//Delegate(Owner *owner, int (Owner::*func)(const Args &... args))
	//{
	//	_func = reinterpret_cast<Func>(func);
	//	_ptr = reinterpret_cast<X *>(owner);
	//}
	Delegate(Owner *owner, int (Owner::*func)(const Args ... args))
	{
		_func = reinterpret_cast<Func>(func);
		_ptr = reinterpret_cast<X *>(owner);
	}

	bool operator ! ()
	{
		return (!_ptr || !_func);
	}

	bool operator == (const Delegate &delegate)
	{
		return (_ptr == delegate._ptr && _func == delegate._func);
	}

	bool operator != (const Delegate &delegate)
	{
		return !(*this == delegate);
	}

    operator bool()
    {
        return (_ptr && _func);
    }

	int operator() (const Args &... args)
	{
		typedef int (X::*Mfn)(const Args ...);
		Mfn func = reinterpret_cast<Mfn>(_func);
	
		if (_ptr && func)
			return (_ptr->*func)(args ...);
		return 0;
	}
};