// cc.Delegate.h
// cc.Delegate class
// Author: Shu.KK
// 2006-08-01
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_DELEGATE_H_
#define CC_DELEGATE_H_

#include "cc.h"
#include "cc.Str.h"

namespace cc
{

#define CC_CLASSID_Delegate                   520010
class DelegateConfig : public Object
{
public:
};

template <class T, typename PMethod>
class Delegate : public Object
{
protected:
	T* obj;
	PMethod method;

public:
	Delegate()
	{
		CC_DEFINE_CLASSID(Delegate);
		obj = NULL;
		method = NULL;
	}

	Delegate(T* obj, PMethod method)
	{
		this->obj = obj;
		this->method = method;
	}

	virtual ~Delegate(){};

	virtual bool IsEmpty()
	{
		return (obj == NULL);
	}

};

//template <class T, typename RT>
//class Delegate0 : public Delegate<T, RT (T::*)()>
//{
//public:
//	Delegate0() : Delegate<T, RT (T::*)()>()
//	{
//	}
//
//	Delegate0(T* obj, RT (T::*method)()) : Delegate<T, RT (T::*)()>(obj, method)
//	{
//	}
//
//	RT Invoke()
//	{
//		return (obj->*method)();
//	}
//
//	RT operator()()
//	{
//		return (obj->*method)();
//	}
//};
//
//template <class T, typename RT, class P1>
//class Delegate1 : public Delegate<T, RT (T::*)(P1)>
//{
//public:
//	Delegate1() : Delegate<T, RT (T::*)(P1)>()
//	{
//	}
//
//	Delegate1(T* obj, RT (T::*method)(P1)) : Delegate<T, RT (T::*)(P1)>(obj, method)
//	{
//	}
//
//	RT Invoke(P1 p1)
//	{
//		return (obj->*method)(p1);
//	}
//
//	RT operator()(P1 p1)
//	{
//		return (obj->*method)(p1);
//	}
//};
//
//template <class T, typename RT, class P1, class P2>
//class Delegate2 : public Delegate<T, RT (T::*)(P1, P2)>
//{
//public:
//	Delegate2() : Delegate<T, RT (T::*)(P1, P2)>()
//	{
//	}
//
//	Delegate2(T* obj, RT (T::*method)(P1, P2)) : Delegate<T, RT (T::*)(P1, P2)>(obj, method)
//	{
//	}
//
//	RT Invoke(P1 p1, P2 p2)
//	{
//		return (obj->*method)(p1, p2);
//	}
//
//	RT operator()(P1 p1, P2 p2)
//	{
//		return (obj->*method)(p1, p2);
//	}
//};
//
//template <class T, typename RT, class P1, class P2, class P3>
//class Delegate3 : public Delegate<T, RT (T::*)(P1, P2, P3)>
//{
//public:
//	Delegate3() : Delegate<T, RT (T::*)(P1, P2, P3)>()
//	{
//	}
//
//	Delegate3(T* obj, RT (T::*method)(P1, P2, P3)) : Delegate<T, RT (T::*)(P1, P2, P3)>(obj, method)
//	{
//	}
//
//	RT Invoke(P1 p1, P2 p2, P3 p3)
//	{
//		return (obj->*method)(p1, p2, p3);
//	}
//
//	RT operator()(P1 p1, P2 p2, P3 p3)
//	{
//		return (obj->*method)(p1, p2, p3);
//	}
//};
//
//template <class T, typename RT, class P1, class P2, class P3, class P4>
//class Delegate4 : public Delegate<T, RT (T::*)(P1, P2, P3, P4)>
//{
//public:
//	Delegate4() : Delegate<T, RT (T::*)(P1, P2, P3, P4)>()
//	{
//	}
//
//	Delegate4(T* obj, RT (T::*method)(P1, P2, P3, P4)) : Delegate<T, RT (T::*)(P1, P2, P3, P4)>(obj, method)
//	{
//	}
//
//	RT Invoke(P1 p1, P2 p2, P3 p3, P4 p4)
//	{
//		return (obj->*method)(p1, p2, p3, p4);
//	}
//
//	RT operator()(P1 p1, P2 p2, P3 p3, P4 p4)
//	{
//		return (obj->*method)(p1, p2, p3, p4);
//	}
//};
//
//
//template <class T, typename _RT,
//	typename _P1 = void*, typename _P2 = void*, typename _P3 = void*, typename _P4 = void*,
//	typename _P5 = void*, typename _P6 = void*>
//class Delegate : public Object
//{
//protected:
//	T* _obj;
//	_RT (T::* _method0)();
//	_RT (T::* _method1)(_P1 p1);
//	_RT (T::* _method2)(_P1 p1, _P2 p2);
//	_RT (T::* _method3)(_P1 p1, _P2 p2, _P3 p3);
//	_RT (T::* _method4)(_P1 p1, _P2 p2, _P3 p3, _P4 p4);
//	_RT (T::* _method5)(_P1 p1, _P2 p2, _P3 p3, _P4 p4, _P5 p5);
//	_RT (T::* _method6)(_P1 p1, _P2 p2, _P3 p3, _P4 p4, _P5 p5, _P6 p6);
//
//public:
//	//constructors
//	//hide htis
//	Delegate()
//	{
//		_obj = NULL;
//	}
//
//	Delegate(T* obj, _RT (T::* method)())
//	{
//		_obj = obj;
//		_method0 = method;
//	}
//	Delegate(T* obj, _RT (T::* method)(_P1 p1))
//	{
//		_obj = obj;
//		_method1 = method;
//	}
//	Delegate(T* obj, _RT (T::* method)(_P1 p1, _P2 p2))
//	{
//		_obj = obj;
//		_method2 = method;
//	}
//	Delegate(T* obj, _RT (T::* method)(_P1 p1, _P2 p2, _P3 p3))
//	{
//		_obj = obj;
//		_method3 = method;
//	}
//	Delegate(T* obj, _RT (T::* method)(_P1 p1, _P2 p2, _P3 p3, _P4 p4))
//	{
//		_obj = obj;
//		_method4 = method;
//	}
//	Delegate(T* obj, _RT (T::* method)(_P1 p1, _P2 p2, _P3 p3, _P4 p4, _P5 p5))
//	{
//		_obj = obj;
//		_method5 = method;
//	}
//	Delegate(T* obj, _RT (T::* method)(_P1 p1, _P2 p2, _P3 p3, _P4 p4, _P5 p5, _P6 p6))
//	{
//		_obj = obj;
//		_method6 = method;
//	}
//	virtual ~Delegate(){};
//
//	bool IsEmpty()
//	{
//		return (_obj == NULL);
//	}
//
//	_RT Invoke()
//	{
//		return (_obj->*_method0)();
//	}
//
//	_RT Invoke(_P1 p1)
//	{
//		return (_obj->*_method1)(p1);
//	}
//
//	_RT Invoke(_P1 p1, _P2 p2)
//	{
//		return (_obj->*_method2)(p1, p2);
//	}
//
//	_RT Invoke(_P1 p1, _P2 p2, _P3 p3)
//	{
//		return (_obj->*_method3)(p1, p2, p3);
//	}
//
//	_RT Invoke(_P1 p1, _P2 p2, _P3 p3, _P4 p4)
//	{
//		return (_obj->*_method4)(p1, p2, p3, p4);
//	}
//
//	_RT Invoke(_P1 p1, _P2 p2, _P3 p3, _P4 p4, _P5 p5)
//	{
//		return (_obj->*_method5)(p1, p2, p3, p4, p5);
//	}
//
//	_RT Invoke(_P1 p1, _P2 p2, _P3 p3, _P4 p4, _P5 p5, _P6 p6)
//	{
//		return (_obj->*_method6)(p1, p2, p3, p4, p5, p6);
//	}
//};

} //namespace cc

#endif //#ifndef CC_DELEGATE_H_

