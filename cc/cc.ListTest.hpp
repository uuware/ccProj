/* cc.ListTest.hpp
   Test for cc.List
   Author: Shu.KK
   created:2005-01-01
   change to new format:2006-01-10,2006-07-29
*/
#ifndef cc_ListTest_H_
#define cc_ListTest_H_

#include "cc.h"
#include "cc.Str.h"
#include "cc.Test.h"
#include "cc.List.h"

namespace cc
{

//////////////////////// class fot test of List START ///////////////////

//this class has three item,but KEY is key1+key2
class ListTestUserData
{
protected:
	int key1;
	double key2;
	int key3;

public:
	void setKey1(int key){key1 = key;};
	const int getKey1() const {return key1;};
	void setKey2(double key){key2 = key;};
	const double getKey2() const {return key2;};
	void setKey3(int key){key3 = key;};
	const int getKey3() const {return key3;};

public:
	ListTestUserData(){};
	virtual ~ListTestUserData(){};
	ListTestUserData &operator=(const ListTestUserData &da)
	{
		key1 = da.getKey1();
		key2 = da.getKey2();
		key3 = da.getKey3();
		return *this;
	};
	const bool operator==(const ListTestUserData &da) const
	{
		//this class,if key1+key2 is equals,then equals
		return (key1 == da.getKey1() && key2 == da.getKey2());
	};
	const bool operator<(const ListTestUserData &da) const
	{
		return (key1 < da.getKey1() && key2 < da.getKey2());
	};
};
	
class ListTest : public cc::Test
{
public:
	ListTest()
	{
		TESTCLASS_COMMENT("test of cc::List");
	}

	void Setup()
	{
	}

	void TearDown()
	{
	}

	void TestCase()
	{
		TESTCASE_BEGIN("test constructors:List<int> listI; and List<int> *plistI(need call Clear() manaual.);")
		{
			//this time create listI1
			//can not use in Dev-C++:List<int> listI1 = List<int>();
			List<int> listI1;
			TESTASSERT_EQUAL(0, listI1.GetSize());
			TESTASSERT_TRUE(listI1.IsEmpty());
			TESTASSERT_EQUAL(0, listI1.GetMinCapacity());
			TESTASSERT_EQUAL(1.3, listI1.GetIncrement());
			TESTASSERT_EQUAL(false, listI1.IsDistinct());
			TESTASSERT_EQUAL(false, listI1.IsSort());
			TESTASSERT_EQUAL(false, listI1.IsSortDESC());

			//this time create listI2
			List<int> listI2(10, 1.6);
			listI2.Add(111);
			TESTASSERT_EQUAL(1, listI2.GetSize());
			TESTASSERT_FALSE(listI2.IsEmpty());
			TESTASSERT_EQUAL(10, listI2.GetMinCapacity());
			TESTASSERT_EQUAL(1.6, listI2.GetIncrement());
			TESTASSERT_EQUAL(false, listI1.IsDistinct());
			TESTASSERT_EQUAL(false, listI1.IsSort());
			TESTASSERT_EQUAL(false, listI1.IsSortDESC());

			//this time create listI3
			List<int> listI3(true, true, true);
			listI3.Add(222);
			listI3.Add(111);
			TESTASSERT_EQUAL(2, listI3.GetSize());
			TESTASSERT_FALSE(listI3.IsEmpty());
			TESTASSERT_EQUAL(0, listI3.GetMinCapacity());
			TESTASSERT_EQUAL(1.3, listI3.GetIncrement());
			TESTASSERT_EQUAL(true, listI3.IsDistinct());
			TESTASSERT_EQUAL(true, listI3.IsSort());
			TESTASSERT_EQUAL(true, listI3.IsSortDESC());

			//this time create *listI4,not forget to release it after used it
			List<int> *listI4 = new List<int>();
			listI4->Add(222);
			listI4->Add(111);
			listI4->Add(333);
			TESTASSERT_EQUAL(3, listI4->GetSize());
			//listI4 is not auto destructor after this cope!!
			listI4->Clear();
			TESTASSERT_EQUAL(0, listI4->GetSize());
			TESTASSERT_TRUE(listI4->IsEmpty());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test constructors:List(List<_KTy, _KPr> &list, ...) 1/2;")
		{
			List<int> listI1;
			listI1.Add(222);
			listI1.Add(444);
			listI1.Add(333);
			listI1.Add(111);
			listI1.Add(333);

			List<int> listI2(listI1, true, true, true, 10, 1.5);
			TESTASSERT_EQUAL(4, listI2.GetSize());
			TESTASSERT_FALSE(listI2.IsEmpty());
			TESTASSERT_EQUAL(10, listI2.GetMinCapacity());
			TESTASSERT_EQUAL(1.5, listI2.GetIncrement());
			TESTASSERT_EQUAL(true, listI2.IsDistinct());
			TESTASSERT_EQUAL(true, listI2.IsSort());
			TESTASSERT_EQUAL(true, listI2.IsSortDESC());
			TESTASSERT_EQUAL(444, listI2.GetKey(0));
			TESTASSERT_EQUAL(333, listI2.GetKey(1));
			TESTASSERT_EQUAL(222, listI2.GetKey(2));
			TESTASSERT_EQUAL(111, listI2.GetKey(3));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test constructors:List(List<_KTy, _KPr> &list, ...) 2/2;")
		{
			List<int> listI1;
			listI1.Add(222);
			listI1.Add(444);
			listI1.Add(333);
			listI1.Add(111);
			listI1.Add(333);

			List<int> listI2(listI1, 1, 3);
			TESTASSERT_EQUAL(3, listI2.GetSize());
			TESTASSERT_FALSE(listI2.IsEmpty());
			TESTASSERT_EQUAL(0, listI2.GetMinCapacity());
			TESTASSERT_EQUAL(1.3, listI2.GetIncrement());
			TESTASSERT_EQUAL(false, listI2.IsDistinct());
			TESTASSERT_EQUAL(false, listI2.IsSort());
			TESTASSERT_EQUAL(false, listI2.IsSortDESC());
			TESTASSERT_EQUAL(444, listI2.GetKey(0));
			TESTASSERT_EQUAL(333, listI2.GetKey(1));
			TESTASSERT_EQUAL(111, listI2.GetKey(2));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:List(int nMinCapacity, double nIncrement = 1.4)")
		{
			//this time change MinCapacity&Increment
			List<int> listI1(10, 1.6);
			TESTASSERT_EQUAL(10, listI1.GetMinCapacity());
			TESTASSERT_EQUAL(1.6, listI1.GetIncrement());

			//this time not change MinCapacity&Increment
			List<int> listI2(-1, 0.9);
			TESTASSERT_EQUAL(0, listI2.GetMinCapacity());
			TESTASSERT_EQUAL(1.3, listI2.GetIncrement());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:List(bool isSort, bool bIsDESC = false, bool isDistinct = false)")
		{
			//without sort
			List<int> listI1;
			listI1.Add(222);
			listI1.Add(111);
			listI1.Add(333);
			TESTASSERT_EQUAL(false, listI1.IsDistinct());
			TESTASSERT_EQUAL(false, listI1.IsSort());
			TESTASSERT_EQUAL(false, listI1.IsSortDESC());
			TESTASSERT_EQUAL(3, listI1.GetSize());
			TESTASSERT_EQUAL(222, listI1.GetKey(0));
			TESTASSERT_EQUAL(111, listI1.GetKey(1));
			TESTASSERT_EQUAL(333, listI1.GetKey(2));

			//sort and !desc
			List<int> listI2(true);
			listI2.Add(222);
			listI2.Add(444);
			listI2.Add(333);
			listI2.Add(111);
			listI2.Add(333);
			TESTASSERT_EQUAL(false, listI2.IsDistinct());
			TESTASSERT_EQUAL(true, listI2.IsSort());
			TESTASSERT_EQUAL(false, listI2.IsSortDESC());
			TESTASSERT_EQUAL(5, listI2.GetSize());
			TESTASSERT_EQUAL(111, listI2.GetKey(0));
			TESTASSERT_EQUAL(222, listI2.GetKey(1));
			TESTASSERT_EQUAL(333, listI2.GetKey(2));
			TESTASSERT_EQUAL(333, listI2.GetKey(3));
			TESTASSERT_EQUAL(444, listI2.GetKey(4));

			//sort and desc, and with distinct
			List<int> listI3(true, true, true);
			listI3.Add(222);
			listI3.Add(444);
			listI3.Add(333);
			listI3.Add(111);
			listI3.Add(333);
			TESTASSERT_EQUAL(true, listI3.IsDistinct());
			TESTASSERT_EQUAL(true, listI3.IsSort());
			TESTASSERT_EQUAL(true, listI3.IsSortDESC());
			TESTASSERT_EQUAL(4, listI3.GetSize());
			TESTASSERT_EQUAL(444, listI3.GetKey(0));
			TESTASSERT_EQUAL(333, listI3.GetKey(1));
			TESTASSERT_EQUAL(222, listI3.GetKey(2));
			TESTASSERT_EQUAL(111, listI3.GetKey(3));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test char&wchar_t:List(bool isSort, bool bIsDESC = false, bool isDistinct = false)")
		{
			//sort and desc, and with distinct
			List<char *> listC1(true, true, true);
			listC1.Add(("222"));
			listC1.Add(("444"));
			listC1.Add(("333"));
			listC1.Add(("111"));
			listC1.Add(("333"));
			TESTASSERT_EQUAL(true, listC1.IsDistinct());
			TESTASSERT_EQUAL(true, listC1.IsSort());
			TESTASSERT_EQUAL(true, listC1.IsSortDESC());
			TESTASSERT_EQUAL(4, listC1.GetSize());
			TESTASSERT_EQUAL(("444"), listC1.GetKey(0));
			TESTASSERT_EQUAL(("333"), listC1.GetKey(1));
			TESTASSERT_EQUAL(("222"), listC1.GetKey(2));
			TESTASSERT_EQUAL(("111"), listC1.GetKey(3));

			//sort and desc, and with distinct
			List<wchar_t *> listC2(true, true, true);
			listC2.Add(L"222");
			listC2.Add(L"444");
			listC2.Add(L"333");
			listC2.Add(L"111");
			listC2.Add(L"333");
			TESTASSERT_EQUAL(true, listC2.IsDistinct());
			TESTASSERT_EQUAL(true, listC2.IsSort());
			TESTASSERT_EQUAL(true, listC2.IsSortDESC());
			TESTASSERT_EQUAL(4, listC2.GetSize());
			TESTASSERT_EQUAL(L"444", listC2.GetKey(0));
			TESTASSERT_EQUAL(L"333", listC2.GetKey(1));
			TESTASSERT_EQUAL(L"222", listC2.GetKey(2));
			TESTASSERT_EQUAL(L"111", listC2.GetKey(3));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test Add:Add(List<_KTy, _KPr> &list, ...); 1/2")
		{
			List<int> listI1;
			listI1.Add(222);
			listI1.Add(444);
			listI1.Add(333);
			listI1.Add(111);
			listI1.Add(333);

			List<int> listI2;
			listI2.Add(listI1);
			TESTASSERT_EQUAL(5, listI2.GetSize());
			TESTASSERT_EQUAL(222, listI2.GetKey(0));
			TESTASSERT_EQUAL(444, listI2.GetKey(1));
			TESTASSERT_EQUAL(333, listI2.GetKey(2));
			TESTASSERT_EQUAL(111, listI2.GetKey(3));
			TESTASSERT_EQUAL(333, listI2.GetKey(4));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test Add:Add(List<_KTy, _KPr> &list, ...); 2/2")
		{
			List<int> listI1;
			listI1.Add(222);
			listI1.Add(444);
			listI1.Add(333);
			listI1.Add(111);
			listI1.Add(333);

			List<int> listI2;
			listI2.Add(listI1, 1, 2);
			TESTASSERT_EQUAL(2, listI2.GetSize());
			TESTASSERT_EQUAL(444, listI2.GetKey(0));
			TESTASSERT_EQUAL(333, listI2.GetKey(1));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test GetKey/SetAt/GetIndex;")
		{
			//sort and desc, and with distinct
			List<TCHAR *> listC1;
			listC1.Add(_T("222"));
			listC1.Add(_T("444"));
			listC1.Add(_T("333"));

			TESTASSERT_EQUAL(3, listC1.GetSize());
			TESTASSERT_EQUAL(_T("444"), listC1.GetKey(1));
			listC1.SetKey(1, _T("new 444"));
			TESTASSERT_EQUAL(_T("new 444"), listC1.GetKey(1));

			TESTASSERT_EQUAL(2, listC1.GetIndex(_T("333")));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of:int Insert(int index, const _KTy &key);")
		{
			//sort and desc, and with distinct
			List<TCHAR *> listC1;
			listC1.Add(_T("222"));
			listC1.Add(_T("444"));
			listC1.Add(_T("333"));

			TESTASSERT_EQUAL(3, listC1.GetSize());
			listC1.Insert(1, _T("new to 1111"));
			TESTASSERT_EQUAL(4, listC1.GetSize());
			TESTASSERT_EQUAL(_T("new to 1111"), listC1.GetKey(1));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of:void Remove(const _KTy &key);/void RemoveAt(int index);")
		{
			//sort and desc, and with distinct
			List<TCHAR *> listC1;
			listC1.Add(_T("222"));
			listC1.Add(_T("444"));
			listC1.Add(_T("333"));
			listC1.Add(_T("111"));

			TESTASSERT_EQUAL(4, listC1.GetSize());
			listC1.Remove(_T("444"));
			TESTASSERT_EQUAL(3, listC1.GetSize());
			TESTASSERT_EQUAL(_T("333"), listC1.GetKey(1));
			listC1.RemoveAt(1);
			TESTASSERT_EQUAL(2, listC1.GetSize());
			TESTASSERT_EQUAL(_T("111"), listC1.GetKey(1));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of:Clear/SetCapacity/ContainsKey")
		{
			//sort and desc, and with distinct
			List<TCHAR *> listC1;
			listC1.Add(_T("444"));
			listC1.Add(_T("222"));
			listC1.Add(_T("222"));
			listC1.Add(_T("111"));

			TESTASSERT_EQUAL(4, listC1.GetSize());
			listC1.Clear();
			TESTASSERT_TRUE(listC1.IsEmpty());
			TESTASSERT_EQUAL(0, listC1.GetSize());

			listC1.Add(_T("444"));
			listC1.Add(_T("222"));
			listC1.Add(_T("333"));
			listC1.Add(_T("111"));
			listC1.SetCapacity(10, 1.5);
			TESTASSERT_EQUAL(10, listC1.GetMinCapacity());
			TESTASSERT_EQUAL(1.5, listC1.GetIncrement());
			TESTASSERT_TRUE(listC1.ContainsKey(_T("222")));
			TESTASSERT_FALSE(listC1.ContainsKey(_T("not exist")));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of:Equals/operator=/operator==/operator!=")
		{
			List<TCHAR *> listC1;
			listC1.SetCapacity(10, 1.5);
			listC1.Add(_T("444"));
			listC1.Add(_T("222"));
			listC1.Add(_T("333"));
			listC1.Add(_T("111"));

			List<TCHAR *> listC2;
			listC1.SetCapacity(20, 2.0);
			listC2.Add(_T("444"));
			listC2.Add(_T("222"));
			listC2.Add(_T("222"));
			listC2.Add(_T("111"));
			TESTASSERT_FALSE(listC2.Equals(listC1));
			listC2.SetKey(2, _T("333"));
			TESTASSERT_TRUE(listC2.Equals(listC1));

			List<TCHAR *> listC3 = listC1;
			//MinCapacity&Increment not change by "="
			TESTASSERT_EQUAL(0, listC3.GetMinCapacity());
			TESTASSERT_EQUAL(1.3, listC3.GetIncrement());
			TESTASSERT_TRUE(listC3 == listC1);
			TESTASSERT_FALSE(listC3 != listC1);

			listC3 = listC2;
			//MinCapacity&Increment not change by "="
			TESTASSERT_EQUAL(0, listC3.GetMinCapacity());
			TESTASSERT_EQUAL(1.3, listC3.GetIncrement());
			TESTASSERT_TRUE(listC3 == listC2);
			TESTASSERT_FALSE(listC3 != listC2);

			//the "=" only diff key&val
			listC3.SetCapacity(30, 3.0);
			TESTASSERT_TRUE(listC3 == listC2);
			TESTASSERT_FALSE(listC3 != listC2);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of:operator=&sort it")
		{
			List<TCHAR *> listC1;
			listC1.Add(_T("444"));
			listC1.Add(_T("222"));
			listC1.Add(_T("333"));
			listC1.Add(_T("111"));

			List<TCHAR *> listC2(true);
			listC2 = listC1;
			//this not equals
			TESTASSERT_FALSE(listC2 == listC1);
			TESTASSERT_TRUE(listC2 != listC1);
			TESTASSERT_EQUAL(_T("111"), listC2.GetKey(0));
			TESTASSERT_EQUAL(_T("222"), listC2.GetKey(1));
			TESTASSERT_EQUAL(_T("333"), listC2.GetKey(2));
			TESTASSERT_EQUAL(_T("444"), listC2.GetKey(3));

			List<TCHAR *> listC3(true, true);
			listC3 = listC1;
			//this not equals
			TESTASSERT_FALSE(listC3 == listC1);
			TESTASSERT_TRUE(listC3 != listC1);
			TESTASSERT_EQUAL(_T("444"), listC3.GetKey(0));
			TESTASSERT_EQUAL(_T("333"), listC3.GetKey(1));
			TESTASSERT_EQUAL(_T("222"), listC3.GetKey(2));
			TESTASSERT_EQUAL(_T("111"), listC3.GetKey(3));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:Performance,pay attation to the time of treated with large monut SORT item")
		{
			int loopcnt = 10000;
			cc::Log::traceConsole(RGB(255, 0, 0), _T("==============================\r\n"));
			cc::Log::debug(_T("add %d int item((int)(rand() * 10000) and sort of it"), loopcnt);
			List<int> listI1(true);
			int i;

			cc::Log::traceConsole(RGB(255, 0, 0), _T("start adding..."));
			clock_t tmStart = ::clock ();
			for(i = 0; i < loopcnt; i++)
			{
				listI1.Add((int)(rand() * 10000));
			}
			cc::Log::traceConsole(RGB(255, 0, 0), _T("end.\r\n"));
			cc::Log::debug(_T("size of list:(%d)"), listI1.GetSize());
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);

			//time of seek data
			cc::Log::debug(_T("seek data of middle&end:"));
			tmStart = ::clock ();
			cc::Log::debug(_T("listI1.GetKey(%d):%d"), loopcnt/2, listI1.GetKey(loopcnt/2));
			cc::Log::debug(_T("listI1.GetKey(%d):%d"), loopcnt - 1, listI1.GetKey(loopcnt - 1));
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);

			//it is need time to clear
			cc::Log::debug(_T("it is need time to clear,now clear..."));
			tmStart = ::clock ();
			listI1.Clear();
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:Performance,pay attation to the time of treated with large monut item")
		{
			int loopcnt = 10000;
			cc::Log::traceConsole(RGB(255, 0, 0), _T("==============================\r\n"));
			cc::Log::debug(_T("add %d int item((int)(rand() * 10000)) without of sort"), loopcnt);
			List<int> listI1;
			int i;

			cc::Log::traceConsole(RGB(255, 0, 0), _T("start adding..."));
			clock_t tmStart = ::clock ();
			for(i = 0; i < loopcnt; i++)
			{
				listI1.Add((int)(rand() * 10000));
			}
			cc::Log::traceConsole(RGB(255, 0, 0), _T("end.\r\n"));
			cc::Log::debug(_T("size of list:(%d)"), listI1.GetSize());
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);

			//time of seek data
			cc::Log::debug(_T("seek data of middle&end:"));
			tmStart = ::clock ();
			cc::Log::debug(_T("listI1.GetKey(%d):%d"), loopcnt/2, listI1.GetKey(loopcnt/2));
			cc::Log::debug(_T("listI1.GetKey(%d):%d"), loopcnt - 1, listI1.GetKey(loopcnt - 1));
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);

			//it is need time to clear
			cc::Log::debug(_T("it is need time to clear,now clear..."));
			tmStart = ::clock ();
			listI1.Clear();
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:user defined data(entity class[ListTestUserData])")
		{
			//first define this entity
			//pay attation to that key is:key1+key2
			ListTestUserData userData1 = ListTestUserData();
			userData1.setKey1(3);
			userData1.setKey2(3.1);
			userData1.setKey3(33);

			ListTestUserData userData2 = ListTestUserData();
			userData2.setKey1(1);
			userData2.setKey2(1.1);
			userData2.setKey3(99);

			ListTestUserData userData3 = ListTestUserData();
			userData3.setKey1(2);
			userData3.setKey2(2.1);
			userData3.setKey3(22);

			ListTestUserData userData4 = ListTestUserData();
			userData4.setKey1(1);
			userData4.setKey2(1.1);
			userData4.setKey3(88);

			//without sort
			cc::List<ListTestUserData> list;
			list.Add(userData1);
			list.Add(userData2);
			list.Add(userData3);
			list.Add(userData4);
			TESTASSERT_EQUAL(4, list.GetSize());
			TESTASSERT_EQUAL(3, list.GetKey(0).getKey1());
			TESTASSERT_EQUAL(3.1, list.GetKey(0).getKey2());
			TESTASSERT_EQUAL(33, list.GetKey(0).getKey3());
			TESTASSERT_EQUAL(1, list.GetKey(3).getKey1());
			TESTASSERT_EQUAL(1.1, list.GetKey(3).getKey2());
			TESTASSERT_EQUAL(88, list.GetKey(3).getKey3());

			//now with sort
			cc::List<ListTestUserData> list2(list, true);
			TESTASSERT_EQUAL(4, list2.GetSize());
			TESTASSERT_EQUAL(1, list2.GetKey(0).getKey1());
			TESTASSERT_EQUAL(1.1, list2.GetKey(0).getKey2());
			TESTASSERT_EQUAL(99, list2.GetKey(0).getKey3());
			TESTASSERT_EQUAL(1, list2.GetKey(1).getKey1());
			TESTASSERT_EQUAL(1.1, list2.GetKey(1).getKey2());
			TESTASSERT_EQUAL(88, list2.GetKey(1).getKey3());
			TESTASSERT_EQUAL(2, list2.GetKey(2).getKey1());
			TESTASSERT_EQUAL(2.1, list2.GetKey(2).getKey2());
			TESTASSERT_EQUAL(22, list2.GetKey(2).getKey3());
			TESTASSERT_EQUAL(3, list2.GetKey(3).getKey1());
			TESTASSERT_EQUAL(3.1, list2.GetKey(3).getKey2());
			TESTASSERT_EQUAL(33, list2.GetKey(3).getKey3());
		}
		TESTCASE_END;

		#ifdef cc_Str_H_
		TESTCASE_BEGIN("test:cc::List<cc::Str*> listPStr")
		{
			//if use pointer,then if sort,only compare with addres
			//but it is efficiency without copy of Str and new&destructor of Str
			cc::Str* str1 = new cc::Str(_T("3:911"));
			cc::Str* str2 = new cc::Str(_T("2:922"));
			cc::List<cc::Str*> listPStr;
			listPStr.Add(str1);
			listPStr.Add(str2);

			TESTASSERT_EQUAL(2, listPStr.GetSize());
			TESTASSERT_EQUAL(_T("3:911"), (TCHAR*)listPStr.GetKey(0)->GetBuf());
			TESTASSERT_EQUAL(_T("2:922"), (TCHAR*)listPStr.GetKey(1)->GetBuf());

			//not forget this!!
			for(int n = 0; n < listPStr.GetSize(); n++)
			{
				delete listPStr.GetKey(n);
			}
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:Arr<cc::Str> arrStr")
		{
			//while set to Arr or get from Arr,then new Str each once!
			cc::List<cc::Str> listPStr;
			if(true)
			{
				//if out of this scope,str1&str2 will disposed auto
				//but the value of copy is exist in listPStr
				cc::Str str1 = _T("3:911");
				cc::Str str2 = _T("2:922");
				listPStr.Add(str1);
				listPStr.Add(str2);
				//now even change str1,the value of listPStr is not change
				str1.ValueOf(_T("x:xxx"));
			}

			TESTASSERT_EQUAL(2, listPStr.GetSize());
			TESTASSERT_EQUAL(_T("3:911"), (TCHAR*)listPStr.GetKey(0).GetBuf());
			TESTASSERT_EQUAL(_T("2:922"), (TCHAR*)listPStr.GetKey(1).GetBuf());

			cc::List<cc::Str> listPStr2(listPStr, true);
			cc::Str str3 = _T("5:99");
			cc::Str str4 = _T("2:99");
			cc::Str str5 = _T("1:99");
			listPStr2.Add(str3);
			listPStr2.Add(str4);
			listPStr2.Add(str5);
			TESTASSERT_EQUAL(5, listPStr2.GetSize());
			TESTASSERT_EQUAL(_T("1:99"), (TCHAR*)listPStr2.GetKey(0).GetBuf());
			TESTASSERT_EQUAL(_T("2:922"), (TCHAR*)listPStr2.GetKey(1).GetBuf());
			TESTASSERT_EQUAL(_T("2:99"), (TCHAR*)listPStr2.GetKey(2).GetBuf());
			TESTASSERT_EQUAL(_T("3:911"), (TCHAR*)listPStr2.GetKey(3).GetBuf());
			TESTASSERT_EQUAL(_T("5:99"), (TCHAR*)listPStr2.GetKey(4).GetBuf());
		}
		TESTCASE_END;
		#endif //#ifdef cc_Str_H_

	};
};

class MapTest : public cc::Test
{
public:
	MapTest()
	{
		TESTCLASS_COMMENT("test of cc::Map");
	}

	void Setup()
	{
	}

	void TearDown()
	{
	}

	void TestCase()
	{
		TESTCASE_BEGIN("test constructors:Map<int, char*> map;")
		{
			//
			Map<int, char*> map;
			TESTASSERT_EQUAL(0, map.GetSize());
			TESTASSERT_TRUE(map.IsEmpty());
			TESTASSERT_EQUAL(0, map.GetMinCapacity());
			TESTASSERT_EQUAL(1.3, map.GetIncrement());
			TESTASSERT_EQUAL(false, map.IsDistinct());
			TESTASSERT_EQUAL(false, map.IsSort());
			TESTASSERT_EQUAL(false, map.IsSortDESC());

			//this time create map2
			Map<int, char*> map2(10, 1.5);
			map2.Add(1,"11");
			TESTASSERT_EQUAL(1, map2.GetSize());
			TESTASSERT_FALSE(map2.IsEmpty());
			TESTASSERT_EQUAL(10, map2.GetMinCapacity());
			TESTASSERT_EQUAL(1.5, map2.GetIncrement());
			TESTASSERT_EQUAL(false, map2.IsDistinct());
			TESTASSERT_EQUAL(false, map2.IsSort());
			TESTASSERT_EQUAL(false, map2.IsSortDESC());

			//this time create map3
			Map<int, char*> map3(true, true, true);
			map3.Add(1,"11");
			map3.Add(2,"22");
			TESTASSERT_EQUAL(2, map3.GetSize());
			TESTASSERT_FALSE(map3.IsEmpty());
			TESTASSERT_EQUAL(0, map3.GetMinCapacity());
			TESTASSERT_EQUAL(1.3, map3.GetIncrement());
			TESTASSERT_EQUAL(true, map3.IsDistinct());
			TESTASSERT_EQUAL(true, map3.IsSort());
			TESTASSERT_EQUAL(true, map3.IsSortDESC());

			//this time create *map4,not forget to release it after used it
			Map<int, char*> *map4 = new Map<int, char*>();
			map4->Add(1,"11");
			map4->Add(2,"22");
			map4->Add(3,"33");
			TESTASSERT_EQUAL(3, map4->GetSize());
			//map4 is not auto destructor after this cope!!
			map4->Clear();
			TESTASSERT_EQUAL(0, map4->GetSize());
			TESTASSERT_TRUE(map4->IsEmpty());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test char&wchar_t:List(bool isSort, bool bIsDESC = false, bool isDistinct = false)")
		{
			//sort and desc, and with distinct
			Map<int, char *> mapC1(true, true, true);
			mapC1.Add(222, "for 222 first");
			mapC1.Add(111, "for 111");
			mapC1.Add(444, "for 444");
			mapC1.Add(222, "for 222 second");
			mapC1.Add(333, "for 333");
			TESTASSERT_EQUAL(true, mapC1.IsDistinct());
			TESTASSERT_EQUAL(true, mapC1.IsSort());
			TESTASSERT_EQUAL(true, mapC1.IsSortDESC());
			TESTASSERT_EQUAL(4, mapC1.GetSize());
			TESTASSERT_EQUAL(444, mapC1.GetKey(0));
			TESTASSERT_EQUAL(333, mapC1.GetKey(1));
			TESTASSERT_EQUAL(222, mapC1.GetKey(2));
			TESTASSERT_EQUAL(111, mapC1.GetKey(3));

			TESTASSERT_EQUAL(("for 444"), mapC1.GetAt(0));
			TESTASSERT_EQUAL(("for 333"), mapC1.GetAt(1));
			TESTASSERT_EQUAL(("for 222 second"), mapC1.GetAt(2));
			TESTASSERT_EQUAL(("for 111"), mapC1.GetAt(3));

			//sort and desc, and with distinct
			Map<int, wchar_t *> mapC2(true, false, false);
			mapC2.Add(222, L"for 222 first");
			mapC2.Add(111, L"for 111");
			mapC2.Add(444, L"for 444");
			mapC2.Add(222, L"for 222 second");
			mapC2.Add(333, L"for 333");
			TESTASSERT_EQUAL(false, mapC2.IsDistinct());
			TESTASSERT_EQUAL(true, mapC2.IsSort());
			TESTASSERT_EQUAL(false, mapC2.IsSortDESC());
			TESTASSERT_EQUAL(5, mapC2.GetSize());
			TESTASSERT_EQUAL(L"for 111", mapC2.GetAt(0));
			TESTASSERT_EQUAL(L"for 222 first", mapC2.GetAt(1));
			TESTASSERT_EQUAL(L"for 222 second", mapC2.GetAt(2));
			TESTASSERT_EQUAL(L"for 333", mapC2.GetAt(3));
			TESTASSERT_EQUAL(L"for 444", mapC2.GetAt(4));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of:ContainsKey/ContainsValue")
		{
			//sort and desc, and with distinct
			Map<TCHAR*, int> mapC1;
			mapC1.Add(_T("for 44"), 44);
			mapC1.Add(_T("for 22"), 22);
			mapC1.Add(_T("for 11"), 11);
			mapC1.Add(_T("for 33"), 33);

			TESTASSERT_TRUE(mapC1.ContainsKey(_T("for 22")));
			TESTASSERT_TRUE(mapC1.ContainsValue(22));
			TESTASSERT_FALSE(mapC1.ContainsKey(_T("not exist")));
			TESTASSERT_FALSE(mapC1.ContainsValue(999));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of:Get/GetKey/Get")
		{
			//sort and desc, and with distinct
			Map<TCHAR*, int> mapC1;
			mapC1.Add(_T("for 44"), 44);
			mapC1.Add(_T("for 22"), 22);
			mapC1.Add(_T("for 11"), 11);
			mapC1.Add(_T("for 33"), 33);

			TESTASSERT_EQUAL(22, mapC1.Get(_T("for 22")));
			TESTASSERT_EQUAL(_T("for 22"), mapC1.GetKey(1));
			TESTASSERT_EQUAL(11, mapC1.GetAt(2));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of:SetValueAt")
		{
			//sort and desc, and with distinct
			Map<TCHAR*, int> mapC1;
			mapC1.Add(_T("for 44"), 44);
			mapC1.Add(_T("for 22"), 22);
			mapC1.Add(_T("for 11"), 11);

			mapC1.SetValueAt(1, 55);
			TESTASSERT_EQUAL(55, mapC1.GetAt(1));
			mapC1.SetValue(_T("for 11"), 1111);
			TESTASSERT_EQUAL(1111, mapC1.Get(_T("for 11")));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of:Equals/operator=/operator==/operator!=")
		{
			Map<TCHAR*, int> mapC1;
			mapC1.SetCapacity(10, 1.5);
			mapC1.Add(_T("444"), 44);
			mapC1.Add(_T("222"), 22);
			mapC1.Add(_T("333"), 33);
			mapC1.Add(_T("111"), 11);

			Map<TCHAR*, int> mapC2;
			mapC2.SetCapacity(20, 2.5);
			mapC2.Add(_T("444"), 44);
			mapC2.Add(_T("222"), 22);
			mapC2.Add(_T("333"), 2233);
			mapC2.Add(_T("111"), 11);
			TESTASSERT_FALSE(mapC2.Equals(mapC1));
			mapC2.SetValueAt(2, 33);
			TESTASSERT_TRUE(mapC2.Equals(mapC1));

			Map<TCHAR*, int> mapC3 = mapC1;
			//MinCapacity&Increment not change by "="
			TESTASSERT_EQUAL(0, mapC3.GetMinCapacity());
			TESTASSERT_EQUAL(1.3, mapC3.GetIncrement());
			TESTASSERT_TRUE(mapC3 == mapC1);
			TESTASSERT_FALSE(mapC3 != mapC1);

			mapC3 = mapC2;
			//MinCapacity&Increment not change by "="
			TESTASSERT_EQUAL(0, mapC3.GetMinCapacity());
			TESTASSERT_EQUAL(1.3, mapC3.GetIncrement());
			TESTASSERT_TRUE(mapC3 == mapC2);
			TESTASSERT_FALSE(mapC3 != mapC2);

			//the "=" only diff key&val
			mapC3.SetCapacity(30, 3.0);
			TESTASSERT_TRUE(mapC3 == mapC2);
			TESTASSERT_FALSE(mapC3 != mapC2);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of:operator=&sort it")
		{
			Map<TCHAR*, int> mapC1;
			mapC1.SetCapacity(10, 1.5);
			mapC1.Add(_T("444"), 44);
			mapC1.Add(_T("222"), 22);
			mapC1.Add(_T("333"), 33);
			mapC1.Add(_T("111"), 11);

			Map<TCHAR*, int> mapC2(true);
			mapC2 = mapC1;
			//this not equals
			TESTASSERT_FALSE(mapC2 == mapC1);
			TESTASSERT_TRUE(mapC2 != mapC1);
			TESTASSERT_EQUAL(11, mapC2.GetAt(0));
			TESTASSERT_EQUAL(22, mapC2.GetAt(1));
			TESTASSERT_EQUAL(33, mapC2.GetAt(2));
			TESTASSERT_EQUAL(44, mapC2.GetAt(3));
			TESTASSERT_EQUAL(_T("111"), mapC2.GetKey(0));
			TESTASSERT_EQUAL(_T("222"), mapC2.GetKey(1));
			TESTASSERT_EQUAL(_T("333"), mapC2.GetKey(2));
			TESTASSERT_EQUAL(_T("444"), mapC2.GetKey(3));

			Map<TCHAR*, int> mapC3(mapC1, true, true);
			//this not equals
			TESTASSERT_FALSE(mapC3 == mapC1);
			TESTASSERT_TRUE(mapC3 != mapC1);
			TESTASSERT_EQUAL(44, mapC3.GetAt(0));
			TESTASSERT_EQUAL(33, mapC3.GetAt(1));
			TESTASSERT_EQUAL(22, mapC3.GetAt(2));
			TESTASSERT_EQUAL(11, mapC3.GetAt(3));
			TESTASSERT_EQUAL(_T("444"), mapC3.GetKey(0));
			TESTASSERT_EQUAL(_T("333"), mapC3.GetKey(1));
			TESTASSERT_EQUAL(_T("222"), mapC3.GetKey(2));
			TESTASSERT_EQUAL(_T("111"), mapC3.GetKey(3));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:Performance,pay attation to the time of treated with large monut SORT item")
		{
			int loopcnt = 10000;
			cc::Log::traceConsole(RGB(255, 0, 0), _T("==============================\r\n"));
			cc::Log::debug(_T("add %d int item((int)(rand() * 10000) and sort of it"), loopcnt);
			cc::Log::debug(_T("others plus each key is 50byte len(about:[%f]M)"), (double)(loopcnt * 50 * 8)/1024000);
			Map<TCHAR*, int> mapC1(true);
			TCHAR tstBuf[] = _T("0123456789abcdefghij0123456789ABCDEFGHIJ0123456789");
			int i;

			cc::Log::traceConsole(RGB(255, 0, 0), _T("start adding..."));
			clock_t tmStart = ::clock ();
			for(i = 0; i < loopcnt; i++)
			{
				mapC1.Add(tstBuf, (int)(rand() * 10000));
			}
			cc::Log::traceConsole(RGB(255, 0, 0), _T("end.\r\n"));
			cc::Log::debug(_T("size of map:(%d)"), mapC1.GetSize());
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);

			//time of seek data
			cc::Log::debug(_T("seek data of middle&end:"));
			tmStart = ::clock ();
			cc::Log::debug(_T("map.Get(%d):%d"), loopcnt/2, mapC1.GetAt(loopcnt/2));
			cc::Log::debug(_T("map.Get(%d):%d"), loopcnt - 1, mapC1.GetAt(loopcnt - 1));
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);

			//it is need time to clear
			cc::Log::debug(_T("it is need time to clear,now clear..."));
			tmStart = ::clock ();
			mapC1.Clear();
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test:Performance,pay attation to the time of treated with large monut item")
		{
			int loopcnt = 10000;
			cc::Log::traceConsole(RGB(255, 0, 0), _T("==============================\r\n"));
			cc::Log::debug(_T("add %d int item((int)(rand() * 10000)) without of sort"), loopcnt);
			cc::Log::debug(_T("others plus each key is 50byte len(about:[%f]M)"), (double)(loopcnt * 50 * 8)/1024000);
			Map<TCHAR*, int> mapC1(true);
			TCHAR tstBuf[] = _T("0123456789abcdefghij0123456789ABCDEFGHIJ0123456789");
			int i;

			cc::Log::traceConsole(RGB(255, 0, 0), _T("start adding..."));
			clock_t tmStart = ::clock ();
			for(i = 0; i < loopcnt; i++)
			{
				mapC1.Add(tstBuf, (int)(rand() * 10000));
			}
			cc::Log::traceConsole(RGB(255, 0, 0), _T("end.\r\n"));
			cc::Log::debug(_T("size of map:(%d)"), mapC1.GetSize());
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);

			//time of seek data
			cc::Log::debug(_T("seek data of middle&end:"));
			tmStart = ::clock ();
			cc::Log::debug(_T("map.Get(%d):%d"), loopcnt/2, mapC1.GetAt(loopcnt/2));
			cc::Log::debug(_T("map.Get(%d):%d"), loopcnt - 1, mapC1.GetAt(loopcnt - 1));
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);

			//it is need time to clear
			cc::Log::debug(_T("it is need time to clear,now clear..."));
			tmStart = ::clock ();
			mapC1.Clear();
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);
		}
		TESTCASE_END;

	};
};

class MapTableTest : public cc::Test
{
public:
	MapTableTest()
	{
		TESTCLASS_COMMENT("test of cc::MapTable");
	}

	void Setup()
	{
	}

	void TearDown()
	{
	}

	void TestCase()
	{
		TESTCASE_BEGIN("test constructors:MapTable<int nMaxCols> tbl;")
		{
			//
			MapTable<int, char*> tbl(20);
			TESTASSERT_EQUAL(0, tbl.GetSize());
			TESTASSERT_TRUE(tbl.IsEmpty());
			TESTASSERT_EQUAL(0, tbl.GetMinCapacity());
			TESTASSERT_EQUAL(1.3, tbl.GetIncrement());
			TESTASSERT_EQUAL(false, tbl.IsDistinct());
			TESTASSERT_EQUAL(false, tbl.IsSort());
			TESTASSERT_EQUAL(false, tbl.IsSortDESC());

			//this time create tbl2
			MapTable<int, char*> tbl2(20, 10, 1.5);
			//tbl2.Add(1,"11");
			//TESTASSERT_EQUAL(1, tbl2.GetSize());
			//TESTASSERT_FALSE(tbl2.IsEmpty());
			TESTASSERT_EQUAL(10, tbl2.GetMinCapacity());
			TESTASSERT_EQUAL(1.5, tbl2.GetIncrement());
			TESTASSERT_EQUAL(false, tbl2.IsDistinct());
			TESTASSERT_EQUAL(false, tbl2.IsSort());
			TESTASSERT_EQUAL(false, tbl2.IsSortDESC());

			//this time create tbl3
			MapTable<int, char*> tbl3(20, true, true, true);
			//tbl3.Add(1,"11");
			//tbl3.Add(2,"22");
			//TESTASSERT_EQUAL(2, tbl3.GetSize());
			//TESTASSERT_FALSE(tbl3.IsEmpty());
			TESTASSERT_EQUAL(0, tbl3.GetMinCapacity());
			TESTASSERT_EQUAL(1.3, tbl3.GetIncrement());
			TESTASSERT_EQUAL(true, tbl3.IsDistinct());
			TESTASSERT_EQUAL(true, tbl3.IsSort());
			TESTASSERT_EQUAL(true, tbl3.IsSortDESC());

			//this time create *tbl4,not forget to release it after used it
			MapTable<int, char*> *tbl4 = new MapTable<int, char*>(20);
			//tbl4->Add(1,"11");
			//tbl4->Add(2,"22");
			//tbl4->Add(3,"33");
			//TESTASSERT_EQUAL(3, tbl4->GetSize());
			//tbl4 is not auto destructor after this cope!!
			tbl4->Clear();
			TESTASSERT_EQUAL(0, tbl4->GetSize());
			TESTASSERT_TRUE(tbl4->IsEmpty());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test char&wchar_t:List(bool isSort, bool bIsDESC = false, bool isDistinct = false)")
		{
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of:ContainsKey/ContainsValue")
		{
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of:Get/GetKey/Get")
		{
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of:SetValue")
		{
		}
		TESTCASE_END;

	};
};

} //namespace cc

#endif //#ifndef cc_ListTest_H_
