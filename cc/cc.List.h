// cc.List.h
// The Java like List,Map class
// Author: Shu.KK
// 2005-01-01,2006-07-01
/*
assess form index is quick than from key

*/
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef CC_List_H_
#define CC_List_H_

#if defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#endif

#include "cc.h"
#ifdef __BORLANDC__
#include <windows.h>
#endif /* __BORLANDC__ */

#include <ctype.h>
#include <wchar.h>
#include <tchar.h>
#include <string.h>
#include <stdlib.h>

#include "cc.Str.h"
#include "cc.Exception.h"

namespace cc
{


// TEMPLATE STRUCT CC_comp
template<class _Ty>
struct CC_comp
{
	// functor for operator==
	bool equals(const _Ty& _Left, const _Ty& _Right, bool bIsIgnoreCase) const
	{
		return (_Left == _Right);
	}

	// functor for operator<
	bool less(const _Ty& _Left, const _Ty& _Right, bool bIsIgnoreCase) const
	{
		return (_Left < _Right);
	}

	// functor for operator=
	void copy(_Ty* _Left, const _Ty& _Right)
	{
		*_Left = _Right;
	}

	// init
	void init(_Ty* key) const
	{
	}

	// Dispose
	void Dispose(_Ty* key) const
	{
		//TODO
	}
};

template<>
struct CC_comp<cc::Str>
{
	// functor for operator-
	bool equals(const cc::Str& _Left, const cc::Str& _Right, bool bIsIgnoreCase) const
	{
		return (_Left.CompareTo(_Right, bIsIgnoreCase) == 0);
	}

	bool less(const cc::Str& _Left, const cc::Str& _Right, bool bIsIgnoreCase) const
	{
		return (_Left.CompareTo(_Right, bIsIgnoreCase) < 0);
	}

	// functor for operator=
	void copy(cc::Str* _Left, const cc::Str& _Right)
	{
		*_Left = _Right;
	}

	// init
	void init(cc::Str* key) const
	{
	}

	// Dispose
	void Dispose(cc::Str* key) const
	{
		if(key != NULL)
		{
			key->Clear();
		}
	}
};

template<>
struct CC_comp<char*>
{
	// functor for operator==
	bool equals(const char* _Left, const char* _Right, bool bIsIgnoreCase) const
	{
		if(_Left == NULL && _Right == NULL)
		{
			return true;
		}
		if(_Left == NULL || _Right == NULL)
		{
			return false;
		}
		if(bIsIgnoreCase)
		{
			#ifdef __BORLANDC__
			return (stricmp(_Left, _Right) == 0);
			#else
			return (_stricmp(_Left, _Right) == 0);
			#endif /* __BORLANDC__ */
		}
		return (strcmp(_Left, _Right) == 0);
	}

	// functor for operator<
	bool less(const char* _Left, const char* _Right, bool bIsIgnoreCase) const
	{
		if(_Left == NULL && _Right != NULL)
		{
			return true;
		}
		if(_Left == NULL || _Right == NULL)
		{
			return false;
		}
		if(bIsIgnoreCase)
		{
			#ifdef __BORLANDC__
			return (stricmp(_Left, _Right) < 0);
			#else
			return (_stricmp(_Left, _Right) < 0);
			#endif /* __BORLANDC__ */
		}
		return (strcmp(_Left, _Right) < 0);
	}

	// functor for operator=
	void copy(char** _Left, const char* _Right)
	{
		if(*_Left != NULL)
		{
			free(*_Left);
			*_Left = NULL;
		}
		if(_Right == NULL)
		{
			return;
		}
		size_t nLen = strlen(_Right);
		*_Left = (char*)malloc(nLen + 1);
#ifdef __STDC_SECURE_LIB__
		strncpy_s(*_Left, nLen + 1, _Right, nLen);
#else
		strncpy(*_Left, _Right, nLen);
#endif
		(*_Left)[nLen] = '\0';
	}

	// init
	void init(char** key) const
	{
		*key = NULL;
	}

	// Dispose
	void Dispose(char** key) const
	{
		if(*key != NULL)
		{
			free((void*)*key);
			*key = NULL;
		}
	}
};

template<>
struct CC_comp<wchar_t*>
{
	// functor for operator==
	bool equals(const wchar_t* _Left, const wchar_t* _Right, bool bIsIgnoreCase) const
	{
		if(_Left == NULL && _Right == NULL)
		{
			return true;
		}
		if(_Left == NULL || _Right == NULL)
		{
			return false;
		}
		if(bIsIgnoreCase)
		{
			return (_wcsicmp(_Left, _Right) == 0);
		}
		return (wcscmp(_Left, _Right) == 0);
	}

	// functor for operator<
	bool less(const wchar_t* _Left, const wchar_t* _Right, bool bIsIgnoreCase) const
	{
		if(_Left == NULL && _Right != NULL)
		{
			return true;
		}
		if(_Left == NULL || _Right == NULL)
		{
			return false;
		}
		if(bIsIgnoreCase)
		{
			return (_wcsicmp(_Left, _Right) < 0);
		}
		return (wcscmp(_Left, _Right) < 0);
	}

	// functor for operator=
	void copy(wchar_t** _Left, const wchar_t* _Right)
	{
		if(*_Left != NULL)
		{
			free(*_Left);
			*_Left = NULL;
		}
		if(_Right == NULL)
		{
			return;
		}
		size_t nLen = wcslen(_Right);
		*_Left = (wchar_t*)malloc((nLen + 1) * sizeof(wchar_t));
#ifdef __STDC_SECURE_LIB__
		wcsncpy_s(*_Left, nLen + 1, _Right, nLen);
#else
		wcsncpy(*_Left, _Right, nLen);
#endif
		(*_Left)[nLen] = '\0';
	}

	// init
	void init(wchar_t** key) const
	{
		*key = NULL;
	}

	// Dispose
	void Dispose(wchar_t** key) const
	{
		if(*key != NULL)
		{
			free((void*)*key);
			*key = NULL;
		}
	}
};

///////////////////////////////////////////////////////////////////////////////
//List
template <class _KTy, class _KPr = CC_comp<_KTy> >
class List : public Object
{
protected:
	typedef List<_KTy, _KPr> _Myt;
	typedef _KPr fun_compare;
	fun_compare _comp; // the comparator predicate for keys

	struct Assoc
	{
		_KTy key; // at Arr be treated as val,but others is key
	};

	bool _isSort;
	bool _isSortDESC;
	bool _isDistinct;
	void _Alloc(int len);
	int _Ins(int index, Assoc *mBufNew);
	//called by constructors,so is not valid even with virtual
	void _Init();
	void _RemoveAt(int index);
	virtual void* _NewAssoc(const _KTy &key);
	virtual int _SizeAssoc() const;
	virtual void* _GetAssoc(int index) const;
	virtual void _InsAssoc(int index, void* mBufNew);
	virtual void _MoveAssoc(int nFrom, int nTo, int nSize);
	virtual bool _EqualsAssoc(const void* pAssoc1, const void* pAssoc2) const;
	virtual void _DisposeAssoc(void* pAssoc);

	Assoc** _entity;
	Assoc* _mBufTmp;
	int _nCapacity;
	int _nRealCapacity;
	int _nMinCapacity;
	double _nIncrement;
	bool _bIsIgnoreCase;

public:
	//constructors
	List();
	//only valid while add nexs element
	List(int nMinCapacity, double nIncrement = 1.3);
	List(bool isSort, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
	List(_Myt &list, bool isSort = false, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
	List(_Myt &list, int fromIndex, bool isSort = false, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
	List(_Myt &list, int fromIndex, int len, bool isSort = false, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
	virtual ~List();
	int Add(const _KTy &key);
	int Add(_Myt &list);
	int Add(_Myt &list, int fromIndex);
	int Add(_Myt &list, int fromIndex, int len);
	//if is sorted,then error!
	int Insert(int index, const _KTy &key);
	//return key
	const _KTy &GetKey(int index) const;
	//if is sorted,then error!
	void SetKey(int index, const _KTy &key);
	void Remove(const _KTy &key);
	void RemoveAt(int index);
	void Swap(int fromIndex, int toIndex);
	void Swap(const _KTy &key, int toIndex);
	void Clear();
	int GetSize() const;
	bool ContainsKey(const _KTy &key) const;

	//compare Value.
	virtual bool Equals(const List<_KTy, _KPr> &list) const;

	//assignment operators
	List<_KTy, _KPr> &operator=(List<_KTy, _KPr> &list);
	bool operator==(List<_KTy, _KPr> &list) const;
	bool operator!=(List<_KTy, _KPr> &list) const;
	//========================================
	//OTHERS
	bool IsEmpty() const {return (bool)(_nCapacity == 0);}
	bool IsSort() const {return _isSort;}
	bool IsSortDESC() const {return _isSortDESC;}
	bool IsDistinct() const {return _isDistinct;}
	//only valid while add nexs element
	void SetCapacity(int nMinCapacity, double nIncrement = 1.3);
	// return index of position
	int GetIndex(const _KTy &key) const;
	int GetMinCapacity() const {return _nMinCapacity;}
	double GetIncrement() const {return _nIncrement;}
	bool GetIsIgnoreCase() const {return _bIsIgnoreCase;}
	void SetIsIgnoreCase(bool isIgnoreCase) {CC_ASSERT(_nCapacity == 0);_bIsIgnoreCase = isIgnoreCase;}
};

template <class _KTy, class _KPr>
List<_KTy, _KPr>::List()
{
	_Init();
}

template <class _KTy, class _KPr>
List<_KTy, _KPr>::List(int nMinCapacity, double nIncrement)
{
	_Init();
	if(nMinCapacity > 0)
	{
		_nMinCapacity = nMinCapacity;
	}
	if(nIncrement > 1.0)
	{
		_nIncrement = nIncrement;
	}
}

template <class _KTy, class _KPr>
List<_KTy, _KPr>::List(bool isSort, bool bIsDESC, bool isDistinct, int nMinCapacity, double nIncrement)
{
	_Init();
	if(nMinCapacity > 0)
	{
		_nMinCapacity = nMinCapacity;
	}
	if(nIncrement > 1.0)
	{
		_nIncrement = nIncrement;
	}
	_isSort = isSort;
	_isSortDESC = bIsDESC;
	_isDistinct = isDistinct;
}

template <class _KTy, class _KPr>
List<_KTy, _KPr>::List(List<_KTy, _KPr> &list, bool isSort, bool bIsDESC, bool isDistinct, int nMinCapacity, double nIncrement)
{
	_Init();
	if(nMinCapacity > 0)
	{
		_nMinCapacity = nMinCapacity;
	}
	if(nIncrement > 1.0)
	{
		_nIncrement = nIncrement;
	}
	_isSort = isSort;
	_isSortDESC = bIsDESC;
	_isDistinct = isDistinct;
	Add(list);
}

template <class _KTy, class _KPr>
List<_KTy, _KPr>::List(List<_KTy, _KPr> &list, int fromIndex, bool isSort, bool bIsDESC, bool isDistinct, int nMinCapacity, double nIncrement)
{
	_Init();
	if(nMinCapacity > 0)
	{
		_nMinCapacity = nMinCapacity;
	}
	if(nIncrement > 1.0)
	{
		_nIncrement = nIncrement;
	}
	_isSort = isSort;
	_isSortDESC = bIsDESC;
	_isDistinct = isDistinct;
	Add(list, fromIndex);
}

template <class _KTy, class _KPr>
List<_KTy, _KPr>::List(List<_KTy, _KPr> &list, int fromIndex, int len, bool isSort, bool bIsDESC, bool isDistinct, int nMinCapacity, double nIncrement)
{
	_Init();
	if(nMinCapacity > 0)
	{
		_nMinCapacity = nMinCapacity;
	}
	if(nIncrement > 1.0)
	{
		_nIncrement = nIncrement;
	}
	_isSort = isSort;
	_isSortDESC = bIsDESC;
	_isDistinct = isDistinct;
	Add(list, fromIndex, len);
}

template <class _KTy, class _KPr>
List<_KTy, _KPr>::~List()
{
	Clear();
}

template <class _KTy, class _KPr>
inline void List<_KTy, _KPr>::_Init()
{
	_isDistinct = false;
	_isSort = false;
	_isSortDESC = false;

	_entity = NULL;
	_nCapacity = 0;
	_nRealCapacity = 0;
	_nMinCapacity = 0;
	_nIncrement = 1.3;
}

template <class _KTy, class _KPr>
void* List<_KTy, _KPr>::_NewAssoc(const _KTy &key)
{
	//for not auto destructor,not use malloc
	//mEntry* mBufNew = (mEntry*)malloc(sizeof(mEntry));
	//if memset to '\0',then constructors info will be cleared
	//memset(mBufNew, '\0', sizeof(mEntry));
	Assoc* mBufNew = new Assoc[1];
	if(mBufNew == NULL)
	{
 		throw OutOfMemoryException(_T("List::_NewAssoc(const _KTy &key), ERROR for new Assoc[1]."));
	}
	_comp.init(&(mBufNew->key));
	_comp.copy(&(mBufNew->key), key);
	return (void*)mBufNew;
}

template <class _KTy, class _KPr>
int List<_KTy, _KPr>::_SizeAssoc() const
{
	return sizeof(Assoc*);
}

template <class _KTy, class _KPr>
void* List<_KTy, _KPr>::_GetAssoc(int index) const
{
	return (void*)(*(_entity + index));
}

template <class _KTy, class _KPr>
void List<_KTy, _KPr>::_InsAssoc(int index, void *mBufNew)
{
	*(_entity + index) = (Assoc*)mBufNew;
}

template <class _KTy, class _KPr>
void List<_KTy, _KPr>::_MoveAssoc(int nFrom, int nTo, int nSize)
{
	memmove(_entity + nFrom, _entity + nTo, nSize * _SizeAssoc());
}

template <class _KTy, class _KPr>
bool List<_KTy, _KPr>::_EqualsAssoc(const void* pAssoc1, const void* pAssoc2) const
{
	return (_comp.equals(((Assoc*)pAssoc1)->key, ((Assoc*)pAssoc2)->key, _bIsIgnoreCase));
}

template <class _KTy, class _KPr>
void List<_KTy, _KPr>::_DisposeAssoc(void* pAssoc)
{
	_comp.Dispose(&(  ((Assoc*)pAssoc)->key  ));
}

// return index of position
template <class _KTy, class _KPr>
int List<_KTy, _KPr>::GetIndex(const _KTy &key) const
{
	//if sorted,and has many items
	if(_isSort && _nCapacity > 3)
	{
		int index = -1;
		int indexOK = -1;
		Assoc* _mBufTmp;
		//for same value,return first
		int leftpos = 0;
		int rightpos = _nCapacity - 1;
		index = rightpos / 2;
//cc::Log::fatal(_T("index:%d, leftpos:%d, rightpos:%d, key:%d"), index, leftpos, rightpos, key);
		if(_isSortDESC)
		{
			while(rightpos != leftpos)
			{
				_mBufTmp = (Assoc*)_GetAssoc(index);
				if(_comp.less(_mBufTmp->key, key, _bIsIgnoreCase))
				{
					rightpos = index;
					if(rightpos > 0 && rightpos > leftpos) rightpos--;
					index = (leftpos + rightpos) / 2;
				}
				else
				{
					if(_comp.equals(_mBufTmp->key, key, _bIsIgnoreCase))
					{
						if(_isDistinct)
						{
							return index;
						}
						indexOK = index;
						rightpos = index;
						if(rightpos > 0 && rightpos > leftpos) rightpos--;
						index = (leftpos + rightpos) / 2;
					}
					else
					{
						leftpos = index + 1;
						index = (leftpos + rightpos) / 2;
					}
				}
			}
		}
		else
		{
			while(rightpos != leftpos)
			{
				_mBufTmp = (Assoc*)_GetAssoc(index);
				if(_comp.less(key, _mBufTmp->key, _bIsIgnoreCase))
				{
					rightpos = index;
					if(rightpos > 0 && rightpos > leftpos) rightpos--;
					index = (leftpos + rightpos) / 2;
				}
				else
				{
					if(_comp.equals(key, _mBufTmp->key, _bIsIgnoreCase))
					{
						if(_isDistinct)
						{
							return index;
						}
						indexOK = index;
						rightpos = index;
						if(rightpos > 0 && rightpos > leftpos) rightpos--;
						index = (leftpos + rightpos) / 2;
					}
					else
					{
						leftpos = index + 1;
						index = (leftpos + rightpos) / 2;
					}
				}
//cc::Log::fatal(_T("notSortDESC, index:%d, rightpos:%d, leftpos:%d"), index, rightpos, leftpos);
			}
		}
		_mBufTmp = (Assoc*)_GetAssoc(index);
		if(_comp.equals(key, _mBufTmp->key, _bIsIgnoreCase))
		{
			return index;
		}
		return indexOK;
	}

	for(int i = 0; i < _nCapacity; i++)
	{
		const Assoc* pAssoc = (Assoc*)_GetAssoc(i);
		if(_comp.equals(pAssoc->key, key, _bIsIgnoreCase))
		{
			return i;
		}
	}
	return -1;
}

template <class _KTy, class _KPr>
void List<_KTy, _KPr>::_RemoveAt(int index)
{
	if(index < 0 || index >= _nCapacity)
	{
		return;
	}
	_mBufTmp = (Assoc*)_GetAssoc(index);
	//_comp.Dispose(&(_mBufTmp->key));
	_DisposeAssoc(_mBufTmp);
	delete []_mBufTmp;
	if(index < _nCapacity - 1)
	{
		_MoveAssoc(index, index + 1, _nCapacity - index - 1);
	}
	_Alloc(_nCapacity - 1);
}

/*
pay attention to:
  _entity may be changed by this process,so you need adjust this.
*/
template <class _KTy, class _KPr>
void List<_KTy, _KPr>::_Alloc(int len)
{
	_nCapacity = (len < 0) ? 0 : len;

	int nNeedLen = (int)(_nCapacity * _nIncrement);
	if(nNeedLen < _nMinCapacity)
	{
		nNeedLen = _nMinCapacity;
	}
	if(_nCapacity > _nRealCapacity || _nRealCapacity > nNeedLen)
	{
		for(int i = len; i < _nCapacity; i++)
		{
			_RemoveAt(i);
		}
		_entity = (Assoc **)realloc(_entity, (nNeedLen + 1) * _SizeAssoc());
		if(_entity == NULL)
		{
			// throw:g++ only,if gcc then error!;
			#ifdef _DEBUG_
			_tprintf(_T("Err:realloc or malloc(%d)\n"), (nNeedLen + 1) * _SizeAssoc());
			#endif //#ifdef _DEBUG_
 			throw OutOfMemoryException(_T("List::_Alloc(int len), ERROR for realloc memory."));
		}
		_nRealCapacity = nNeedLen;
	}
}

template <class _KTy, class _KPr>
int List<_KTy, _KPr>::_Ins(int index, Assoc *mBufNew)
{
	//can be pos of end(=_nCapacity)
	if(index < 0 || index > _nCapacity)
	{
 		throw ArgumentException(_T("List::_Ins(int index, Assoc *mBufNew), ERROR for out of Capacity."));
	}

	//if sorted,then index is changed
	if(_isSort)
	{
		//with the same value,the last add be lastly
		int leftpos = 0;
		int rightpos = _nCapacity;
		index = (rightpos - leftpos) / 2;
		if(_isSortDESC)
		{
			while(rightpos != leftpos)
			{
				_mBufTmp = (Assoc*)_GetAssoc(index);
				if(_comp.less(_mBufTmp->key, mBufNew->key, _bIsIgnoreCase))
				{
					rightpos = index;
					index = (leftpos + index) / 2;
				}
				else
				{
					leftpos = index + 1;
					index = (rightpos + index) / 2;
				}
			}
		}
		else
		{
			while(rightpos != leftpos)
			{
				_mBufTmp = (Assoc*)_GetAssoc(index);
				if(_comp.less(mBufNew->key, _mBufTmp->key, _bIsIgnoreCase))
				{
					rightpos = index;
					index = (leftpos + index) / 2;
				}
				else
				{
					leftpos = index + 1;
					index = (rightpos + index) / 2;
				}
			}
		}
		index = rightpos;
	}

	int nOldLen = _nCapacity;
	_Alloc(nOldLen + 1);
	//now _nCapacity is add 1 in _Alloc
	if(index < nOldLen)
	{
		_MoveAssoc(index + 1, index, nOldLen - index);
	}
	_InsAssoc(index, mBufNew);
	return index;
}

template <class _KTy, class _KPr>
void List<_KTy, _KPr>::Clear()
{
	for(int i = 0; i < _nCapacity; i++)
	{
		_mBufTmp = (Assoc*)_GetAssoc(i);
		//mBufTmp[0]->val.~Obj();
		//_comp.Dispose(&(_mBufTmp->key));
		_DisposeAssoc(_mBufTmp);
		delete []_mBufTmp;
	}
	if(_entity != NULL)
	{
		free(_entity);
		_entity = NULL;
	}

	_nCapacity = 0;
	_nRealCapacity = 0;
}

template <class _KTy, class _KPr>
int List<_KTy, _KPr>::GetSize() const
{
	return _nCapacity;
}

template <class _KTy, class _KPr>
bool List<_KTy, _KPr>::Equals(const List<_KTy, _KPr> &list) const
{
	//if is self
	if(_entity == list._entity)
	{
		return true;
	}
	if(_nCapacity != list.GetSize())
	{
		return false;
	}
	for(int i = 0; i < _nCapacity; i++)
	{
		const Assoc *_mBufTmp1 = (Assoc*)_GetAssoc(i);
		const Assoc *_mBufTmp2 = (Assoc*)list._GetAssoc(i);
		//if(_mBufTmp->key != list.GetKey(i))
		if(!_EqualsAssoc(_mBufTmp1, _mBufTmp2))
		{
			return false;
		}
	}
	return true;
}

template <class _KTy, class _KPr>
int List<_KTy, _KPr>::Add(const _KTy &key)
{
	if(_isDistinct)
	{
		int nIndex = GetIndex(key);
		if(nIndex != -1)
		{
			return nIndex;
		}
	}
	return _Ins(_nCapacity, (Assoc*)_NewAssoc(key));
}

template <class _KTy, class _KPr>
int List<_KTy, _KPr>::Add(List<_KTy, _KPr> &list)
{
	return Add(list, 0, list.GetSize());
}

template <class _KTy, class _KPr>
int List<_KTy, _KPr>::Add(List<_KTy, _KPr> &list, int fromIndex)
{
	return Add(list, fromIndex, list.GetSize() - fromIndex);
}

template <class _KTy, class _KPr>
int List<_KTy, _KPr>::Add(List<_KTy, _KPr> &list, int fromIndex, int len)
{
	//if is self
	if(_entity == list._entity)
	{
		return 0;
	}
	if(fromIndex < 0 || len > list.GetSize() - fromIndex)
	{
 		throw ArgumentException(_T("List::Add(&list, int fromIndex, int len), ERROR for out of Capacity."));
	}
	int cnt = 0;
	for(int i = fromIndex; i < fromIndex + len; i++)
	{
		Add(list.GetKey(i));
		cnt++;
	}
	return cnt;
}

template <class _KTy, class _KPr>
int List<_KTy, _KPr>::Insert(int index, const _KTy &key)
{
	//if sort,then cannot refer the index
	if(_isSort)
	{
 		throw ArgumentException(_T("List::Insert(int index, const _KTy &key), ERROR for insert while is Sorted."));
	}
	return _Ins(index, (Assoc*)_NewAssoc(key));
}

template <class _KTy, class _KPr>
const _KTy& List<_KTy, _KPr>::GetKey(int index) const
{
	//can be pos of end(=_nCapacity)
	if(index < 0 || index > _nCapacity)
	{
 		throw ArgumentException(_T("List::GetKey(int index), ERROR for out of Capacity."));
	}
	const Assoc* pAssoc = (Assoc*)_GetAssoc(index);
	return pAssoc->key;
}

template <class _KTy, class _KPr>
void List<_KTy, _KPr>::Swap(int fromIndex, int toIndex)
{
	//if is sorted,then error!
	if(_isSort || fromIndex < 0 || fromIndex > _nCapacity || toIndex < 0 || toIndex > _nCapacity)
	{
 		throw ArgumentException(_T("List::Swap(int fromIndex, int toIndex), ERROR for out of Capacity."));
	}
	if(fromIndex != toIndex)
	{
		int nSize = _SizeAssoc();
		void* mBuf = malloc(nSize);
		memmove(mBuf, _entity + toIndex, nSize);
		memmove(_entity + toIndex, _entity + fromIndex, nSize);
		memmove(_entity + fromIndex, mBuf, nSize);
		free(mBuf);
	}
}

template <class _KTy, class _KPr>
void List<_KTy, _KPr>::Swap(const _KTy &key, int toIndex)
{
	int fromIndex = GetIndex(key);
	Swap(fromIndex, toIndex);
}

template <class _KTy, class _KPr>
void List<_KTy, _KPr>::SetKey(int index, const _KTy &key)
{
	//if is sorted,then error!
	if(_isSort || index < 0 || index > _nCapacity)
	{
 		throw ArgumentException(_T("List::SetAt(int index, const _KTy &key), ERROR for out of Capacity."));
	}
	Assoc* pAssoc = (Assoc*)_GetAssoc(index);
	_comp.copy(&(pAssoc->key), key);
}

template <class _KTy, class _KPr>
void List<_KTy, _KPr>::Remove(const _KTy &key)
{
	return _RemoveAt(GetIndex(key));
}

template <class _KTy, class _KPr>
void List<_KTy, _KPr>::RemoveAt(int index)
{
	return _RemoveAt(index);
}

template <class _KTy, class _KPr>
bool List<_KTy, _KPr>::ContainsKey(const _KTy &key) const
{
	return (bool)(GetIndex(key) != -1);
}

// Compare implementations
template <class _KTy, class _KPr>
bool List<_KTy, _KPr>::operator==(List<_KTy, _KPr> &list) const
{
	return Equals(list);
}

template <class _KTy, class _KPr>
bool List<_KTy, _KPr>::operator!=(List<_KTy, _KPr> &list) const
{
	return (!Equals(list));
}

template <class _KTy, class _KPr>
List<_KTy, _KPr>& List<_KTy, _KPr>::operator=(List<_KTy, _KPr> &list)
{
	//if is self
	if(_entity == list._entity)
	{
		return *this;
	}
	Clear();
	Add(list, 0, list.GetSize());
	return *this;
}

template <class _KTy, class _KPr>
void List<_KTy, _KPr>::SetCapacity(int nMinCapacity, double nIncrement)
{
	if(nMinCapacity > 0)
	{
		_nMinCapacity = nMinCapacity;
	}
	if(nIncrement > 1.0)
	{
		_nIncrement = nIncrement;
	}
}

///////////////////////////////////////////////////////////////////////////////
//Map
template <class _KTy, class _Ty, class _KPr = CC_comp<_KTy>, class _Pr = CC_comp<_Ty> >
class Map : public List<_KTy, _KPr>
{
protected:
	typedef Map<_KTy, _Ty, _KPr, _Pr> _Myt;
	typedef _KTy key_type;
	typedef _Ty val_type;
	typedef _Pr fun_val_compare;
	fun_val_compare _vcomp; // the comparator predicate for values

#if defined __GNUC__ && __GNUC__ >= 3
	struct Assoc
	{
		_KTy key; // at Arr be treated as val,but others is key
	};
#endif
	struct MapAssoc
	{
		_KTy key;
		_Ty val;
	};

	virtual void* _NewAssoc(const _KTy &key);
	virtual int _SizeAssoc() const;
	virtual void* _GetAssoc(int index) const;
	virtual void _InsAssoc(int index, void* mBufNew);
	virtual void _MoveAssoc(int nFrom, int nTo, int nSize);
	virtual bool _EqualsAssoc(const void* pAssoc1, const void* pAssoc2) const;
	virtual void _DisposeAssoc(void* pAssoc);
	MapAssoc* _mBufTmp;

public:
	//constructors
	Map();
	Map(int nMinCapacity, double nIncrement = 1.3);
	Map(bool isSort, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
	Map(_Myt &list, bool isSort = false, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
	Map(_Myt &list, int fromIndex, bool isSort = false, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
	Map(_Myt &list, int fromIndex, int len, bool isSort = false, bool bIsDESC = false, bool isDistinct = false, int nMinCapacity = 0, double nIncrement = 1.3);
	virtual ~Map();
	//bool ContainsKey(const _KTy &key); //privade by base
	bool ContainsValue(const _Ty &val);
	int Add(const _KTy &key, const _Ty &val);
	int Add(_Myt &map);
	int Add(_Myt &map, int fromIndex);
	int Add(_Myt &map, int fromIndex, int len);
	const _KTy& GetKey(int index) const;
	const _Ty& Get(const _KTy &key) const;
	void SetValue(const _KTy &key, const _Ty &val);
	_Ty& GetAt(int index) const;
	void SetValueAt(int index, const _Ty &val);

	//assignment operators
	_Myt &operator=(_Myt &list);
	bool operator==(_Myt &list) const;
	bool operator!=(_Myt &list) const;
	//========================================
	//OTHERS
private:
	//for hide base's SetKey(base is return key)
	void Swap(int fromIndex, int toIndex){};
	void Swap(const _KTy &key, int toIndex){};
	void SetKey(int index, const _KTy &key){};

};

template <class _KTy, class _Ty, class _KPr, class _Pr>
Map<_KTy, _Ty, _KPr, _Pr>::Map() : List<_KTy, _KPr>()
{
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
Map<_KTy, _Ty, _KPr, _Pr>::Map(int nMinCapacity, double nIncrement)
	: List<_KTy, _KPr>(nMinCapacity, nIncrement)
{
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
Map<_KTy, _Ty, _KPr, _Pr>::Map(bool isSort, bool bIsDESC , bool isDistinct, int nMinCapacity, double nIncrement)
	: List<_KTy, _KPr>(isSort, bIsDESC, isDistinct, nMinCapacity, nIncrement)
{
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
Map<_KTy, _Ty, _KPr, _Pr>::Map(Map<_KTy, _Ty, _KPr, _Pr> &map, bool isSort, bool bIsDESC, bool isDistinct, int nMinCapacity, double nIncrement)
: List<_KTy, _KPr>(isSort, bIsDESC, isDistinct, nMinCapacity, nIncrement)
{
	Add(map, 0, map.GetSize());
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
Map<_KTy, _Ty, _KPr, _Pr>::Map(Map<_KTy, _Ty, _KPr, _Pr> &map, int fromIndex, bool isSort, bool bIsDESC, bool isDistinct, int nMinCapacity, double nIncrement)
	: List<_KTy, _KPr>(isSort, bIsDESC, isDistinct, nMinCapacity, nIncrement)
{
	Add(map, fromIndex, map.GetSize() - fromIndex);
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
Map<_KTy, _Ty, _KPr, _Pr>::Map(Map<_KTy, _Ty, _KPr, _Pr> &map, int fromIndex, int len, bool isSort, bool bIsDESC, bool isDistinct, int nMinCapacity, double nIncrement)
	: List<_KTy, _KPr>(isSort, bIsDESC, isDistinct, nMinCapacity, nIncrement)
{
	Add(map, fromIndex, len);
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
Map<_KTy, _Ty, _KPr, _Pr>::~Map()
{
	//must need! otherwise not call to _DisposeAssoc from List::Clear
	Clear();
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
void* Map<_KTy, _Ty, _KPr, _Pr>::_NewAssoc(const _KTy &key)
{
	//for not auto destructor,not use malloc
	//mEntry* mBufNew = (mEntry*)malloc(sizeof(mEntry));
	//if memset to '\0',then constructors info will be cleared
	//memset(mBufNew, '\0', sizeof(mEntry));
	MapAssoc* mBufNew = new MapAssoc[1];
	if(mBufNew == NULL)
	{
 		throw OutOfMemoryException(_T("Map::_NewAssoc(const _KTy &key), ERROR for new MapAssoc[1]."));
	}
	this->_comp.init(&(mBufNew->key));
	this->_comp.copy(&(mBufNew->key), key);
	return (void*)mBufNew;
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
int Map<_KTy, _Ty, _KPr, _Pr>::_SizeAssoc() const
{
	return sizeof(MapAssoc*);
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
void* Map<_KTy, _Ty, _KPr, _Pr>::_GetAssoc(int index) const
{
	return (void*)(*((MapAssoc**)this->_entity + index));
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
void Map<_KTy, _Ty, _KPr, _Pr>::_InsAssoc(int index, void *mBufNew)
{
	*((MapAssoc**)this->_entity + index) = (MapAssoc*)mBufNew;
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
void Map<_KTy, _Ty, _KPr, _Pr>::_MoveAssoc(int nFrom, int nTo, int nSize)
{
	memmove((MapAssoc**)this->_entity + nFrom, (MapAssoc**)this->_entity + nTo, nSize * _SizeAssoc());
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
bool Map<_KTy, _Ty, _KPr, _Pr>::_EqualsAssoc(const void* pAssoc1, const void* pAssoc2) const
{
	return (this->_comp.equals(((Assoc*)pAssoc1)->key, ((Assoc*)pAssoc2)->key, this->_bIsIgnoreCase) &&
			this->_vcomp.equals(((MapAssoc*)pAssoc1)->val, ((MapAssoc*)pAssoc2)->val, this->_bIsIgnoreCase));
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
void Map<_KTy, _Ty, _KPr, _Pr>::_DisposeAssoc(void* pAssoc)
{
	this->_comp.Dispose(&(  ((MapAssoc*)pAssoc)->key  ));
	this->_vcomp.Dispose(&(  ((MapAssoc*)pAssoc)->val  ));
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
int Map<_KTy, _Ty, _KPr, _Pr>::Add(const _KTy &key, const _Ty &val)
{
	if(this->_isDistinct)
	{
		int nIndex = GetIndex(key);
		if(nIndex > -1)
		{
			SetValueAt(nIndex, val);
			return nIndex;
		}
	}
	_mBufTmp = (MapAssoc*)_NewAssoc(key);
	this->_vcomp.init(&(_mBufTmp->val));
	this->_vcomp.copy(&(_mBufTmp->val), val);
	return _Ins(this->_nCapacity, (Assoc*)_mBufTmp);
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
int Map<_KTy, _Ty, _KPr, _Pr>::Add(Map<_KTy, _Ty, _KPr, _Pr> &map)
{
	return Add(map, 0, map.GetSize());
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
int Map<_KTy, _Ty, _KPr, _Pr>::Add(Map<_KTy, _Ty, _KPr, _Pr> &list, int fromIndex)
{
	return Add(this->map, fromIndex, this->map.GetSize() - fromIndex);
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
int Map<_KTy, _Ty, _KPr, _Pr>::Add(Map<_KTy, _Ty, _KPr, _Pr> &map, int fromIndex, int len)
{
	//if is self
	if(this->_entity == map._entity)
	{
		return 0;
	}
	if(fromIndex < 0 || len > map.GetSize() - fromIndex)
	{
 		throw ArgumentException(_T("Map::Add(&map, int fromIndex, int len), ERROR for out of Capacity."));
	}
	int cnt = 0;
	for(int i = fromIndex; i < fromIndex + len; i++)
	{
		Add(map.GetKey(i), map.GetAt(i));
		cnt++;
	}
	return cnt;
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
const _Ty& Map<_KTy, _Ty, _KPr, _Pr>::Get(const _KTy &key) const
{
	int nIndex = GetIndex(key);
	if(nIndex == -1)
	{
		//not found
 		throw ArgumentException(_T("Map::Get(const _KTy &key), ERROR for out of Capacity."));
	}

	MapAssoc *pMapAssoc = (MapAssoc*)_GetAssoc(nIndex);
	return pMapAssoc->val;
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
const _KTy& Map<_KTy, _Ty, _KPr, _Pr>::GetKey(int index) const
{
	//can be pos of end(=_nCapacity)
	if(index < 0 || index > this->_nCapacity)
	{
 		throw ArgumentException(_T("Map::GetKey(int index), ERROR for out of Capacity."));
	}

	MapAssoc *pMapAssoc = (MapAssoc*)_GetAssoc(index);
	return pMapAssoc->key;
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
_Ty& Map<_KTy, _Ty, _KPr, _Pr>::GetAt(int index) const
{
	//can be pos of end(=_nCapacity)
	if(index < 0 || index > this->_nCapacity)
	{
 		throw ArgumentException(_T("Map::Get(int index), ERROR for out of Capacity."));
	}
	MapAssoc *pMapAssoc = (MapAssoc*)_GetAssoc(index);
	return pMapAssoc->val;
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
void Map<_KTy, _Ty, _KPr, _Pr>::SetValueAt(int index, const _Ty &val)
{
	//can be pos of end(=_nCapacity)
	if(index < 0 || index > this->_nCapacity)
	{
 		throw ArgumentException(_T("Map::SetValue(int index, const _Ty &val), ERROR for out of Capacity."));
	}

	_mBufTmp = (MapAssoc*)_GetAssoc(index);
	_vcomp.copy(&(_mBufTmp->val), val);
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
void Map<_KTy, _Ty, _KPr, _Pr>::SetValue(const _KTy &key, const _Ty &val)
{
	int index = GetIndex(key);
	return SetValueAt(index, val);
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
bool Map<_KTy, _Ty, _KPr, _Pr>::ContainsValue(const _Ty &val)
{
	for(int i = 0; i < this->_nCapacity; i++)
	{
		_mBufTmp = (MapAssoc*)_GetAssoc(i);
		if(_vcomp.equals(_mBufTmp->val, val, this->_bIsIgnoreCase))
		{
			return true;
		}
	}
	return false;
}

// Compare implementations
template <class _KTy, class _Ty, class _KPr, class _Pr>
bool Map<_KTy, _Ty, _KPr, _Pr>::operator==(Map<_KTy, _Ty, _KPr, _Pr> &map) const
{
	return Equals(map);
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
bool Map<_KTy, _Ty, _KPr, _Pr>::operator!=(Map<_KTy, _Ty, _KPr, _Pr> &map) const
{
	return (!Equals(map));
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
Map<_KTy, _Ty, _KPr, _Pr>& Map<_KTy, _Ty, _KPr, _Pr>::operator=(Map<_KTy, _Ty, _KPr, _Pr> &map)
{
	//if is self
	if(&this->_entity == &map._entity)
	{
		return *this;
	}
	this->_isSort = map._isSort;
	this->_isSortDESC = map._isSortDESC;
	this->_isDistinct = map._isDistinct;
	this->_nMinCapacity = map._nMinCapacity;
	this->_nIncrement = map._nIncrement;
	this->Clear();
	Add(map, 0, map.GetSize());
	return *this;
}


///////////////////////////////////////////////////////////////////////////////
//MapTable
template <class _KTy, class _Ty,  class _KPr = CC_comp<_KTy>, class _Pr = CC_comp<_Ty> >
class MapTable : public List<_KTy, _KPr>
{
protected:
	typedef MapTable<_KTy, _Ty, _KPr, _Pr> _Myt;
	typedef _KTy key_type;
	typedef _Ty val_type;
	typedef _Pr fun_val_compare;
	fun_val_compare _vcomp; // the comparator predicate for values

#if defined __GNUC__ && __GNUC__ >= 3
	struct Assoc
	{
		_KTy key; // at Arr be treated as val,but others is key
	};
#endif
	struct MapAssoc
	{
		_KTy key;
		List<_Ty, _Pr> val;
	};

	virtual void* _NewAssoc(const _KTy &key);
	virtual int _SizeAssoc() const;
	virtual void* _GetAssoc(int nRow) const;
	virtual void _InsAssoc(int nRow, void* mBufNew);
	virtual void _MoveAssoc(int nFromRow, int nToRow, int nSize);
	virtual bool _EqualsAssoc(const void* pAssoc1, const void* pAssoc2) const;
	virtual void _DisposeAssoc(void* pAssoc);
	MapAssoc* _mBufTmp;
	List<_KTy, _KPr> _ColumnNames;
	int _nMaxCols;

private:
	//at least need MaxCols
	MapTable(){};
public:
	//constructors
	MapTable(int nMaxCols);
	MapTable(int nMaxCols, int nMinCapacity, double nIncrement = 1.3);
	MapTable(int nMaxCols, bool isSort, bool bIsDESC = false, bool isDistinct = false);
	MapTable(int nMaxCols, _Myt &list, int nMinCapacity = 0, double nIncrement = 1.3, bool isSort = false, bool bIsDESC = false, bool isDistinct = false);
	MapTable(int nMaxCols, _Myt &list, int fromIndex, int nMinCapacity = 0, double nIncrement = 1.3, bool isSort = false, bool bIsDESC = false, bool isDistinct = false);
	MapTable(int nMaxCols, _Myt &list, int fromIndex, int len, int nMinCapacity = 0, double nIncrement = 1.3, bool isSort = false, bool bIsDESC = false, bool isDistinct = false);
	virtual ~MapTable();
	//bool ContainsKey(const _KTy &key); //privade by base
	//bool ContainsValue(const _Ty &val); //not need
	//cannot add or remove item out this cope
	List<_Ty, _Pr>& AppendRow(const _KTy &key);
	List<_Ty, _Pr>& GetRow(const _KTy &key) const;
	List<_Ty, _Pr>& GetRowAt(int nRow) const;
	List<_Ty, _Pr>& Get(const _KTy &key, const _KTy &columnName) const;
	List<_Ty, _Pr>& Get(const _KTy &key, int nColumn) const;
	List<_Ty, _Pr>& GetAt(int nRow, const _KTy &columnName) const;
	List<_Ty, _Pr>& GetAt(int nRow, int nColumn) const;
	const _KTy& GetKey(int nRow) const;
	void Remove(const _KTy &key);
	void RemoveAt(int nRow);

	//========================================
	//OTHERS
	const _KTy& GetColumnName(int index) const;
	const _KTy& SetColumnName(int index, const _KTy &columnName) const;

private:
	//for hide base's SetKey
	void SetKey(int index, const _KTy &key){};
	_Ty& GetAt(int index) const{};
	const _Ty& Get(const _KTy &key) const{};

};

template <class _KTy, class _Ty, class _KPr, class _Pr>
MapTable<_KTy, _Ty, _KPr, _Pr>::MapTable(int nMaxCols) : List<_KTy, _KPr>()
{
	_nMaxCols = nMaxCols;
	for(int i = 0; i < _nMaxCols; i++)
	{
	}
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
MapTable<_KTy, _Ty, _KPr, _Pr>::MapTable(int nMaxCols, int nMinCapacity, double nIncrement)
	: List<_KTy, _KPr>(nMinCapacity, nIncrement)
{
	_nMaxCols = nMaxCols;
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
MapTable<_KTy, _Ty, _KPr, _Pr>::MapTable(int nMaxCols, bool isSort, bool bIsDESC , bool isDistinct)
	: List<_KTy, _KPr>(isSort, bIsDESC, isDistinct)
{
	_nMaxCols = nMaxCols;
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
MapTable<_KTy, _Ty, _KPr, _Pr>::MapTable(int nMaxCols, MapTable<_KTy, _Ty, _KPr, _Pr> &list, int nMinCapacity, double nIncrement, bool isSort, bool bIsDESC, bool isDistinct)
	: List<_KTy, _KPr>(isSort, bIsDESC, isDistinct)
{
	_nMaxCols = nMaxCols;
	//SetCapacity(nMinCapacity, nIncrement);
	if(nMinCapacity > 0)
	{
		this->_nMinCapacity = nMinCapacity;
	}
	if(nIncrement > 1.0)
	{
		this->_nIncrement = nIncrement;
	}
	Add(list);
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
MapTable<_KTy, _Ty, _KPr, _Pr>::MapTable(int nMaxCols, MapTable<_KTy, _Ty, _KPr, _Pr> &list, int fromIndex, int nMinCapacity, double nIncrement, bool isSort, bool bIsDESC, bool isDistinct)
	: List<_KTy, _KPr>(isSort, bIsDESC, isDistinct)
{
	_nMaxCols = nMaxCols;
	//SetCapacity(nMinCapacity, nIncrement);
	if(nMinCapacity > 0)
	{
		this->_nMinCapacity = nMinCapacity;
	}
	if(nIncrement > 1.0)
	{
		this->_nIncrement = nIncrement;
	}
	Add(list, fromIndex);
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
MapTable<_KTy, _Ty, _KPr, _Pr>::MapTable(int nMaxCols, MapTable<_KTy, _Ty, _KPr, _Pr> &list, int fromIndex, int len, int nMinCapacity, double nIncrement, bool isSort, bool bIsDESC, bool isDistinct)
	: List<_KTy, _KPr>(isSort, bIsDESC, isDistinct)
{
	_nMaxCols = nMaxCols;
	//SetCapacity(nMinCapacity, nIncrement);
	if(nMinCapacity > 0)
	{
		this->_nMinCapacity = nMinCapacity;
	}
	if(nIncrement > 1.0)
	{
		this->_nIncrement = nIncrement;
	}
	Add(list, fromIndex, len);
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
MapTable<_KTy, _Ty, _KPr, _Pr>::~MapTable()
{
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
void* MapTable<_KTy, _Ty, _KPr, _Pr>::_NewAssoc(const _KTy &key)
{
	//for not auto destructor,not use malloc
	//mEntry* mBufNew = (mEntry*)malloc(sizeof(mEntry));
	//if memset to '\0',then constructors info will be cleared
	//memset(mBufNew, '\0', sizeof(mEntry));
	MapAssoc* mBufNew = new MapAssoc[1];
	if(mBufNew == NULL)
	{
 		throw OutOfMemoryException(_T("MapTable::_NewAssoc(const _KTy &key), ERROR for new MapAssoc[1]."));
	}
	this->_comp.init(&(mBufNew->key));
	this->_comp.copy(&(mBufNew->key), key);
	List<_Ty, _Pr> val = mBufNew->val;
	return (void*)mBufNew;
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
int MapTable<_KTy, _Ty, _KPr, _Pr>::_SizeAssoc() const
{
	return sizeof(MapAssoc*);
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
void* MapTable<_KTy, _Ty, _KPr, _Pr>::_GetAssoc(int index) const
{
	return (void*)(*((MapAssoc**)this->_entity + index));
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
void MapTable<_KTy, _Ty, _KPr, _Pr>::_InsAssoc(int index, void *mBufNew)
{
	*((MapAssoc**)this->_entity + index) = (MapAssoc*)mBufNew;
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
void MapTable<_KTy, _Ty, _KPr, _Pr>::_MoveAssoc(int nFrom, int nTo, int nSize)
{
	memmove((MapAssoc**)this->_entity + nFrom, (MapAssoc**)this->_entity + nTo, nSize * _SizeAssoc());
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
bool MapTable<_KTy, _Ty, _KPr, _Pr>::_EqualsAssoc(const void* pAssoc1, const void* pAssoc2) const
{
	return (this->_comp.equals(((MapAssoc*)pAssoc1)->key, ((MapAssoc*)pAssoc2)->key, this->_bIsIgnoreCase));
	//return (_comp.equals(((Assoc*)pAssoc1)->key, ((Assoc*)pAssoc2)->key) &&
	//	_vcomp.equals(((MapAssoc*)pAssoc1)->val, ((MapAssoc*)pAssoc2)->val));
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
void MapTable<_KTy, _Ty, _KPr, _Pr>::_DisposeAssoc(void* pAssoc)
{
	this->_comp.Dispose(&(  ((MapAssoc*)pAssoc)->key  ));
	//_vcomp.Dispose(&(  ((MapAssoc*)pAssoc)->val  ));
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
List<_Ty, _Pr>& MapTable<_KTy, _Ty, _KPr, _Pr>::AppendRow(const _KTy &key)
{
	if(this->_isDistinct)
	{
		int nRow = GetIndex(key);
		if(nRow != -1)
		{
			MapAssoc *pMapAssoc = (MapAssoc*)_GetAssoc(nRow);
			return pMapAssoc->val;
		}
	}

	_mBufTmp = (MapAssoc*)_NewAssoc(key);
	List<_Ty, _Pr> val = _mBufTmp->val;
	for(int i = 0; i < _nMaxCols; i++)
	{
	}
	_Ins(this->_nCapacity, (Assoc*)_mBufTmp);
	return _mBufTmp->val;
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
List<_Ty, _Pr>& MapTable<_KTy, _Ty, _KPr, _Pr>::GetRow(const _KTy &key) const
{
	int nIndex = GetIndex(key);
	if(nIndex == -1)
	{
		//not found
 		throw ArgumentException(_T("Map::GetRow(const _KTy &key), ERROR for out of Capacity."));
	}
	MapAssoc *pMapAssoc = (MapAssoc*)_GetAssoc(nIndex);
	return pMapAssoc->val;
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
List<_Ty, _Pr>& MapTable<_KTy, _Ty, _KPr, _Pr>::GetRowAt(int nRow) const
{
	//can be pos of end(=_nCapacity)
	if(nRow < 0 || nRow > this->_nCapacity)
	{
 		throw ArgumentException(_T("Map::GetRow(int nRow), ERROR for out of Capacity."));
	}
	MapAssoc *pMapAssoc = (MapAssoc*)_GetAssoc(nRow);
	return pMapAssoc->val;
}

template <class _KTy, class _Ty, class _KPr, class _Pr>
const _KTy& MapTable<_KTy, _Ty, _KPr, _Pr>::GetKey(int nRow) const
{
	//can be pos of end(=_nCapacity)
	if(nRow < 0 || nRow > this->_nCapacity)
	{
 		throw ArgumentException(_T("Map::GetKey(int nRow), ERROR for out of Capacity."));
	}
	const MapAssoc *pMapAssoc = (MapAssoc*)_GetAssoc(nRow);
	return pMapAssoc->key;
}

} //namespace cc

#endif //#ifndef CC_List_H_
