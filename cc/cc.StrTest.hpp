/* cc.StrTest.hpp
   Test for cc.Str
   Author: Shu.KK
   created:2004-09-25
   modified:2005-09-01
   change to new format:2005-12-01
*/
#ifndef cc_StrTest_H_
#define cc_StrTest_H_

#include "cc.Str.h"
#include "cc.Test.h"

namespace cc
{

class StrTest : public cc::Test
{
public:
	StrTest()
	{
		TESTCLASS_COMMENT("test of cc::Str");
	}

	void Setup()
	{
	}

	void TearDown()
	{
	}

	void TestCase()
	{
		TESTCASE_BEGIN("test class name;")
		{
			Str str;
			TCHAR claName[] = _T("class cc::Str");
			//class name may diferent to compiler!
			TESTASSERT_TRUE(str.IsClass(claName));
			TESTASSERT_TRUE(str.IsClass(cc::Str(claName)));

			TESTASSERT_EQUAL(claName, (TCHAR *)str.GetClassName().GetBuf());
			TESTASSERT_TRUE(str.GetClassID() == CC_CLASSID_Str);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of static int Len(const TCHAR *str)")
		{
			const TCHAR *tc_1 = NULL;
			const TCHAR *tc_2 = _T("");
			const TCHAR *tc_3 = _T("0123456789ABCDEFGHIJ");
			const TCHAR *tc_4 = _T("89AB");

			int exp1 = cc::Str::Len(tc_1);
			TESTASSERT_TRUE(exp1 == 0);
			int exp2 = cc::Str::Len(tc_2);
			TESTASSERT_TRUE(exp2 == 0);
			int exp3 = cc::Str::Len(tc_3);
			TESTASSERT_TRUE(exp3 == 20);
			int exp4 = cc::Str::Len(tc_4);
			TESTASSERT_TRUE(exp4 == 4);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of static int Cmp(const TCHAR *str, const TCHAR *str2, bool ignoreCase = false)/NCmp/Equals,Compare(same to Cmp)")
		{
			const TCHAR *tcnull = NULL;
			const TCHAR *tc_a = _T("0123456789ABCDEFGHIJ");
			const TCHAR *tc_b = _T("0123456789ABCDEFGHIJ");
			const TCHAR *tc_c = _T("89AB");
			const TCHAR *tc_d = _T("89ab");
			const TCHAR *tc_e = _T("0123456789abcde");
			const TCHAR *tc_f = _T("0123456789abcdefghijklm");

			int exp2 = cc::Str::Cmp(tc_a, tc_b);
			TESTASSERT_TRUE(exp2 == 0);
			int exp3 = cc::Str::Cmp(tc_a, tc_c);
			TESTASSERT_TRUE(exp3 < 0);
			int exp4 = cc::Str::Cmp(tc_c, tc_a);
			TESTASSERT_TRUE(exp4 > 0);
			exp2 = cc::Str::Compare(tc_a, tc_b);
			TESTASSERT_TRUE(exp2 == 0);
			exp3 = cc::Str::Compare(tc_a, tc_c);
			TESTASSERT_TRUE(exp3 < 0);
			exp4 = cc::Str::Compare(tc_c, tc_a);
			TESTASSERT_TRUE(exp4 > 0);

			int exp5 = cc::Str::Cmp(tc_c, tc_d);
			TESTASSERT_TRUE(exp5 < 0);
			int exp6 = cc::Str::Cmp(tc_c, tc_d, true);
			TESTASSERT_TRUE(exp6 == 0);
			exp5 = cc::Str::Compare(tc_c, tc_d);
			TESTASSERT_TRUE(exp5 < 0);
			exp6 = cc::Str::Compare(tc_c, tc_d, true);
			TESTASSERT_TRUE(exp6 == 0);

			int exp7 = cc::Str::Cmp(tc_a, tc_e);
			TESTASSERT_TRUE(exp7 < 0);
			int exp71 = cc::Str::Cmp(tc_a, tc_e, true);
			TESTASSERT_TRUE(exp71 > 0);
			exp7 = cc::Str::Compare(tc_a, tc_e);
			TESTASSERT_TRUE(exp7 < 0);
			exp71 = cc::Str::Compare(tc_a, tc_e, true);
			TESTASSERT_TRUE(exp71 > 0);

			int exp8 = cc::Str::Cmp(tc_a, tc_f);
			TESTASSERT_TRUE(exp8 < 0);
			int exp81 = cc::Str::Cmp(tc_a, tc_f, true);
			TESTASSERT_TRUE(exp81 < 0);
			exp8 = cc::Str::Compare(tc_a, tc_f);
			TESTASSERT_TRUE(exp8 < 0);
			exp81 = cc::Str::Compare(tc_a, tc_f, true);
			TESTASSERT_TRUE(exp81 < 0);

			int exp9 = cc::Str::NCmp(tc_a, tc_e, 15);
			TESTASSERT_TRUE(exp9 < 0);
			int exp91 = cc::Str::NCmp(tc_a, tc_e, 15, true);
			TESTASSERT_TRUE(exp91 == 0);
			exp9 = cc::Str::Compare(tc_a, 0, tc_e, 0, 15);
			TESTASSERT_TRUE(exp9 < 0);
			exp91 = cc::Str::Compare(tc_a, 0, tc_e, 0, 15, true);
			TESTASSERT_TRUE(exp91 == 0);

			int exp10 = cc::Str::NCmp(tc_a, tc_f, 20);
			TESTASSERT_TRUE(exp10 < 0);
			int exp101 = cc::Str::NCmp(tc_a, tc_f, 20, true);
			TESTASSERT_TRUE(exp101 == 0);
			exp10 = cc::Str::Compare(tc_a, 0, tc_f, 0, 20);
			TESTASSERT_TRUE(exp10 < 0);
			exp101 = cc::Str::Compare(tc_a, 0, tc_f, 0, 20, true);
			TESTASSERT_TRUE(exp101 == 0);

			int exp11 = cc::Str::NCmp(tc_a, tc_f, 30, true);
			TESTASSERT_TRUE(exp11 < 0);
			exp11 = cc::Str::Compare(tc_a, 0, tc_f, 0, 30, true);
			TESTASSERT_TRUE(exp11 < 0);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of static int Search(const TCHAR *buf, ..., const TCHAR *str, ...)")
		{
			const TCHAR *tcnull = NULL;
			const TCHAR *tc_a = _T("0123456789ABCDEFGHIJ");
			const TCHAR *tc_b = _T("0123456789ABCDEFGHIJ");
			const TCHAR *tc_c = _T("89AB");
			const TCHAR *tc_d = _T("89ab");
			const TCHAR *tc_e = _T("0123456789abced");
			const TCHAR *tc_f = _T("0123456789abcedfghijklm");

			int exp2 = cc::Str::Search(tc_a, tc_b);
			TESTASSERT_TRUE(exp2 == 0);
			int exp3 = cc::Str::Search(tc_a, tc_c);
			TESTASSERT_TRUE(exp3 == 8);
			int exp4 = cc::Str::Search(tc_c, tc_a);
			TESTASSERT_TRUE(exp4 < 0);

			int exp5 = cc::Str::Search(tc_c, tc_d);
			TESTASSERT_TRUE(exp5 < 0);
			int exp6 = cc::Str::Search(tc_c, tc_d, true);
			TESTASSERT_TRUE(exp6 == 0);

			int exp7 = cc::Str::Search(tc_a, 5, 6, tc_d, -1, true);
			TESTASSERT_TRUE(exp7 < 0);
			int exp72 = cc::Str::Search(tc_a, 5, 7, tc_d, -1, true);
			TESTASSERT_TRUE(exp72 == 8);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of static int SearchLast(const char *buf, ..., const char *str, ...)")
		{
			const TCHAR *tcnull = NULL;
			const TCHAR *tc_a = _T("0123456789ABCDEFGHIJ");
			const TCHAR *tc_b = _T("0123456789ABCDEFGHIJ");
			const TCHAR *tc_c = _T("89AB");
			const TCHAR *tc_d = _T("89ab");
			const TCHAR *tc_e = _T("0123456789abced");
			const TCHAR *tc_f = _T("0123456789abcedfghijklm");

			int exp2 = cc::Str::SearchLast(tc_a, tc_b);
			TESTASSERT_TRUE(exp2 == 0);
			int exp3 = cc::Str::SearchLast(tc_a, tc_c);
			TESTASSERT_TRUE(exp3 == 8);
			int exp4 = cc::Str::SearchLast(tc_c, tc_a);
			TESTASSERT_TRUE(exp4 < 0);

			int exp5 = cc::Str::SearchLast(tc_c, tc_d);
			TESTASSERT_TRUE(exp5 < 0);
			int exp6 = cc::Str::SearchLast(tc_c, tc_d, true);
			TESTASSERT_TRUE(exp6 == 0);

			//int exp7 = cc::Str::SearchLast(tc_a, 5, 6, tc_d, -1, true);
			//TESTASSERT_TRUE(exp7 < 0);
			//int exp72 = cc::Str::SearchLast(tc_a, 5, 7, tc_d, -1, true);
			//TESTASSERT_TRUE(exp72 == 8);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of static Str Concat(const TCHAR*/Str&, ...)")
		{
			Str str = cc::Str::Concat(_T("11"), _T("22"));
			TESTASSERT_EQUAL(_T("1122"), (TCHAR *)str.GetBuf());
			Str str2 = cc::Str::Concat(_T("11"), _T("22"), _T("33"));
			TESTASSERT_EQUAL(_T("112233"), (TCHAR *)str2.GetBuf());
			Str str3 = cc::Str::Concat(_T("11"), _T("22"), _T("33"), _T("44"));
			TESTASSERT_EQUAL(_T("11223344"), (TCHAR *)str3.GetBuf());

			//pay attation to that saveLangCode is defined by _UNICODE
			Str str4 = cc::Str::Concat("11", "22");
			TESTASSERT_EQUAL(_T("1122"), (TCHAR *)str4.GetBuf());
			Str str5 = cc::Str::Concat(L"11", L"22", L"33");
			TESTASSERT_EQUAL(_T("112233"), (TCHAR *)str5.GetBuf());
			Str str6 = cc::Str::Concat(Str(_T("aa")), Str(_T("bb")), Str(_T("cc")), Str(_T("dd")));
			TESTASSERT_EQUAL(_T("aabbccdd"), (TCHAR *)str6.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("static Str Join(const Str &separator, const Str str[], int startIndex, int count, bool noLastSeparator = false)")
		{
			Str str[5];
			str[0] = _T("11");
			str[1] = _T("22");
			str[2] = _T("33");
			Str separator = _T("; ");
			Str result = cc::Str::Join(separator, str, 0, 3);
			TESTASSERT_EQUAL(_T("11; 22; 33; "), (TCHAR *)result.GetBuf());

			Str str2[5];
			str2[0] = _T("11");
			str2[1] = _T("22");
			str2[2] = _T("33");
			str2[3] = _T("44");
			str2[4] = _T("55");
			Str separator2 = _T(",");
			Str result2 = cc::Str::Join(separator2, str2, 1, 3, true);
			TESTASSERT_EQUAL(_T("22,33,44"), (TCHAR *)result2.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test NULL for all method;")
		{
			//this time class[str1("a")] is created(has NULL value).
			Str str;
			TESTASSERT_TRUE(str.IsNULL());
			TESTASSERT_TRUE(str.IsBlankWithTrim());
			TESTASSERT_TRUE(str.IsBlankWithoutTrim());
			TESTASSERT_NULL((TCHAR *)str.GetBuf());
			TESTASSERT_TRUE(str.Equals(str));

			TESTASSERT_TRUE(str.CompareTo((char*)NULL) == 0);
			TESTASSERT_TRUE(str.CompareTo(_T("a")) < 0);
			TESTASSERT_TRUE(str.CompareTo(_T("")) < 0);

			TESTASSERT_FALSE(str.EndsWith(_T("a")));
			TESTASSERT_FALSE(str.EndsWith(_T("A"), true));

			Str str1 = str.ToString();
			TESTASSERT_TRUE(str1.IsNULL());

			Str str2 = str.Clone();
			TESTASSERT_TRUE(str2.IsNULL());

			Str str3 = cc::Str();
			TESTASSERT_TRUE(str3.Equals((char*)NULL, true));
			TESTASSERT_FALSE(str3.Equals(_T(""), true));
			TESTASSERT_TRUE(str.Equals(str3, true));

			Str str4;
			Str in4 = _T("string1");
			Str exp4 = _T("string:string1, string:string2, int:123, float:456.7890");
			str4.Format(_T("string:%s, string:%s, int:%d, float:%4.4f"), (TCHAR*)in4.GetBuf(), _T("string2"), 123, 456.789);
			cc::Log::debug(_T("%s"), (TCHAR*)str4);
			TESTASSERT_TRUE(str4.Equals(exp4));

			Str str5;
			Str exp5 = "string:string1, string:string2, int:123, float:456.7890";
			str5.Format(cc::LangCode::SYSTEM, "string:%s, string:%s, int:%d, float:%4.4f", (char*)in4, "string2", 123, 456.789);
			cc::Log::debug("%s", (char*)str5);
			TESTASSERT_TRUE(str5.Equals(exp5));

			Str str6;
			Str exp6;
			//indexOf NULL is return -1
			TESTASSERT_TRUE(str6.IndexOf((char*)NULL) == 0);
			TESTASSERT_TRUE(str6.IndexOf(_T("")) < 0);
			TESTASSERT_TRUE(str6.IndexOf(_T("a")) < 0);
			TESTASSERT_TRUE(str6.IndexOf(exp6) == 0);

			Str str7;
			Str exp7;
			str7.Insert(0, (char*)NULL);
			TESTASSERT_TRUE(str7.Equals(exp7));

			Str str8;
			Str exp8 = _T("");
			str8.Insert(0, _T(""));
			TESTASSERT_TRUE(str8.Equals(exp8));

			Str str9;
			Str exp9 = _T("Aa");
			str9.Insert(0, _T("Aa"));
			TESTASSERT_TRUE(str9.Equals(exp9));

			Str list10[] = {_T("1"), "2", _T("3"), "4"};
			Str exp10 = _T("2-3");
			Str str10 = cc::Str::Join(_T("-"), list10, 1, 2, true);
			TESTASSERT_TRUE(str10.Equals(exp10));

			Str str11;
			Str exp11;
			//indexOf NULL is return -1
			TESTASSERT_TRUE(str11.LastIndexOf((char*)NULL) == 0);
			TESTASSERT_TRUE(str11.LastIndexOf(_T("")) < 0);
			TESTASSERT_TRUE(str11.LastIndexOf(_T("a")) < 0);
			TESTASSERT_TRUE(str11.LastIndexOf(exp11) == 0);

			Str str12;
			Str exp12 = "XXX";
			str12.Pad(3, 'X');
			TESTASSERT_TRUE(str12.Equals(exp12));

			Str str13;
			Str exp13 = "XXX";
			str13.PadLeft(3, 'X');
			TESTASSERT_TRUE(str13.Equals(exp13));

			Str str14;
			Str exp14 = "XXX";
			str14.PadRight(3, 'X');
			TESTASSERT_TRUE(str14.Equals(exp14));

			Str str15;
			Str exp15;
			str15.Remove(0);
			TESTASSERT_TRUE(str15.Equals(exp15));

			Str str16;
			Str exp16;
			str16.Replace(_T("A"), _T("B"));
			TESTASSERT_TRUE(str16.Equals(exp16));

			Str str17;
			Str exp17;
			TESTASSERT_TRUE(str17.StartsWith(exp17));
			TESTASSERT_FALSE(str17.StartsWith(_T("Aa")));

			Str str18;
			Str str182 = str18.Substring(0);
			TESTASSERT_TRUE(str18.IsNULL());
			TESTASSERT_TRUE(str182.IsNULL());

			Str str19;
			str19.ToLower();
			TESTASSERT_TRUE(str19.IsNULL());

			Str str20;
			str20.ToUpper();
			TESTASSERT_TRUE(str20.IsNULL());

			Str str21;
			Str str212 = str20.ToString();
			TESTASSERT_TRUE(str212.IsNULL());

			Str str22;
			str22.Trim();
			str22.TrimLeft();
			str22.TrimRight();
			TESTASSERT_TRUE(str22.IsNULL());

			Str str23;
			str23.Clear();
			TESTASSERT_TRUE(str23.IsNULL());

			Str str24;
			TESTASSERT_TRUE(str24.GetLength() == 0);

			Str str25;
			Str *pcs = NULL;
			int ret25 = str25.Split(";", &pcs, true);
			TESTASSERT_TRUE(ret25 == 0);
			TESTASSERT_TRUE(pcs == NULL);

			Str str26;
			Str exp26 = str26.SplitBy(";", true);
			TESTASSERT_TRUE(str26.IsNULL());
			TESTASSERT_TRUE(exp26.IsNULL());

			Str str27;
			TESTASSERT_TRUE(str27.GetBuf() == NULL);
			str27.CompactStringBuf();
			TESTASSERT_TRUE(str27.IsNULL());

			Str str28;
			TESTASSERT_TRUE(str28.ToInt() == 0);
			Str str29;
			TESTASSERT_TRUE(str29.ToDouble() == 0);
			Str str30;
			TESTASSERT_TRUE(str30.ToFloat() == 0);
			Str str31;
			TESTASSERT_TRUE(str31.ToLong() == 0);

			Str str32;
			TESTASSERT_TRUE(str32.ToMByte());
			TESTASSERT_TRUE(str32.IsNULL());
			TESTASSERT_TRUE(str32.ToTChar());
			TESTASSERT_TRUE(str32.IsNULL());
			TESTASSERT_TRUE(str32.ToWChar());
			TESTASSERT_TRUE(str32.IsNULL());

			Str str33;
			str33.ToReverse();
			TESTASSERT_TRUE(str33.IsNULL());

			Str str34;
			Str exp341 = str34.Left(1);
			Str exp342 = str34.Right(1);
			Str exp343 = str34.Mid(1);
			TESTASSERT_TRUE(exp341.IsNULL());
			TESTASSERT_TRUE(exp342.IsNULL());
			TESTASSERT_TRUE(exp343.IsNULL());

			Str str35;
			Str exp35 = _T("aA");
			str35.Append(_T("aA"));
			TESTASSERT_TRUE(str35.Equals(exp35));

			Str str36;
			Str exp36 = _T("Aa");
			str36.AppendFormat(_T("%s"), _T("Aa"));
			TESTASSERT_TRUE(str36.Equals(exp36));

			Str str37;
			Str exp37 = _T("AaAaAa");
			str37.Repeat(_T("Aa"), 3);
			TESTASSERT_TRUE(str37.Equals(exp37));

			Str str38;
			int ret38 = str38.FindCount(_T("Aa"));
			TESTASSERT_TRUE(ret38 == 0);

			Str str39;
			TESTASSERT_FALSE(str39.IsAlpha());
			TESTASSERT_TRUE(str39.IsBlankWithoutTrim());
			TESTASSERT_TRUE(str39.IsBlankWithTrim());
			TESTASSERT_FALSE(str39.IsLower());
			TESTASSERT_FALSE(str39.IsNumber());
			TESTASSERT_TRUE(str39.IsTChar());
			TESTASSERT_TRUE(str39.IsUnicode());
			TESTASSERT_FALSE(str39.IsUpper());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test Blank(\"\") for all method;")
		{
			//this time class[str1("a")] is created(has NULL value).
			Str str(_T(""));
			TESTASSERT_FALSE(str.IsNULL());
			TESTASSERT_TRUE(str.IsBlankWithTrim());
			TESTASSERT_TRUE(str.IsBlankWithoutTrim());
			TESTASSERT_TRUE(str.Equals(str));

			TESTASSERT_TRUE(str.CompareTo((char*)NULL) > 0);
			TESTASSERT_TRUE(str.CompareTo(_T("a")) < 0);
			TESTASSERT_TRUE(str.CompareTo(_T("")) == 0);

			TESTASSERT_FALSE(str.EndsWith(_T("a")));
			TESTASSERT_FALSE(str.EndsWith(_T("A"), true));

			Str str0 = _T("");
			TESTASSERT_FALSE(str0.IsNULL());
			TESTASSERT_TRUE(str0.IsBlankWithTrim());
			TESTASSERT_TRUE(str0.IsBlankWithoutTrim());
			TESTASSERT_TRUE(str0.Equals(str0));

			Str str1 = str.ToString();
			TESTASSERT_FALSE(str1.IsNULL());
			TESTASSERT_TRUE(str1.IsBlankWithTrim());
			TESTASSERT_TRUE(str1.IsBlankWithoutTrim());

			Str str2 = str.Clone();
			TESTASSERT_FALSE(str2.IsNULL());
			TESTASSERT_TRUE(str2.IsBlankWithTrim());
			TESTASSERT_TRUE(str2.IsBlankWithoutTrim());

			Str str3 = cc::Str(_T(""));
			TESTASSERT_FALSE(str3.Equals((char*)NULL, true));
			TESTASSERT_TRUE(str3.Equals(_T(""), true));
			TESTASSERT_TRUE(str.Equals(str3, true));

			Str str4 = _T("");
			Str in4 = _T("string1");
			Str exp4 = _T("string:string1, string:string2, int:123, float:456.7890");
			str4.Format(_T("string:%s, string:%s, int:%d, float:%4.4f"), (TCHAR*)in4.GetBuf(), _T("string2"), 123, 456.789);
			cc::Log::debug(_T("%s"), (TCHAR*)str4);
			TESTASSERT_TRUE(str4.Equals(exp4));

			Str str5 = "";
			Str exp5 = "string:string1, string:string2, int:123, float:456.7890";
			str5.Format(cc::LangCode::SYSTEM, "string:%s, string:%s, int:%d, float:%4.4f", (char*)in4, "string2", 123, 456.789);
			cc::Log::debug("%s", (char*)str5);
			TESTASSERT_TRUE(str5.Equals(exp5));

			Str str6 = _T("");
			Str exp6 = _T("");
			//indexOf NULL is return -1
			TESTASSERT_TRUE(str6.IndexOf((char*)NULL) < 0);
			TESTASSERT_TRUE(str6.IndexOf(_T("")) == 0);
			TESTASSERT_TRUE(str6.IndexOf(_T("a")) < 0);
			TESTASSERT_TRUE(str6.IndexOf(exp6) == 0);

			Str str7 = _T("");
			Str exp7 = _T("");
			str7.Insert(0, (char*)NULL);
			TESTASSERT_TRUE(str7.Equals(exp7));

			Str str8 = _T("");
			Str exp8 = _T("");
			str8.Insert(0, _T(""));
			TESTASSERT_TRUE(str8.Equals(exp8));

			Str str9 = _T("");
			Str exp9 = _T("Aa");
			str9.Insert(0, _T("Aa"));
			TESTASSERT_TRUE(str9.Equals(exp9));

			Str list10[] = {_T("1"), "2", _T("3"), "4"};
			Str exp10 = _T("2-3");
			Str str10 = cc::Str::Join(_T("-"), list10, 1, 2, true);
			TESTASSERT_TRUE(str10.Equals(exp10));

			Str str11 = _T("");
			Str exp11 = _T("");
			//indexOf NULL is return -1
			TESTASSERT_TRUE(str11.LastIndexOf((char*)NULL) < 0);
			TESTASSERT_TRUE(str11.LastIndexOf(_T("")) == 0);
			TESTASSERT_TRUE(str11.LastIndexOf(_T("a")) < 0);
			TESTASSERT_TRUE(str11.LastIndexOf(exp11) == 0);

			Str str12 = _T("");
			Str exp12 = "XXX";
			str12.Pad(3, 'X');
			TESTASSERT_TRUE(str12.Equals(exp12));

			Str str13 = _T("");
			Str exp13 = "XXX";
			str13.PadLeft(3, 'X');
			TESTASSERT_TRUE(str13.Equals(exp13));

			Str str14 = _T("");
			Str exp14 = "XXX";
			str14.PadRight(3, 'X');
			TESTASSERT_TRUE(str14.Equals(exp14));

			Str str15 = _T("");
			Str exp15 = _T("");
			str15.Remove(0);
			TESTASSERT_TRUE(str15.Equals(exp15));

			Str str16 = _T("");
			Str exp16 = _T("");
			str16.Replace(_T("A"), _T("B"));
			TESTASSERT_TRUE(str16.Equals(exp16));

			Str str17 = _T("");
			Str exp17 = _T("");
			TESTASSERT_TRUE(str17.StartsWith(exp17));
			TESTASSERT_FALSE(str17.StartsWith(_T("Aa")));

			Str str18 = _T("");
			Str str182 = str18.Substring(0);
			TESTASSERT_TRUE(str18.IsBlankWithoutTrim());
			TESTASSERT_TRUE(str182.IsBlankWithoutTrim());

			Str str19 = _T("");
			str19.ToLower();
			TESTASSERT_TRUE(str19.IsBlankWithoutTrim());

			Str str20 = _T("");
			str20.ToUpper();
			TESTASSERT_TRUE(str20.IsBlankWithoutTrim());

			Str str21 = _T("");
			Str str212 = str20.ToString();
			TESTASSERT_TRUE(str212.IsBlankWithoutTrim());

			Str str22 = _T("");
			str22.Trim();
			str22.TrimLeft();
			str22.TrimRight();
			TESTASSERT_TRUE(str22.IsBlankWithoutTrim());

			Str str23 = _T("");
			str23.Clear();
			TESTASSERT_TRUE(str23.IsBlankWithoutTrim());

			Str str24 = _T("");
			TESTASSERT_TRUE(str24.GetLength() == 0);

			Str str25 = _T("");
			Str *pcs = NULL;
			int ret25 = str25.Split(";", &pcs, true);
			TESTASSERT_TRUE(ret25 == 0);
			TESTASSERT_TRUE(pcs == NULL);

			Str str26 = _T("");
			Str exp26 = str26.SplitBy(";", true);
			TESTASSERT_TRUE(str26.IsBlankWithoutTrim());
			TESTASSERT_TRUE(exp26.IsBlankWithoutTrim());

			Str str27 = _T("");
			TESTASSERT_FALSE(str27.GetBuf() == NULL);
			str27.CompactStringBuf();
			TESTASSERT_TRUE(str27.IsBlankWithoutTrim());

			Str str28 = _T("");
			TESTASSERT_TRUE(str28.ToInt() == 0);
			Str str29 = _T("");
			TESTASSERT_TRUE(str29.ToDouble() == 0);
			Str str30 = _T("");
			TESTASSERT_TRUE(str30.ToFloat() == 0);
			Str str31 = _T("");
			TESTASSERT_TRUE(str31.ToLong() == 0);

			Str str32 = _T("");
			TESTASSERT_TRUE(str32.ToMByte());
			TESTASSERT_TRUE(str32.IsBlankWithoutTrim());
			TESTASSERT_TRUE(str32.ToTChar());
			TESTASSERT_TRUE(str32.IsBlankWithoutTrim());
			TESTASSERT_TRUE(str32.ToWChar());
			TESTASSERT_TRUE(str32.IsBlankWithoutTrim());

			Str str33 = _T("");
			str33.ToReverse();
			TESTASSERT_TRUE(str33.IsBlankWithoutTrim());

			Str str34 = _T("");
			Str exp341 = str34.Left(1);
			Str exp342 = str34.Right(1);
			Str exp343 = str34.Mid(1);
			TESTASSERT_TRUE(exp341.IsBlankWithoutTrim());
			TESTASSERT_TRUE(exp342.IsBlankWithoutTrim());
			TESTASSERT_TRUE(exp343.IsBlankWithoutTrim());

			Str str35 = _T("");
			Str exp35 = _T("aA");
			str35.Append(_T("aA"));
			TESTASSERT_TRUE(str35.Equals(exp35));

			Str str36 = _T("");
			Str exp36 = _T("Aa");
			str36.AppendFormat(_T("%s"), _T("Aa"));
			TESTASSERT_TRUE(str36.Equals(exp36));

			Str str37 = _T("");
			Str exp37 = _T("AaAaAa");
			str37.Repeat(_T("Aa"), 3);
			TESTASSERT_TRUE(str37.Equals(exp37));

			Str str38 = _T("");
			int ret38 = str38.FindCount(_T("Aa"));
			TESTASSERT_TRUE(ret38 == 0);

			Str str39 = _T("");
			TESTASSERT_FALSE(str39.IsAlpha());
			TESTASSERT_TRUE(str39.IsBlankWithoutTrim());
			TESTASSERT_TRUE(str39.IsBlankWithTrim());
			TESTASSERT_FALSE(str39.IsLower());
			TESTASSERT_FALSE(str39.IsNumber());
			TESTASSERT_TRUE(str39.IsTChar());
			TESTASSERT_TRUE(str39.IsUnicode());
			TESTASSERT_FALSE(str39.IsUpper());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test constructors:Str str; and Str *pstr;")
		{
			//this time class[str1("a")] is created(has NULL value).
			Str str = _T("a");
			TESTASSERT_FALSE(str.IsNULL());
			TESTASSERT_EQUAL(_T("a"), (TCHAR *)str.GetBuf());
			//this time create another Str()[str2("b")],and by "operator=" value is copied
			//and also this time [str2("b")] is destructor
			str = Str(_T("b"));
			TESTASSERT_FALSE(str.IsNULL());
			TESTASSERT_EQUAL(_T("b"), (TCHAR *)str.GetBuf());
			//this time class is not created yet.
			Str *pstr;
			pstr = NULL;
			TESTASSERT_NULL(pstr);
			//create class this time
			pstr = new Str(_T("c"));
			TESTASSERT_FALSE(pstr->IsNULL());
			TESTASSERT_EQUAL(_T("c"), (TCHAR *)pstr->GetBuf());
			//pstr("c") is not auto destructor after this cope!!
			pstr->Clear();
			delete pstr;
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test NULL:Str str;str.IsNULL();")
		{
			Str str;
			TESTASSERT_TRUE(str.IsNULL());
			//test:TCHAR,you can change defined UNICODE to test char or wchar_t
			TESTASSERT_NULL(str.GetBuf());

			Str str2 = (TCHAR*)NULL;
			TESTASSERT_TRUE(str2.IsNULL());
			TESTASSERT_NULL(str2.GetBuf());

			TCHAR* tc = NULL;
			Str str3 = tc;
			TESTASSERT_TRUE(str3.IsNULL());
			TESTASSERT_NULL(str3.GetBuf());

			Str strnull = (TCHAR*)NULL;
			Str str4 = _T("a");
			TESTASSERT_FALSE(str4.IsNULL());
			str4 = strnull;
			TESTASSERT_TRUE(str4.IsNULL());
			str4 = _T("a");
			TESTASSERT_FALSE(str4.IsNULL());
			str4.Clear();
			TESTASSERT_TRUE(str4.IsNULL());

			//constructors with NULL
			TCHAR* tcnull = NULL;
			Str strnull1(tcnull, 100);
			TESTASSERT_TRUE(strnull1.IsNULL());
			Str strnull2(tcnull, 100, 200);
			TESTASSERT_TRUE(strnull2.IsNULL());
			Str strnull3(strnull, 100, 200);
			TESTASSERT_TRUE(strnull3.IsNULL());
			Str strnull4(cc::LangCode::UNICODE16, tcnull, 100);
			TESTASSERT_TRUE(strnull4.IsNULL());
			Str strnull5(cc::LangCode::UNICODE16, tcnull, 100, 200);
			TESTASSERT_TRUE(strnull5.IsNULL());
			Str strnull6(cc::LangCode::UNICODE16, tcnull, -100);
			TESTASSERT_TRUE(strnull6.IsNULL());
			Str strnull7(cc::LangCode::UNICODE16, strnull, 100, 200);
			TESTASSERT_TRUE(strnull7.IsNULL());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of Str(const TCHAR*/Str& str, int fromIndex, int len);")
		{
			//constructors with int fromIndex, int len, LangCode fromLangCode = LangCode::SYSTEM
			Str str1 = _T("0123456789ABCDEFGHIJ");
			//default langcode
			#if defined (_UNICODE)
			TESTASSERT_EQUAL(cc::LangCode::UNICODE16, str1.GetLangCode());
			#else
			TESTASSERT_EQUAL(cc::LangCode::SYSTEM, str1.GetLangCode());
			#endif
			TESTASSERT_EQUAL_MSG("error of constructors", _T("0123456789ABCDEFGHIJ"), (TCHAR *)str1.GetBuf());

			//from begin,to end
			Str str2 = Str(_T("0123456789ABCDEFGHIJ"), 5);
			TESTASSERT_EQUAL_MSG("error of constructors", _T("56789ABCDEFGHIJ"), (TCHAR *)str2.GetBuf());

			//from begin,to len
			Str str3 = Str(_T("0123456789ABCDEFGHIJ"), 5, 10);
			TESTASSERT_EQUAL_MSG("error of constructors", _T("56789ABCDE"), (TCHAR *)str3.GetBuf());

			//take nothing
			Str str4 = Str(_T("0123456789ABCDEFGHIJ"), -5, 10);
			TESTASSERT_TRUE(str4.IsNULL());
			TESTASSERT_EQUAL_MSG("error of constructors", (TCHAR *)NULL, (TCHAR *)str4.GetBuf());

			//take to end
			Str str5 = Str(_T("0123456789ABCDEFGHIJ"), 5, -10);
			TESTASSERT_FALSE(str5.IsNULL());
			TESTASSERT_EQUAL_MSG("error of constructors", _T("56789ABCDEFGHIJ"), (TCHAR *)str5.GetBuf());

			Str str6 = Str((TCHAR*)NULL, 5, 10);
			TESTASSERT_TRUE(str6.IsNULL());
			TESTASSERT_NULL((TCHAR *)str6.GetBuf());

			Str strstr = _T("0123456789ABCDEFGHIJ");
			Str strstr1 = Str(strstr);
			TESTASSERT_EQUAL_MSG("error of constructors", _T("0123456789ABCDEFGHIJ"), (TCHAR *)strstr1.GetBuf());

			//from begin,to end
			Str strstr2 = Str(strstr, 5);
			TESTASSERT_EQUAL_MSG("error of constructors", _T("56789ABCDEFGHIJ"), (TCHAR *)strstr2.GetBuf());

			//from begin,to len
			Str strstr3 = Str(strstr, 5, 10);
			TESTASSERT_EQUAL_MSG("error of constructors", _T("56789ABCDE"), (TCHAR *)strstr3.GetBuf());

			//define with LangCode:LangCode::SYSTEM
			//from TCHAR
			Str strLang1 = Str(cc::LangCode::SYSTEM, _T("0123456789ABCDEFGHIJ"));
			TESTASSERT_EQUAL(cc::LangCode::SYSTEM, strLang1.GetLangCode());
			TESTASSERT_EQUAL("0123456789ABCDEFGHIJ", (char *)strLang1.GetBuf());
			Str strLang2 = Str(cc::LangCode::SYSTEM, _T("0123456789ABCDEFGHIJ"), 5);
			TESTASSERT_EQUAL("56789ABCDEFGHIJ", (char *)strLang2.GetBuf());
			Str strLang3 = Str(cc::LangCode::SYSTEM, _T("0123456789ABCDEFGHIJ"), 5, 10);
			TESTASSERT_EQUAL("56789ABCDE", (char *)strLang3.GetBuf());
			Str strLang4 = Str(cc::LangCode::SYSTEM, (char*)NULL, 5, 10);
			TESTASSERT_TRUE(str6.IsNULL());
			TESTASSERT_NULL((char *)strLang4.GetBuf());
			//from Str
			Str strstrLang1 = Str(cc::LangCode::SYSTEM, strstr);
			TESTASSERT_EQUAL("0123456789ABCDEFGHIJ", (char *)strstrLang1.GetBuf());
			Str strstrLang2 = Str(cc::LangCode::SYSTEM, strstr, 5);
			TESTASSERT_EQUAL("56789ABCDEFGHIJ", (char *)strstrLang2.GetBuf());
			Str strstrLang3 = Str(cc::LangCode::SYSTEM, strstr, 5, 10);
			TESTASSERT_EQUAL("56789ABCDE", (char *)strstrLang3.GetBuf());

			//define with LangCode:LangCode::UNICODE16
			//from TCHAR
			Str strLang21 = Str(cc::LangCode::UNICODE16, _T("0123456789ABCDEFGHIJ"));
			TESTASSERT_EQUAL(cc::LangCode::UNICODE16, strLang21.GetLangCode());
			TESTASSERT_EQUAL(L"0123456789ABCDEFGHIJ", (wchar_t *)strLang21.GetBuf());
			Str strLang22 = Str(cc::LangCode::UNICODE16, _T("0123456789ABCDEFGHIJ"), 5);
			TESTASSERT_EQUAL(L"56789ABCDEFGHIJ", (wchar_t *)strLang22.GetBuf());
			Str strLang23 = Str(cc::LangCode::UNICODE16, _T("0123456789ABCDEFGHIJ"), 5, 10);
			TESTASSERT_EQUAL(L"56789ABCDE", (wchar_t *)strLang23.GetBuf());
			Str strLang24 = Str(cc::LangCode::UNICODE16, (wchar_t*)NULL, 5, 10);
			TESTASSERT_TRUE(strLang24.IsNULL());
			TESTASSERT_NULL((wchar_t *)strLang24.GetBuf());
			//from Str
			Str strstrLang21 = Str(cc::LangCode::UNICODE16, strstr);
			TESTASSERT_EQUAL(L"0123456789ABCDEFGHIJ", (wchar_t *)strstrLang21.GetBuf());
			Str strstrLang22 = Str(cc::LangCode::UNICODE16, strstr, 5);
			TESTASSERT_EQUAL(L"56789ABCDEFGHIJ", (wchar_t *)strstrLang22.GetBuf());
			Str strstrLang23 = Str(cc::LangCode::UNICODE16, strstr, 5, 10);
			TESTASSERT_EQUAL(L"56789ABCDE", (wchar_t *)strstrLang23.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of nMinCapacity and incrementMultiPercent")
		{
			//nMinCapacity and incrementMultiPercent
			Str strCapacity1(11, 1.3);
			TESTASSERT_EQUAL(11, strCapacity1.GetMinCapacity());
			TESTASSERT_EQUAL((float)1.3, strCapacity1.GetIncPercent());
			Str strCapacity2(11, 1.5);
			TESTASSERT_EQUAL(11, strCapacity2.GetMinCapacity());
			TESTASSERT_EQUAL((float)1.5, strCapacity2.GetIncPercent());
			//not change for IncMultiPercent < 1
			Str strCapacity3(11, (float)0.9);
			TESTASSERT_EQUAL(11, strCapacity3.GetMinCapacity());
			TESTASSERT_EQUAL((float)1.3, strCapacity3.GetIncPercent());
			Str strCapacity4(cc::LangCode::UNICODE16, (unsigned int)11);
			TESTASSERT_EQUAL(11, strCapacity4.GetMinCapacity());
			TESTASSERT_EQUAL(cc::LangCode::UNICODE16, strCapacity4.GetLangCode());
			Str strCapacity5(cc::LangCode::UNICODE16, (unsigned int)11, 2.0);
			TESTASSERT_EQUAL(11, strCapacity5.GetMinCapacity());
			TESTASSERT_EQUAL((float)2.0, strCapacity5.GetIncPercent());
			TESTASSERT_EQUAL(cc::LangCode::UNICODE16, strCapacity5.GetLangCode());

			Str str_default;
			TESTASSERT_EQUAL_MSG("default MinCapacity is not right", 0, str_default.GetMinCapacity());
			TESTASSERT_EQUAL_MSG("default IncMultiPercent is not right", (float)1.3, str_default.GetIncPercent());
			TESTASSERT_EQUAL_MSG("default RealCapacity is not right", 0, str_default.GetRealCapacity());
			TESTASSERT_TRUE(str_default.IsNULL());

			Str str = Str(100, 1.5);
			TESTASSERT_EQUAL_MSG("MinCapacity is not right", 100, str.GetMinCapacity());
			TESTASSERT_EQUAL_MSG("IncMultiPercent is not right", (float)1.5, str.GetIncPercent());
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 100, str.GetRealCapacity());
			TESTASSERT_FALSE(str.IsNULL());

			//if incrementMultiPercent <1 then not take it,and not malloc buffer this time but when buffer changed.
			str.SetCapacity(0, 0.5);
			TESTASSERT_EQUAL_MSG("MinCapacity is not right", 0, str.GetMinCapacity());
			TESTASSERT_EQUAL_MSG("IncMultiPercent is not right", (float)1.5, str.GetIncPercent());
			TESTASSERT_TRUE(str_default.IsNULL());

			//if incrementMultiPercent <1 then not take it,and not malloc buffer this time but when buffer changed.
			str.SetCapacity(20, (float)1.6);
			TESTASSERT_EQUAL_MSG("MinCapacity is not right", 20, str.GetMinCapacity());
			TESTASSERT_EQUAL_MSG("IncMultiPercent is not right", (float)1.6, str.GetIncPercent());
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 20, str.GetRealCapacity());
			TESTASSERT_FALSE(str.IsNULL());

			str = _T("abc");
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 20, str.GetRealCapacity());
			str = _T("abcABC");
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 20, str.GetRealCapacity());
			str.Repeat(_T("xyZ"), 6);
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 20, str.GetRealCapacity());
			str.Repeat(_T("xyZ"), 8);
			//3*8=24,24*1.6=38.4(so is 38)
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 38, str.GetRealCapacity());
			str.Repeat(_T("abCD"), 8);
			//4*8=32,32<38(so not change)
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 38, str.GetRealCapacity());
			str.Repeat(_T("abCDE"), 8);
			//5*8=40,40*1.6=64(so is 64)
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 64, str.GetRealCapacity());
			str.Repeat(_T("abCDEf"), 8);
			//6*8=48,48<64(so not change)
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 64, str.GetRealCapacity());
			str.Replace(_T("abCDEf"), _T("abCDEfg"));
			//7*8=56,56<64(so not change)
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 64, str.GetRealCapacity());
			str.Remove(40);
			//now is 40,40<64(real),and <(40*1.6=64)(so not change)
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 64, str.GetRealCapacity());
			str.Remove(30);
			//now is 30,30<64 but 64(real)>(30*1.6=48)(so change to 48)
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 48, str.GetRealCapacity());
			str.Append(_T("X"));
			//now is 31,31<48(so not change)
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 48, str.GetRealCapacity());
			str.Insert(0, _T("X"));
			//now is 32,32<48(so not change)
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 48, str.GetRealCapacity());
			str.CompactStringBuf();
			//Compact to 32
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 32, str.GetRealCapacity());
			str.Remove(10);
			//now is 10,but MinCapacity is 20,so,(20*1.6=32),change to 32
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 32, str.GetRealCapacity());
			str.CompactStringBuf();
			//Compact to 10
			TESTASSERT_EQUAL_MSG("RealCapacity is not right", 10, str.GetRealCapacity());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of operator==,!=,>,>=,<,<=(const TCHAR*/Str& str)")
		{
			const TCHAR *tcnull = NULL;
			const TCHAR *tc_a = _T("aaba");
			const TCHAR *tc_b = _T("aabb");
			const TCHAR *tc_c = _T("aabc");

			Str streq1;
			Str streq2;
			TESTASSERT_TRUE(streq1 == streq2);
			TESTASSERT_FALSE(streq1 != streq2);
			//can not be:
			//TESTASSERT_TRUE(tcnull == streq1);
			TESTASSERT_TRUE(streq1 == tcnull);
			TESTASSERT_FALSE(streq1 != tcnull);
			TESTASSERT_FALSE(streq1 > tcnull);
			TESTASSERT_TRUE(streq1 >= tcnull);
			TESTASSERT_FALSE(streq1 < tcnull);
			TESTASSERT_TRUE(streq1 <= tcnull);

			Str streq3 = _T("aabb");
			Str streq4 = _T("aabb");
			TESTASSERT_TRUE(streq3 == streq4);
			TESTASSERT_FALSE(streq3 != streq4);
			TESTASSERT_TRUE(streq3 == tc_b);
			TESTASSERT_FALSE(streq3 != tc_b);
			TESTASSERT_FALSE(streq3 > tc_b);
			TESTASSERT_TRUE(streq3 >= tc_b);
			TESTASSERT_FALSE(streq3 < tc_b);
			TESTASSERT_TRUE(streq3 <= tc_b);

			TESTASSERT_FALSE(streq3 == tc_a);
			TESTASSERT_TRUE(streq3 != tc_a);
			TESTASSERT_TRUE(streq3 > tc_a);
			TESTASSERT_TRUE(streq3 >= tc_a);
			TESTASSERT_FALSE(streq3 < tc_a);
			TESTASSERT_FALSE(streq3 <= tc_a);
			TESTASSERT_FALSE(streq3 == tc_c);
			TESTASSERT_TRUE(streq3 != tc_c);
			TESTASSERT_FALSE(streq3 > tc_c);
			TESTASSERT_FALSE(streq3 >= tc_c);
			TESTASSERT_TRUE(streq3 < tc_c);
			TESTASSERT_TRUE(streq3 <= tc_c);

			Str strchar = Str(cc::LangCode::SYSTEM, "aabb");
			Str strwchar1 = Str(cc::LangCode::UNICODE16, L"aabb");
			TESTASSERT_TRUE(strchar == strwchar1);
			TESTASSERT_FALSE(strchar != strwchar1);
			TESTASSERT_TRUE(strchar == tc_b);
			TESTASSERT_FALSE(strchar != tc_b);
			TESTASSERT_TRUE(strwchar1 == tc_b);
			TESTASSERT_FALSE(strwchar1 != tc_b);
			TESTASSERT_FALSE(strchar > tc_b);
			TESTASSERT_TRUE(strchar >= tc_b);
			TESTASSERT_FALSE(strchar < tc_b);
			TESTASSERT_TRUE(strchar <= tc_b);

			TESTASSERT_FALSE(strchar == tc_a);
			TESTASSERT_TRUE(strchar != tc_a);
			TESTASSERT_TRUE(strchar > tc_a);
			TESTASSERT_TRUE(strchar >= tc_a);
			TESTASSERT_FALSE(strchar < tc_a);
			TESTASSERT_FALSE(strchar <= tc_a);
			TESTASSERT_FALSE(strchar == tc_c);
			TESTASSERT_TRUE(strchar != tc_c);
			TESTASSERT_FALSE(strchar > tc_c);
			TESTASSERT_FALSE(strchar >= tc_c);
			TESTASSERT_TRUE(strchar < tc_c);
			TESTASSERT_TRUE(strchar <= tc_c);

			TESTASSERT_FALSE(strwchar1 == tc_a);
			TESTASSERT_TRUE(strwchar1 != tc_a);
			TESTASSERT_TRUE(strwchar1 > tc_a);
			TESTASSERT_TRUE(strwchar1 >= tc_a);
			TESTASSERT_FALSE(strwchar1 < tc_a);
			TESTASSERT_FALSE(strwchar1 <= tc_a);
			TESTASSERT_FALSE(strwchar1 == tc_c);
			TESTASSERT_TRUE(strwchar1 != tc_c);
			TESTASSERT_FALSE(strwchar1 > tc_c);
			TESTASSERT_FALSE(strwchar1 >= tc_c);
			TESTASSERT_TRUE(strwchar1 < tc_c);
			TESTASSERT_TRUE(strwchar1 <= tc_c);

			Str str_a = Str("aaba");
			Str str_b = Str("aabb");
			Str str_c = Str("aabc");
			//Str<->Str:char
			TESTASSERT_TRUE(strchar == str_b);
			TESTASSERT_FALSE(strchar != str_b);
			TESTASSERT_TRUE(strchar == str_b);
			TESTASSERT_FALSE(strchar != str_b);
			TESTASSERT_FALSE(strchar > str_b);
			TESTASSERT_TRUE(strchar >= str_b);
			TESTASSERT_FALSE(strchar < str_b);
			TESTASSERT_TRUE(strchar <= str_b);
			TESTASSERT_FALSE(strchar == str_a);
			TESTASSERT_TRUE(strchar != str_a);
			TESTASSERT_TRUE(strchar > str_a);
			TESTASSERT_TRUE(strchar >= str_a);
			TESTASSERT_FALSE(strchar < str_a);
			TESTASSERT_FALSE(strchar <= str_a);
			TESTASSERT_FALSE(strchar == str_c);
			TESTASSERT_TRUE(strchar != str_c);
			TESTASSERT_FALSE(strchar > str_c);
			TESTASSERT_FALSE(strchar >= str_c);
			TESTASSERT_TRUE(strchar < str_c);
			TESTASSERT_TRUE(strchar <= str_c);
			//Str<->Str:wchar_t
			TESTASSERT_TRUE(strwchar1 == str_b);
			TESTASSERT_FALSE(strwchar1 != str_b);
			TESTASSERT_TRUE(strwchar1 == str_b);
			TESTASSERT_FALSE(strwchar1 != str_b);
			TESTASSERT_FALSE(strwchar1 > str_b);
			TESTASSERT_TRUE(strwchar1 >= str_b);
			TESTASSERT_FALSE(strwchar1 < str_b);
			TESTASSERT_TRUE(strwchar1 <= str_b);
			TESTASSERT_FALSE(strwchar1 == str_a);
			TESTASSERT_TRUE(strwchar1 != str_a);
			TESTASSERT_TRUE(strwchar1 > str_a);
			TESTASSERT_TRUE(strwchar1 >= str_a);
			TESTASSERT_FALSE(strwchar1 < str_a);
			TESTASSERT_FALSE(strwchar1 <= str_a);
			TESTASSERT_FALSE(strwchar1 == str_c);
			TESTASSERT_TRUE(strwchar1 != str_c);
			TESTASSERT_FALSE(strwchar1 > str_c);
			TESTASSERT_FALSE(strwchar1 >= str_c);
			TESTASSERT_TRUE(strwchar1 < str_c);
			TESTASSERT_TRUE(strwchar1 <= str_c);
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of operator=,+=,+(const TCHAR*/Str& str)")
		{
			//operator=
			Str str = _T("");
			TESTASSERT_EQUAL(_T(""), (TCHAR *)str.GetBuf());
			Str str2 = _T("This is cs2.");
			str = str2;
			TESTASSERT_EQUAL(_T("This is cs2."), (TCHAR *)str.GetBuf());

			//operator+=:NULL + str + NULL,...
			str = (TCHAR*)NULL;
			TESTASSERT_TRUE(str.IsNULL());
			TESTASSERT_NULL(str.GetBuf());
			str += _T("(string1)");
			TESTASSERT_EQUAL(_T("(string1)"), (TCHAR *)str.GetBuf());
			str += (TCHAR*)NULL;
			TESTASSERT_EQUAL(_T("(string1)"), (TCHAR *)str.GetBuf());
			str += Str();
			TESTASSERT_EQUAL(_T("(string1)"), (TCHAR *)str.GetBuf());
			str += _T("(string2)");
			TESTASSERT_EQUAL(_T("(string1)(string2)"), (TCHAR *)str.GetBuf());
			str2 = _T("(string3)");
			str += str2;
			TESTASSERT_EQUAL(_T("(string1)(string2)(string3)"), (TCHAR *)str.GetBuf());

			//str = _T("(string1)");
			//str += (long)-123456789;
			//TESTASSERT_EQUAL(_T("(string1)-123456789"), (TCHAR *)str.GetBuf());
			//str += (long)99.000;
			//TESTASSERT_EQUAL(_T("(string1)-12345678999"), (TCHAR *)str.GetBuf());

			//+=long:it is hard fot test of Digits
			//for float is auto turn to double,and Digits is changed,so not use it!
			//str = _T("(string1)");
			//str += (float)-123456789.012;
			//TESTASSERT_EQUAL(_T("(string1)-123456789.012"), (TCHAR *)str.GetBuf());

			////actually the Digits is only 18byte!!
			////double nDouble = (double)0.12345678901234567;
			//str = _T("(string1)");
			//str += (double)-123456789;
			//TESTASSERT_EQUAL(_T("(string1)-123456789"), (TCHAR *)str.GetBuf());
			//str += (double)99.000;
			//TESTASSERT_EQUAL(_T("(string1)-12345678999"), (TCHAR *)str.GetBuf());

			str2 = _T("(string1)");
			str = str2 + _T("(string2)");
			TESTASSERT_EQUAL(_T("(string1)(string2)"), (TCHAR *)str.GetBuf());

			//operator+
			str2 = _T("(string1)");
			str = _T("(string2)") + str2;
			TESTASSERT_EQUAL(_T("(string2)(string1)"), (TCHAR *)str.GetBuf());

			str2 = _T("(string1)");
			str = _T("(string2)");
			str = str2 + str;
			TESTASSERT_EQUAL(_T("(string1)(string2)"), (TCHAR *)str.GetBuf());

			//operator with char
			str2 = _T("(string1)");
			str = cc::Str();
			str = str2 + 'A';
			TESTASSERT_EQUAL(_T("(string1)A"), (TCHAR *)str.GetBuf());
			str = str + 'B';
			TESTASSERT_EQUAL(_T("(string1)AB"), (TCHAR *)str.GetBuf());
			str += 'C';
			TESTASSERT_EQUAL(_T("(string1)ABC"), (TCHAR *)str.GetBuf());

			str2 = _T("(string1)");
			str = cc::Str();
			str = str2 + 0;
			TESTASSERT_EQUAL(_T("(string1)0"), (TCHAR *)str.GetBuf());
			str = str + 1;
			TESTASSERT_EQUAL(_T("(string1)01"), (TCHAR *)str.GetBuf());
			str += 2;
			TESTASSERT_EQUAL(_T("(string1)012"), (TCHAR *)str.GetBuf());

			str2 = _T("(string1)");
			str = cc::Str();
			str = str2 + 0;
			TESTASSERT_EQUAL(_T("(string1)0"), (TCHAR *)str.GetBuf());
			str = str + -1;
			TESTASSERT_EQUAL(_T("(string1)0-1"), (TCHAR *)str.GetBuf());
			str += -2;
			TESTASSERT_EQUAL(_T("(string1)0-1-2"), (TCHAR *)str.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of Clear()/GetLength()")
		{
			Str str = _T("0123456789ABCDEFGHIJ");
			TESTASSERT_EQUAL(20, str.GetLength());
			str.Clear();
			TESTASSERT_TRUE(str.IsNULL());
			TESTASSERT_EQUAL(0, str.GetLength());

			//length of one chinese word is (UNICODE ? 1 : 2)
			Str str2 = (TCHAR*)NULL;
			TESTASSERT_EQUAL(0, str2.GetLength());
			Str str3 = _T("01234567890123456789");
			TESTASSERT_EQUAL(20, str3.GetLength());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of Pad/PadLeft/PadRight(int totalWidth, const char paddingChar, LangCode fromLangCode = LangCode::SYSTEM)")
		{
			Str str;
			TESTASSERT_TRUE(str.IsNULL());
			str.PadLeft(3, _T('x'));
			TESTASSERT_EQUAL(_T("xxx"), (TCHAR *)str.GetBuf());
			str.PadRight(6, _T('y'));
			TESTASSERT_EQUAL(_T("xxxyyy"), (TCHAR *)str.GetBuf());

			Str str2 = _T("XY");
			str2.PadLeft(5, _T('x'));
			TESTASSERT_EQUAL(_T("xxxXY"), (TCHAR *)str2.GetBuf());
			str2.PadRight(8, _T('y'));
			TESTASSERT_EQUAL(_T("xxxXYyyy"), (TCHAR *)str2.GetBuf());

			Str str3 = _T("XY");
			//padleft:3, padright:4
			str3.Pad(9, _T('x'));
			TESTASSERT_EQUAL(_T("xxxXYxxxx"), (TCHAR *)str3.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of CompareTo(const TCHAR*/Str&, bool IgnoreCase = false)")
		{
			Str str = _T("abcd");
			TESTASSERT_EQUAL(1, str.CompareTo(_T("abcb")));
			TESTASSERT_EQUAL(1, str.CompareTo(_T("abcc")));
			TESTASSERT_EQUAL(0, str.CompareTo(_T("abcd")));
			TESTASSERT_EQUAL(-1, str.CompareTo(_T("abce")));
			TESTASSERT_EQUAL(-1, str.CompareTo(_T("abcf")));
			TESTASSERT_EQUAL(1, str.CompareTo(_T("abcB")));
			TESTASSERT_EQUAL(1, str.CompareTo(_T("abcC")));
			TESTASSERT_EQUAL(1, str.CompareTo(_T("abcD")));
			TESTASSERT_EQUAL(1, str.CompareTo(_T("abcE")));
			TESTASSERT_EQUAL(1, str.CompareTo(_T("abcF")));

			TESTASSERT_EQUAL(1, str.CompareTo(_T("abcB"), true));
			TESTASSERT_EQUAL(1, str.CompareTo(_T("abcC"), true));
			TESTASSERT_EQUAL(0, str.CompareTo(_T("abcD"), true));
			TESTASSERT_EQUAL(-1, str.CompareTo(_T("abcE"), true));
			TESTASSERT_EQUAL(-1, str.CompareTo(_T("abcF"), true));

			Str str_d = _T("abcd");
			Str str_D = _T("abcD");
			Str str_b = _T("abcb");
			Str str_B = _T("abcB");
			Str str_C = _T("abcC");
			Str str_f = _T("abcf");
			Str str_E = _T("abcE");
			Str str_F = _T("abcF");
			TESTASSERT_EQUAL(0, str.CompareTo(str_d));
			TESTASSERT_EQUAL(1, str.CompareTo(str_b));
			TESTASSERT_EQUAL(-1, str.CompareTo(str_f));
			TESTASSERT_EQUAL(1, str.CompareTo(str_D));
			TESTASSERT_EQUAL(1, str.CompareTo(str_B));
			TESTASSERT_EQUAL(1, str.CompareTo(str_F));

			TESTASSERT_EQUAL(1, str.CompareTo(str_B, true));
			TESTASSERT_EQUAL(1, str.CompareTo(str_C, true));
			TESTASSERT_EQUAL(0, str.CompareTo(str_D, true));
			TESTASSERT_EQUAL(-1, str.CompareTo(str_E, true));
			TESTASSERT_EQUAL(-1, str.CompareTo(str_F, true));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of bool StartsWith(const char*/wchar_t*/Str& prefix, int fromIndex, bool IgnoreCase = false) const")
		{
			Str str = _T("abCxyZ");
			TESTASSERT_TRUE(str.StartsWith(_T("abC")));
			TESTASSERT_FALSE(str.StartsWith(_T("abc")));
			TESTASSERT_TRUE(str.StartsWith(_T("abc"), true));
			TESTASSERT_TRUE(str.StartsWith(_T("xyZ"), 3));
			TESTASSERT_TRUE(str.StartsWith(_T("xYz"), 3, true));
			Str str2 = _T("bC");
			TESTASSERT_TRUE(str.StartsWith(str2, 1));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of bool EndsWith(const char*/wchar_t*/Str& suffix, int endLength, bool IgnoreCase = false) const")
		{
			Str str = _T("abCxyZ");
			TESTASSERT_TRUE(str.EndsWith(_T("xyZ")));
			TESTASSERT_FALSE(str.EndsWith(_T("xyz")));
			TESTASSERT_TRUE(str.EndsWith(_T("xyz"), true));
			TESTASSERT_TRUE(str.EndsWith(_T("xy"), str.GetLength() - 1));
			TESTASSERT_FALSE(str.EndsWith(_T("XY"), str.GetLength() - 1));
			TESTASSERT_TRUE(str.EndsWith(_T("XY"), str.GetLength() - 1, true));
			Str str2 = _T("cx");
			TESTASSERT_FALSE(str.EndsWith(str2, 4));
			TESTASSERT_TRUE(str.EndsWith(str2, 4, true));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of bool Equals(const char*/wchar_t*/Str& str, bool IgnoreCase = false) const")
		{
			Str str = _T("aB ");
			TESTASSERT_TRUE(str.Equals(_T("aB ")));
			TESTASSERT_FALSE(str.Equals(_T("ab ")));
			TESTASSERT_TRUE(str.Equals(_T("ab "), true));
			Str str2 = _T("aB ");
			Str str3 = _T("ab ");
			TESTASSERT_TRUE(str.Equals(str2));
			TESTASSERT_FALSE(str.Equals(str3));
			TESTASSERT_TRUE(str.Equals(str3, true));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of int IndexOf(const char*/wchar_t*/Str& str, int fromIndex, bool IgnoreCase = false) const")
		{
			Str str = _T("xyzxYZxyZXyzxyZxyz");
			TESTASSERT_EQUAL(6, str.IndexOf(_T("xyZ")));
			TESTASSERT_EQUAL(0, str.IndexOf(_T("xyZ"), true));
			TESTASSERT_EQUAL(15, str.IndexOf(_T("xyz"), 10));
			TESTASSERT_EQUAL(12, str.IndexOf(_T("xyz"), 10, true));
			Str str2 = _T("YZx");
			TESTASSERT_EQUAL(4, str.IndexOf(str2));

			TESTASSERT_EQUAL(-1, str.IndexOf(str2, 100));
			TESTASSERT_EQUAL(-1, str.IndexOf(str2, -2));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of int IndexOf(const char*/wchar_t*/Str& str, int fromIndex, int count, bool IgnoreCase = false) const")
		{
			Str str = _T("xyzxYZxyZXyzxyZxyz");
			TESTASSERT_EQUAL(-1, str.IndexOf(_T("xyZ"), 0, 5));
			TESTASSERT_EQUAL(6, str.IndexOf(_T("xyZ"), 0, 10));
			TESTASSERT_EQUAL(-1, str.IndexOf(_T("xyZ"), 0, 2, true));
			TESTASSERT_EQUAL(0, str.IndexOf(_T("xyZ"), 0, 5, true));
			TESTASSERT_EQUAL(-1, str.IndexOf(_T("xyz"), 10, 5));
			TESTASSERT_EQUAL(15, str.IndexOf(_T("xyz"), 10, 10));
			TESTASSERT_EQUAL(-1, str.IndexOf(_T("xyz"), 10, 2, true));
			TESTASSERT_EQUAL(12, str.IndexOf(_T("xyz"), 10, 5, true));
			Str str2 = _T("YZx");
			TESTASSERT_EQUAL(4, str.IndexOf(str2));
			TESTASSERT_EQUAL(4, str.IndexOf(str2, 0, 10));
			//even over length,no exception
			TESTASSERT_EQUAL(4, str.IndexOf(str2, 0, 100));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of int LastIndexOf(const char*/wchar_t*/Str& str, int endLength, bool IgnoreCase = false) const")
		{
			Str str = _T("012abc6789012ABC6789");
			TESTASSERT_EQUAL(3, str.LastIndexOf(_T("abc")));
			TESTASSERT_EQUAL(13, str.LastIndexOf(_T("abc"), true));
			TESTASSERT_EQUAL(-1, str.LastIndexOf(_T("aBc"), 10));
			TESTASSERT_EQUAL(3, str.LastIndexOf(_T("aBc"), 10, true));
			Str str2 = _T("abc");
			TESTASSERT_EQUAL(3, str.LastIndexOf(str2));

			TESTASSERT_EQUAL(-1, str.LastIndexOf(str2, -2));
			TESTASSERT_EQUAL(3, str.LastIndexOf(str2, 100));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of Replace(const TCHAR*/Str& oldStr, const TCHAR*/Str& newStr, int fromIndex, bool IgnoreCase = false)")
		{
			Str str = _T("aBcD");
			str.Replace(_T("aBcD"), _T(""));
			TESTASSERT_EQUAL(0, str.GetLength());
			TESTASSERT_EQUAL(_T(""), (TCHAR *)str.GetBuf());

			str = _T("aBcD");
			str.Replace(_T("aBcD"), _T("x"));
			TESTASSERT_EQUAL(_T("x"), (TCHAR *)str.GetBuf());

			str = _T("aBcDe");
			str.Replace(_T("aBcD"), _T("vWxYz"));
			TESTASSERT_EQUAL(_T("vWxYze"), (TCHAR *)str.GetBuf());

			str = _T("aBcDe");
			str.Replace(_T("aBcD"), _T("vWxYzvWxYzvWxYzvWxYz"));
			TESTASSERT_EQUAL(_T("vWxYzvWxYzvWxYzvWxYze"), (TCHAR *)str.GetBuf());

			str = _T("(123)(123)(123)(123)");
			str.Replace(_T("(123)"), _T(""));
			TESTASSERT_EQUAL(_T(""), (TCHAR *)str.GetBuf());

			str = _T("(123)(123)(123)(123)");
			str.Replace(_T("123"), _T("aB"));
			TESTASSERT_EQUAL(_T("(aB)(aB)(aB)(aB)"), (TCHAR *)str.GetBuf());

			str = _T("(123)(123)(123)(123)");
			str.Replace(_T("123"), _T("aBcD"));
			TESTASSERT_EQUAL(_T("(aBcD)(aBcD)(aBcD)(aBcD)"), (TCHAR *)str.GetBuf());

			str = _T("(123)(123)(123)(123)");
			str.Replace(_T("123"), _T("aBcD"), 12);
			TESTASSERT_EQUAL(_T("(123)(123)(123)(aBcD)"), (TCHAR *)str.GetBuf());

			str = _T("(123)(123)(123)(123)");
			str.Replace(_T("123"), _T("aBcD"), 100);
			TESTASSERT_EQUAL(_T("(123)(123)(123)(123)"), (TCHAR *)str.GetBuf());

			str = _T("(123)(123)(123)(123)");
			str.Replace(_T(""), _T("aBcD"));
			TESTASSERT_EQUAL(_T("(123)(123)(123)(123)"), (TCHAR *)str.GetBuf());

			str = _T("(123)(123)(123)(123)");
			Str str2 = _T("(123");
			Str str3 = _T("(x");
			str.Replace(str2, str3);
			TESTASSERT_EQUAL(_T("(x)(x)(x)(x)"), (TCHAR *)str.GetBuf());

			str = _T("(123)(123)(123)(123)");
			str2 = _T("123)");
			str3 = _T("xyzX)");
			str.Replace(str2, str3, -1);
			TESTASSERT_EQUAL(_T("(xyzX)(xyzX)(xyzX)(xyzX)"), (TCHAR *)str.GetBuf());

			str = _T("xyz,xyZ,xyz,xyZ,xYz,Xyz");
			str2 = _T("xyz");
			str3 = _T("ABCD");
			str.Replace(str2, str3);
			TESTASSERT_EQUAL(_T("ABCD,xyZ,ABCD,xyZ,xYz,Xyz"), (TCHAR *)str.GetBuf());

			str = _T("xyz,xyZ,xyz,xyZ,xYz,Xyz");
			str2 = _T("xyz");
			str3 = _T("ABCD");
			str.Replace(str2, str3, true);
			TESTASSERT_EQUAL(_T("ABCD,ABCD,ABCD,ABCD,ABCD,ABCD"), (TCHAR *)str.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of SplitBy(const TCHAR*/Str& str, bool IgnoreCase = false)")
		{
			Str str = _T("xxx,yyy,zzz");
			Str str2 = str.SplitBy(_T(","));
			TESTASSERT_EQUAL(_T("yyy,zzz"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL(_T("xxx"), (TCHAR *)str2.GetBuf());

			//str = yyy,zzz
			Str Splitchar = _T(",");
			str2 = str.SplitBy(Splitchar);
			TESTASSERT_EQUAL(_T("zzz"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL(_T("yyy"), (TCHAR *)str2.GetBuf());

			//str = zzz
			str2 = str.SplitBy(Splitchar);
			TESTASSERT_EQUAL((TCHAR *)NULL, (TCHAR *)str.GetBuf());
			TESTASSERT_NULL((TCHAR *)str.GetBuf());
			TESTASSERT_TRUE(str.IsNULL());
			TESTASSERT_EQUAL(_T("zzz"), (TCHAR *)str2.GetBuf());

			//str = NULL
			str2 = str.SplitBy(Splitchar);
			TESTASSERT_EQUAL((TCHAR *)NULL, (TCHAR *)str.GetBuf());
			TESTASSERT_NULL((TCHAR *)str.GetBuf());
			TESTASSERT_TRUE(str.IsNULL());
			TESTASSERT_EQUAL((TCHAR *)NULL, (TCHAR *)str2.GetBuf());
			TESTASSERT_NULL((TCHAR *)str2.GetBuf());
			TESTASSERT_TRUE(str2.IsNULL());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of Split(const TCHAR*/Str& Splitstr, Str **str, bool IgnoreCase = false)")
		{
			int Splitcnt;
			Str *strArr = NULL;
			Str str = _T("xx1@yy22");
			Splitcnt = str.Split(_T("@"), &strArr);
			TESTASSERT_EQUAL(2, Splitcnt);
			TESTASSERT_EQUAL(_T("xx1"), (TCHAR *)strArr[0].GetBuf());
			TESTASSERT_EQUAL(_T("yy22"), (TCHAR *)strArr[1].GetBuf());
			//!not forgget:delete []pcs; pcs = NULL; (can not auto!)
			delete []strArr;
			strArr = NULL;
			//old str is not changes
			TESTASSERT_EQUAL(_T("xx1@yy22"), (TCHAR *)str.GetBuf());

			str = _T("xx1@yy22@");
			Splitcnt = str.Split(_T("@"), &strArr);
			TESTASSERT_EQUAL(2, Splitcnt);
			TESTASSERT_EQUAL(_T("xx1"), (TCHAR *)strArr[0].GetBuf());
			TESTASSERT_EQUAL(_T("yy22"), (TCHAR *)strArr[1].GetBuf());
			//!not forgget:delete []pcs; pcs = NULL; (can not auto!)
			delete []strArr;
			strArr = NULL;
			//old str is not changes
			TESTASSERT_EQUAL(_T("xx1@yy22@"), (TCHAR *)str.GetBuf());

			//count is gaven
			str = _T("xx1@yy22@");
			Splitcnt = str.Split(_T("@"), &strArr, 1);
			TESTASSERT_EQUAL(2, Splitcnt);
			TESTASSERT_EQUAL(_T("xx1"), (TCHAR *)strArr[0].GetBuf());
			TESTASSERT_EQUAL(_T("yy22@"), (TCHAR *)strArr[1].GetBuf());
			//!not forgget:delete []pcs; pcs = NULL; (can not auto!)
			delete []strArr;
			strArr = NULL;
			//old str is not changes
			TESTASSERT_EQUAL(_T("xx1@yy22@"), (TCHAR *)str.GetBuf());

			str = _T("xx1@yy22@zz333@@");
			Splitcnt = str.Split(_T("@"), &strArr);
			TESTASSERT_EQUAL(4, Splitcnt);
			TESTASSERT_EQUAL(_T("xx1"), (TCHAR *)strArr[0].GetBuf());
			TESTASSERT_EQUAL(_T("yy22"), (TCHAR *)strArr[1].GetBuf());
			TESTASSERT_EQUAL(_T("zz333"), (TCHAR *)strArr[2].GetBuf());
			TESTASSERT_EQUAL(_T(""), (TCHAR *)strArr[3].GetBuf());
			//!not forgget:delete []pcs; pcs = NULL; (can not auto!)
			delete []strArr;
			strArr = NULL;
			//old str is not changes
			TESTASSERT_EQUAL(_T("xx1@yy22@zz333@@"), (TCHAR *)str.GetBuf());

			//count is gaven
			str = _T("xx1@yy22@zz333@@");
			Splitcnt = str.Split(_T("@"), &strArr, 3);
			TESTASSERT_EQUAL(4, Splitcnt);
			TESTASSERT_EQUAL(_T("xx1"), (TCHAR *)strArr[0].GetBuf());
			TESTASSERT_EQUAL(_T("yy22"), (TCHAR *)strArr[1].GetBuf());
			TESTASSERT_EQUAL(_T("zz333"), (TCHAR *)strArr[2].GetBuf());
			TESTASSERT_EQUAL(_T("@"), (TCHAR *)strArr[3].GetBuf());
			//!not forgget:delete []pcs; pcs = NULL; (can not auto!)
			delete []strArr;
			strArr = NULL;
			//old str is not changes
			TESTASSERT_EQUAL(_T("xx1@yy22@zz333@@"), (TCHAR *)str.GetBuf());

			//IgnoreCase = false
			str = _T("[abC]x1[ABC]x2[abC][ABC][abC]x3[ABC]");
			Splitcnt = str.Split(_T("[abC]"), &strArr);
			TESTASSERT_EQUAL(4, Splitcnt);
			TESTASSERT_EQUAL(_T(""), (TCHAR *)strArr[0].GetBuf());
			TESTASSERT_EQUAL(_T("x1[ABC]x2"), (TCHAR *)strArr[1].GetBuf());
			TESTASSERT_EQUAL(_T("[ABC]"), (TCHAR *)strArr[2].GetBuf());
			TESTASSERT_EQUAL(_T("x3[ABC]"), (TCHAR *)strArr[3].GetBuf());
			//!not forgget:delete []pcs; pcs = NULL; (can not auto!)
			delete []strArr;
			strArr = NULL;
			//old str is not changes
			TESTASSERT_EQUAL(_T("[abC]x1[ABC]x2[abC][ABC][abC]x3[ABC]"), (TCHAR *)str.GetBuf());

			//IgnoreCase = true
			str = _T("[abC]x1[ABC]x2[abC][ABC][abC]x3[ABC]");
			Str Splitchar = _T("[abC]");
			Splitcnt = str.Split(Splitchar, &strArr, true);
			TESTASSERT_EQUAL(6, Splitcnt);
			TESTASSERT_EQUAL(_T(""), (TCHAR *)strArr[0].GetBuf());
			TESTASSERT_EQUAL(_T("x1"), (TCHAR *)strArr[1].GetBuf());
			TESTASSERT_EQUAL(_T("x2"), (TCHAR *)strArr[2].GetBuf());
			TESTASSERT_EQUAL(_T(""), (TCHAR *)strArr[3].GetBuf());
			TESTASSERT_EQUAL(_T(""), (TCHAR *)strArr[4].GetBuf());
			TESTASSERT_EQUAL(_T("x3"), (TCHAR *)strArr[5].GetBuf());
			//!not forgget:delete []pcs; pcs = NULL; (can not auto!)
			delete []strArr;
			strArr = NULL;
			//old str is not changes
			TESTASSERT_EQUAL(_T("[abC]x1[ABC]x2[abC][ABC][abC]x3[ABC]"), (TCHAR *)str.GetBuf());

			cc::Str sBuf = Str(cc::LangCode::UNICODE16, L"Lang=Chinese\r\n\r\n[File]\r\nopen=Open\r\n");
			LangCode::Type retuLangCode = sBuf.GetLangCode();
			Str *sLines = NULL;
			int nLine = sBuf.Split(L"\n", &sLines, 0, false, retuLangCode);
			TESTASSERT_EQUAL(4, nLine);
			TESTASSERT_EQUAL(L"Lang=Chinese\r", (wchar_t *)sLines[0].GetBuf());
			TESTASSERT_EQUAL(L"\r", (wchar_t *)sLines[1].GetBuf());
			TESTASSERT_EQUAL(L"[File]\r", (wchar_t *)sLines[2].GetBuf());
			TESTASSERT_EQUAL(L"open=Open\r", (wchar_t *)sLines[3].GetBuf());
			delete []sLines;
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of Substring(int beginIndex, int endIndex),Left/Mid/Right")
		{
			Str str = _T("01234567890123456789");
			Str strsub = str.Substring(12);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL(_T("23456789"), (TCHAR *)strsub.GetBuf());

			//take to end
			strsub = str.Substring(12, 100);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL(_T("23456789"), (TCHAR *)strsub.GetBuf());

			strsub = str.Substring(-10, 100);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL((TCHAR*)NULL, (TCHAR *)strsub.GetBuf());

			//take to end
			strsub = str.Substring(10, -100);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL(_T("0123456789"), (TCHAR *)strsub.GetBuf());

			strsub = str.Left(5);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL(_T("01234"), (TCHAR *)strsub.GetBuf());

			strsub = str.Left(50);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)strsub.GetBuf());

			strsub = str.Left(-5);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL((TCHAR*)_T(""), (TCHAR *)strsub.GetBuf());

			strsub = str.Right(5);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL(_T("56789"), (TCHAR *)strsub.GetBuf());

			strsub = str.Right(50);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)strsub.GetBuf());

			strsub = str.Right(-5);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL((TCHAR*)_T(""), (TCHAR *)strsub.GetBuf());

			strsub = str.Mid(5);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL(_T("567890123456789"), (TCHAR *)strsub.GetBuf());

			strsub = str.Mid(5, 50);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL(_T("567890123456789"), (TCHAR *)strsub.GetBuf());

			strsub = str.Mid(-5, 50);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL((TCHAR*)NULL, (TCHAR *)strsub.GetBuf());

			strsub = str.Mid(-5);
			TESTASSERT_EQUAL(_T("01234567890123456789"), (TCHAR *)str.GetBuf());
			TESTASSERT_EQUAL((TCHAR*)NULL, (TCHAR *)strsub.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of ToReverse()/ToLower()/ToUpper()")
		{
			Str str = _T("aAbBcC");
			str.ToReverse();
			TESTASSERT_EQUAL(_T("CcBbAa"), (TCHAR *)str.GetBuf());
			str.ToLower();
			TESTASSERT_EQUAL(_T("ccbbaa"), (TCHAR *)str.GetBuf());
			str.ToUpper();
			TESTASSERT_EQUAL(_T("CCBBAA"), (TCHAR *)str.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of Trim(const char*/wchar_t/Str& str)/TrimLeft()/TrimRight()")
		{
			Str str = _T("  abcABC  ");
			str.Trim();
			TESTASSERT_EQUAL(_T("abcABC"), (TCHAR *)str.GetBuf());

			str = _T("  abcABC  ");
			str.TrimRight();
			TESTASSERT_EQUAL(_T("  abcABC"), (TCHAR *)str.GetBuf());

			str = _T("  abcABC  ");
			str.TrimLeft();
			TESTASSERT_EQUAL(_T("abcABC  "), (TCHAR *)str.GetBuf());

			//Trim gaven char
			str = _T(" xX xYxYxY");
			str.TrimRight(_T("xY"));
			TESTASSERT_EQUAL(_T(" xX "), (TCHAR *)str.GetBuf());

			str = _T("xYxYxY xX ");
			Str str2 = _T("xY");
			str.TrimLeft(str2);
			TESTASSERT_EQUAL(_T(" xX "), (TCHAR *)str.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of ValueOf(const TCHAR*/Str&str, int fromIndex, int len),ValueOf(int/char)")
		{
			Str str = (TCHAR*)NULL;
			TESTASSERT_NULL(str.GetBuf());
			str.ValueOf(_T("abc"));
			TESTASSERT_EQUAL(_T("abc"), (TCHAR *)str.GetBuf());

			Str strFrom = _T("def");
			str = (TCHAR*)NULL;
			TESTASSERT_NULL(str.GetBuf());
			str.ValueOf(strFrom);
			TESTASSERT_EQUAL(_T("def"), (TCHAR *)str.GetBuf());

			str.ValueOf(123456);
			TESTASSERT_EQUAL(_T("123456"), (TCHAR *)str.GetBuf());

			str.ValueOf(1234567890);
			TESTASSERT_EQUAL(_T("1234567890"), (TCHAR *)str.GetBuf());

			str.ValueOf(-1234567890);
			TESTASSERT_EQUAL(_T("-1234567890"), (TCHAR *)str.GetBuf());

			str.ValueOf('A');
			TESTASSERT_EQUAL(_T("A"), (TCHAR *)str.GetBuf());

			////not test of Digits
			///////////////////////////////////////////////////////////////////////
			//if(false)
			//{
			////for float is auto turn to double,and Digits is changed,so not use it!
			////so not use it!
			//float nFloat = (float)atof("0.012345678");
			////float nFloat = (float)247.346; //actually:247.34599
			//str.ValueOf(nFloat);
			//TESTASSERT_EQUAL(_T("0.0123456782"), (TCHAR *)str.GetBuf());
			//nFloat = (float)atof("0.0123456785"); //actually:0.012345678
			//str.ValueOf(nFloat);
			//TESTASSERT_EQUAL(_T("0.0123456782"), (TCHAR *)str.GetBuf());
			//nFloat = (float)atof("0.0123456786"); //actually:0.012345678
			//nFloat = (float)atof("0.0123456787"); //actually:0.012345679
			//str.ValueOf(nFloat);
			//TESTASSERT_EQUAL(_T("0.0123456791"), (TCHAR *)str.GetBuf());
			//nFloat = (float)atof("123.012345"); //actually:123.01234
			//str.ValueOf(nFloat);
			//TESTASSERT_EQUAL(_T("123.0123443604"), (TCHAR *)str.GetBuf());
			//nFloat = (float)atof("123.012346"); //actually:123.01234
			//nFloat = (float)atof("123.012347"); //actually:123.01234
			//nFloat = (float)atof("123.012348"); //actually:123.01234
			//nFloat = (float)atof("123.012349"); //actually:123.01235
			//str.ValueOf(nFloat);
			//TESTASSERT_EQUAL(_T("123.0123519897"), (TCHAR *)str.GetBuf());
			//nFloat = (float)atof("123456");
			//str.ValueOf(nFloat);
			//TESTASSERT_EQUAL(_T("123456"), (TCHAR *)str.GetBuf());

			////actually the Digits is only 18byte!!
			//double nDouble = atof("0.012345678901234567");
			////double nDouble = (double)0.1234567890123456; //is auto turn to:0.12345678901234559
			//str.ValueOf(nDouble);
			//TESTASSERT_EQUAL(_T("0.012345678901234567"), (TCHAR *)str.GetBuf());
			//nDouble = atof("0.0123456789012345675"); //actually:0.012345678901234567
			//str.ValueOf(nDouble);
			//TESTASSERT_EQUAL(_T("0.012345678901234567"), (TCHAR *)str.GetBuf());
			//nDouble = atof("0.0123456789012345676"); //actually:0.012345678901234568
			//str.ValueOf(nDouble);
			//TESTASSERT_EQUAL(_T("0.012345678901234568"), (TCHAR *)str.GetBuf());

			//nDouble = atof("12345.012345678901");
			//str.ValueOf(nDouble);
			//TESTASSERT_EQUAL(_T("12345.012345678901"), (TCHAR *)str.GetBuf());
			//nDouble = atof("12345.0123456789015"); //actually:12345.012345678901
			//str.ValueOf(nDouble);
			//TESTASSERT_EQUAL(_T("12345.012345678901"), (TCHAR *)str.GetBuf());
			//nDouble = atof("12345.0123456789016"); //actually:12345.012345678902
			//str.ValueOf(nDouble);
			//TESTASSERT_EQUAL(_T("12345.012345678902"), (TCHAR *)str.GetBuf());

			//nDouble = atof("1234567890.0123456");
			//str.ValueOf(nDouble);
			//TESTASSERT_EQUAL(_T("1234567890.0123456"), (TCHAR *)str.GetBuf());
			//nDouble = atof("1234567890.01234565"); //actually:1234567890.0123456
			//str.ValueOf(nDouble);
			//TESTASSERT_EQUAL(_T("1234567890.0123456"), (TCHAR *)str.GetBuf());
			//str.ValueOf(nDouble);
			//TESTASSERT_EQUAL(_T("1234567890.0123456"), (TCHAR *)str.GetBuf());
			//nDouble = atof("1234567890.01234566"); //actually:1234567890.0123456
			//nDouble = atof("1234567890.01234567"); //actually:1234567890.0123456
			//nDouble = atof("1234567890.01234568"); //actually:1234567890.0123458,not 1234567890.0123457!!!
			//str.ValueOf(nDouble);
			//TESTASSERT_EQUAL(_T("1234567890.0123458"), (TCHAR *)str.GetBuf());

			////if over,then valid len is 18(include +-)
			//nDouble = atof("12345678901234567.01234567"); //actuall:12345678901234568.
			//str.ValueOf(nDouble);
			//TESTASSERT_EQUAL(_T("12345678901234568"), (TCHAR *)str.GetBuf());
			//nDouble = atof("123456789012345678.01234567"); //actuall:1.2345678901234568e+017
			//str.ValueOf(nDouble);
			//TESTASSERT_EQUAL(_T("123456789012345680"), (TCHAR *)str.GetBuf());
			//nDouble = atof("1234567890123456789.01234567"); //actuall:1.2345678901234568e+018
			//str.ValueOf(nDouble);
			//TESTASSERT_EQUAL(_T("1234567890123456800"), (TCHAR *)str.GetBuf());
			//nDouble = atof("12345678901234567890.01234567"); //actuall:1.2345678901234567e+019
			//str.ValueOf(nDouble);
			////12345678901234567000??,!!!
			//TESTASSERT_EQUAL(_T("12345678901234567000"), (TCHAR *)str.GetBuf());
			//}
			////not test of Digits
			///////////////////////////////////////////////////////////////////////

			//double nDouble2 = atof("1234567890");
			//str.ValueOf(nDouble2);
			//TESTASSERT_EQUAL(_T("1234567890"), (TCHAR *)str.GetBuf());

			str.ValueOf(_T("0123456789"));
			TESTASSERT_EQUAL(_T("0123456789"), (TCHAR *)str.GetBuf());
			str.ValueOf(_T("0123456789"), 5);
			TESTASSERT_EQUAL(_T("56789"), (TCHAR *)str.GetBuf());
			str.ValueOf(_T("0123456789"), 5, 2);
			TESTASSERT_EQUAL(_T("56"), (TCHAR *)str.GetBuf());
			str.ValueOf(_T("0123456789"), -5, 2);
			TESTASSERT_EQUAL((TCHAR*)NULL, (TCHAR *)str.GetBuf());
			//take to end
			str.ValueOf(_T("0123456789"), 5, -2);
			TESTASSERT_EQUAL(_T("56789"), (TCHAR *)str.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of Repeat(const TCHAR*/Str& str)")
		{
			Str str = (TCHAR*)NULL;
			TESTASSERT_NULL(str.GetBuf());
			str.Repeat(_T("xY"), 3);
			TESTASSERT_EQUAL(_T("xYxYxY"), (TCHAR *)str.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of FindCount(const TCHAR*/Str& str, bool IgnoreCase = false)")
		{
			Str str = _T("xyz,xyZ,xyz,xyZ,xYz,Xyz");
			TESTASSERT_EQUAL(2, str.FindCount(_T("xyz")));
			TESTASSERT_EQUAL(6, str.FindCount(_T("xyz"), true));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of Append(const TCHAR*/Str& str/int/char)/Insert/Remove")
		{
			Str str = _T("012345");
			str.Append(_T("ABC"));
			TESTASSERT_EQUAL(_T("012345ABC"), (TCHAR *)str.GetBuf());
			str.Insert(3, _T("XYZ"));
			TESTASSERT_EQUAL(_T("012XYZ345ABC"), (TCHAR *)str.GetBuf());
			str.Insert(0, _T("XYZ"));
			TESTASSERT_EQUAL(_T("XYZ012XYZ345ABC"), (TCHAR *)str.GetBuf());

			str = _T("0123456789");
			str.Remove(7);
			TESTASSERT_EQUAL(_T("0123456"), (TCHAR *)str.GetBuf());
			str.Remove(3, 5);
			TESTASSERT_EQUAL(_T("01256"), (TCHAR *)str.GetBuf());

			str = _T("012345");
			str.Append('A');
			TESTASSERT_EQUAL(_T("012345A"), (TCHAR *)str.GetBuf());
			str.Append(0);
			TESTASSERT_EQUAL(_T("012345A0"), (TCHAR *)str.GetBuf());
			str.Append(123);
			TESTASSERT_EQUAL(_T("012345A0123"), (TCHAR *)str.GetBuf());
			str.Append(-123);
			TESTASSERT_EQUAL(_T("012345A0123-123"), (TCHAR *)str.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of Format(const TCHAR* strFormat, ...)")
		{
			Str str = _T("");
			str.Format(_T("(%s)(%.10f)"), _T("ABC"), -12345.6789);
			TESTASSERT_EQUAL(_T("(ABC)(-12345.6789000000)"), (TCHAR *)str.GetBuf());
			str.Format(_T("(%d)(%f)"), -12345, -12345.6789);
			TESTASSERT_EQUAL(_T("(-12345)(-12345.678900)"), (TCHAR *)str.GetBuf());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of ToInt/ToLong/ToFloat/ToDouble")
		{
			Str str = _T("01289.345abc01");
			TESTASSERT_EQUAL(1289, (int)str.ToInt());
			TESTASSERT_EQUAL((long)1289, (long)str.ToLong());
			TESTASSERT_EQUAL((float)1289.345, (float)str.ToFloat());
			TESTASSERT_EQUAL((double)1289.345, (double)str.ToDouble());

			str = _T("0123456789012.345abc");
			//for over of int,will not return right int
			//TESTASSERT_EQUAL(123456789012, (int)str.ToInt());
			//TESTASSERT_EQUAL((long)123456789012.345, (long)str.ToLong());
			//TESTASSERT_EQUAL((float)123456789012.345, (float)str.ToFloat());
			TESTASSERT_EQUAL((double)123456789012.345, (double)str.ToDouble());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of Performance,Append&use buf of tchar,10M(10,000,000 Byte)(see log for elapsed second)")
		{
			//ABCDEFGHIJ123abcdefghij123ABCDEFGHIJ1234abcdefghij * 200000(=10,000,000);
			cc::Log::traceConsole(RGB(255, 0, 0), _T("==============================\r\n"));
			cc::Log::debug(_T("test of Performance,Append buf of tchar to 10M(10,000,000 Byte)"));
			int loopcnt = 200000;
			TCHAR *tc = _T("ABCDEFGHIJ123abcdefghij123ABCDEFGHIJ1234abcdefghij");
			int nTcLen = (int)_tcslen(tc);

			cc::Log::debug(_T("Append TCHAR *Buf,to 50Byte * 200000(=10,000,000,10M)"));
			clock_t tmStart = ::clock ();
			TCHAR* testBuf;
			testBuf = (TCHAR*)malloc((loopcnt + 1) * sizeof(TCHAR) * nTcLen);
			int i;
			for(i = 0; i < loopcnt; i++)
			{
				TESTCASE_ISCANCEL();
				memset(testBuf + nTcLen * i, '\0', nTcLen);
				testBuf[nTcLen * i + 1] = _T('\0');
				//_tcscpy(testBuf + nTcLen * i, tc);
				memcpy(testBuf + nTcLen * i, tc, nTcLen);
				if(i == ((int)(i/20000) * 20000))
				{
					cc::Log::traceConsole(RGB(0, 0, 0), _T("(loop:%d, len:%d) "), i, nTcLen * i);
				}
			}
			cc::Log::traceConsole(RGB(0, 0, 0), _T("\r\n"));
			cc::Log::debug(_T("TCHAR *Buf len:(%d)"), _tcslen(testBuf));
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);

			cc::Log::debug(_T("Append Str use default Capacity&IncMultiPercent,to 50Byte * 200000(=10,000,000,10M)"));
			tmStart = ::clock ();
			//now use Str to Append buf to 10M
			//if u know the resize,recommand use:str.SetCapacity(1024 * 1000 * 11, 1.5)
			Str str = (TCHAR*)NULL;
			//str.SetCapacity(1024 * 1000 * 11, 1.5);
			TESTASSERT_EQUAL(0, str.GetRealCapacity());
			for(i = 0; i < loopcnt/2; i++)
			{
				TESTCASE_ISCANCEL();
				str.Append(tc);
				str += tc;
				if(i == ((int)(i/10000) * 10000))
				{
					cc::Log::traceConsole(RGB(0, 0, 0), _T("(loop:%d, len:%d, RealCapacity:%d) "), i, str.GetLength(), str.GetRealCapacity());
				}
			}
			cc::Log::traceConsole(RGB(0, 0, 0), _T("\r\n"));
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			cc::Log::debug(_T("Elapsed second:[%f]\n"), (double)(clock() - tmStart)/1000);

			cc::Log::debug(_T("Append Str direct from 10M of TCHAR *Buf"));
			tmStart = ::clock ();
			//now use Str load buf of 10M
			str = (TCHAR*)NULL;
			str = testBuf;
			free(testBuf);
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			cc::Log::debug(_T("Elapsed second:[%f]"), (double)(clock() - tmStart)/1000);

			cc::Log::debug(_T("test:Str(buf is 10M) insert to head(this time resize buf)"));
			tmStart = ::clock ();
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			str.Insert(0, _T("XXXXXXXXXX"));
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			cc::Log::debug(_T("Elapsed second:[%f]"), (double)(clock() - tmStart)/1000);

			cc::Log::debug(_T("test:Str(buf is 10M) insert to head(this time without resize buf)"));
			tmStart = ::clock ();
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			str.Insert(0, _T("YYYYYYYYYY"));
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			cc::Log::debug(_T("Elapsed second:[%f]"), (double)(clock() - tmStart)/1000);

			cc::Log::debug(_T("test:Str(buf is 10M) remove of head"));
			tmStart = ::clock ();
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			str.Remove(0, 20);
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			cc::Log::debug(_T("Elapsed second:[%f]\r\n"), (double)(clock() - tmStart)/1000);

			cc::Log::debug(_T("test:Str(buf is 10M) FindCount"));
			tmStart = ::clock ();
			int nCnt = str.FindCount(_T("ABCDEFGHIJ"));
			cc::Log::debug(_T("FindCount:%d, with Str length:%d, RealCapacity:%d"), nCnt, str.GetLength(), str.GetRealCapacity());
			TESTASSERT_EQUAL(200000, nCnt);
			cc::Log::debug(_T("Elapsed second:[%f]"), (double)(clock() - tmStart)/1000);

			cc::Log::debug(_T("test:Str(buf is 10M) FindCount with IgnoreCase"));
			tmStart = ::clock ();
			nCnt = str.FindCount(_T("ABCDEFGHIJ"), true);
			cc::Log::debug(_T("FindCount:%d, with Str length:%d, RealCapacity:%d"), nCnt, str.GetLength(), str.GetRealCapacity());
			TESTASSERT_EQUAL(400000, nCnt);
			cc::Log::debug(_T("Elapsed second:[%f]\r\n"), (double)(clock() - tmStart)/1000);

			cc::Log::debug(_T("test:Str(buf is 10M) Replace 10 to 20"));
			tmStart = ::clock ();
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			str.Replace(_T("ABCDEFGHIJ"), _T("A@B@C@D@E@F@G@H@I@J@"));
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			cc::Log::debug(_T("Elapsed second:[%f]"), (double)(clock() - tmStart)/1000);

			cc::Log::debug(_T("test:Str(buf is 10M) Replace 10 to 20 with IgnoreCase"));
			tmStart = ::clock ();
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			str.Replace(_T("abcdefghij"), _T("a@b@c@d@e@f@g@h@i@j@"), true);
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			cc::Log::debug(_T("Elapsed second:[%f]\r\n"), (double)(clock() - tmStart)/1000);

			cc::Log::debug(_T("test:Str(buf is 10M) Replace 20 to 10"));
			tmStart = ::clock ();
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			str.Replace(_T("A@B@C@D@E@F@G@H@I@J@"), _T("QRSTUVWXYZ"));
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			cc::Log::debug(_T("Elapsed second:[%f]"), (double)(clock() - tmStart)/1000);

			cc::Log::debug(_T("test:Str(buf is 10M) Replace 20 to 10 with IgnoreCase"));
			tmStart = ::clock ();
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			str.Replace(_T("a@b@c@d@e@f@g@h@i@j@"), _T("qrstuvwxyz"), true);
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			cc::Log::debug(_T("Elapsed second:[%f]\r\n"), (double)(clock() - tmStart)/1000);

			str.Append(_T("XXXyyyZZZ@XXXYYYzzZ@"));
			str.Insert(0, _T("XXXmmmZZZ@XXXMMMzzZ@"));
			cc::Log::debug(_T("test:Str(buf is 10M) IndexOf()(seek to end)"));
			cc::Log::debug(_T("Str length:%d, RealCapacity:%d"), str.GetLength(), str.GetRealCapacity());
			tmStart = ::clock ();
			TESTASSERT_EQUAL(10000082, str.IndexOf(_T("XXXYYYzzZ")));
			cc::Log::debug(_T("Elapsed second:[%f]"), (double)(clock() - tmStart)/1000);
			cc::Log::debug(_T("test:Str(buf is 10M) IndexOf() with IgnoreCase(seek to end)"));
			tmStart = ::clock ();
			TESTASSERT_EQUAL(10000072, str.IndexOf(_T("XXXYYYzzZ"), true));
			cc::Log::debug(_T("Elapsed second:[%f]"), (double)(clock() - tmStart)/1000);
			cc::Log::debug(_T("test:Str(buf is 10M) LastIndexOf()(seek to head)"));
			tmStart = ::clock ();
			TESTASSERT_EQUAL(0, str.LastIndexOf(_T("XXXmmmZZZ")));
			cc::Log::debug(_T("Elapsed second:[%f]"), (double)(clock() - tmStart)/1000);
			cc::Log::debug(_T("test:Str(buf is 10M) LastIndexOf() with IgnoreCase(seek to head)"));
			tmStart = ::clock ();
			TESTASSERT_EQUAL(10, str.LastIndexOf(_T("XXXmmmZZZ"), true));
			cc::Log::debug(_T("Elapsed second:[%f]"), (double)(clock() - tmStart)/1000);
			cc::Log::traceConsole(RGB(255, 0, 0), _T("==============================\r\n"));
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of IsUnicode(),is not change:Str s;s = XX/but change while:Str new = XX")
		{
			//str = (TCHAR*)NULL; str.ToMByte(); str2 = (TCHAR*)NULL; str2.ToWChar();
			//then (cs = cs2) not make cs to WChar! but can do it when define:(Str csnew = cs2)
			Str str = Str(cc::LangCode::SYSTEM);
			Str str2 = Str(cc::LangCode::UNICODE16);
			TESTASSERT_FALSE(str.IsUnicode());
			TESTASSERT_TRUE(str2.IsUnicode());
			//change langcode
			str = str2;
			TESTASSERT_TRUE(str.IsUnicode());
			//change langcode
			Str str3 = str2;
			TESTASSERT_TRUE(str3.IsUnicode());
		}
		TESTCASE_END;

		TESTCASE_BEGIN("test of UNICODE & ANICODE(need see log)")
		{
			//test:UNICODE & ANICODE
			cc::Log::traceConsole(RGB(255, 0, 0), _T("==============================\r\n"));
			cc::Log::debug(_T("test of UNICODE & ANICODE"));
			wchar_t wchJP[] = {0x6f22,0x8a9e,0}; //japanese:kanngo
			char chJP[] = {(char)0x8a,(char)0xbf,(char)0x8c,(char)0xea,0x00}; //japanese:kanngo

			cc::Log::debug(_T("char chJP[] = {0x8a,0xbf,0x8c,0xea,0x00}; //jp:kanngo"));
			//cc::Log::debug(_T("ToTChar():%s"), (TCHAR *)str1.GetBuf());
			//need to set pagecode of Str and chJP
			Str str1 = Str(cc::LangCode::UNICODE16, chJP, cc::LangCode::JP_SJIS);
			cc::Log::debug(L"Str(cc::LangCode::UNICODE16, char *, cc::LangCode::JP_SJIS):%s", (wchar_t *)str1.GetBuf());
			//ToMByte,need to set pagecode(otherwise use system code)
			str1.ToMByte(cc::LangCode::JP_SJIS);
			cc::Log::traceConsole(RGB(255, 0, 0), _T("only good at SJIS system:\r\n"));
			cc::Log::debug("ToMByte(cc::LangCode::JP_SJIS):%s", (char *)str1.GetBuf());
			//not need set pagecode while ToWChar
			str1.ToWChar();
			cc::Log::debug(L"ToWChar():%s", (wchar_t *)str1.GetBuf());
			//test buf
			TESTASSERT_EQUAL(wchJP, (wchar_t *)str1.GetBuf());

			cc::Log::debug(_T("wchar_t wchJP[] = {0x6f22,0x8a9e,0}; //jp:kanngo"));
			//not need to set pagecode,but it is good to set pagecode
			Str str2 = Str(cc::LangCode::UNICODE16, wchJP);
			cc::Log::debug(L"Str(cc::LangCode::UNICODE16, wchar_t *):%s", (wchar_t *)str2.GetBuf());
			//ToMByte,need to set pagecode(otherwise use system code)
			str2.ToMByte(cc::LangCode::JP_SJIS);
			//test buf
			TESTASSERT_EQUAL(chJP, (char *)str2.GetBuf());
			cc::Log::traceConsole(RGB(255, 0, 0), _T("only good at SJIS system:\r\n"));
			cc::Log::debug("ToMByte(cc::LangCode::JP_SJIS):%s", (char *)str2.GetBuf());
			//not need set pagecode while ToWChar
			str2.ToWChar();
			cc::Log::debug(L"ToWChar():%s", (wchar_t *)str2.GetBuf());

			wchar_t wchCN[] = {0x6c49,0x8bed,0}; //chinese:hanyu
			char chCN[] = {(char)0xba,(char)0xba,(char)0xd3,(char)0xef,0x00}; //chinese:hanyu

			cc::Log::debug(_T("char chJP[] = {0xffba,0xffba,0xffd3,0xffef,0x0}; //cn:hanyu"));
			Str str3 = Str(cc::LangCode::UNICODE16, chCN, cc::LangCode::CN_GB2312);
			cc::Log::debug(L"Str(cc::LangCode::UNICODE16, char *, cc::LangCode::CN_GB2312):%s", (wchar_t *)str3.GetBuf());
			//ToMByte,need to set pagecode(otherwise use system code)
			str3.ToMByte(cc::LangCode::CN_GB2312);
			cc::Log::traceConsole(RGB(255, 0, 0), _T("only good at GB2312 system:\r\n"));
			cc::Log::debug("ToMByte(cc::LangCode::CN_GB2312):%s", (char *)str3.GetBuf());
			//not need set pagecode while ToWChar
			str3.ToWChar();
			cc::Log::debug(L"ToWChar():%s", (wchar_t *)str3.GetBuf());
			//test buf
			TESTASSERT_EQUAL(wchCN, (wchar_t *)str3.GetBuf());

			cc::Log::debug(_T("wchar_t wchCN[] = {0xba,0xba,0xd3,0xef,0x0}; //cn:hanyu"));
			Str str4 = Str(cc::LangCode::UNICODE16, wchCN);
			cc::Log::debug(L"Str(cc::LangCode::UNICODE16, wchar_t *):%s", (wchar_t *)str4.GetBuf());
			//ToMByte,need to set pagecode(otherwise use system code)
			str4.ToMByte(cc::LangCode::CN_GB2312);
			//test buf
			TESTASSERT_EQUAL(chCN, (char *)str4.GetBuf());
			cc::Log::traceConsole(RGB(255, 0, 0), _T("only good at GB2312 system:\r\n"));
			cc::Log::debug("ToMByte(cc::LangCode::CN_GB2312):%s", (char *)str4.GetBuf());
			//not need set pagecode while ToWChar
			str4.ToWChar();
			cc::Log::debug(L"ToWChar():%s", (wchar_t *)str4.GetBuf());

			wchar_t wchJP1[] = {0x6f22,0}; //japanese:kanngo
			wchar_t wchJP2[] = {0x8a9e,0}; //japanese:kanngo
			wchar_t wchCN1[] = {0x6c49,0}; //chinese:hanyu
			wchar_t wchCN2[] = {0x8bed,0}; //chinese:hanyu
			Str str5 = Str(cc::LangCode::UNICODE16, chJP, 0, 2, cc::LangCode::JP_SJIS);
			cc::Log::debug(L"Str(cc::LangCode::UNICODE16, wchar_t *, 0, 2):%s", (wchar_t *)str5.GetBuf());
			Str str6 = Str(cc::LangCode::UNICODE16, chJP, 2, 4, cc::LangCode::JP_SJIS);
			cc::Log::debug(L"Str(cc::LangCode::UNICODE16, wchar_t *, 2, 4):%s", (wchar_t *)str6.GetBuf());
			Str str7 = Str(cc::LangCode::UNICODE16, chCN, 0, 2, cc::LangCode::CN_GB2312);
			cc::Log::debug(L"Str(cc::LangCode::UNICODE16, wchar_t *, 0, 2):%s", (wchar_t *)str7.GetBuf());
			Str str8 = Str(cc::LangCode::UNICODE16, chCN, 2, 4, cc::LangCode::CN_GB2312);
			cc::Log::debug(L"Str(cc::LangCode::UNICODE16, wchar_t *, 2, 4):%s", (wchar_t *)str8.GetBuf());
			TESTASSERT_EQUAL(wchJP1, (wchar_t *)str5.GetBuf());
			TESTASSERT_EQUAL(wchJP2, (wchar_t *)str6.GetBuf());
			TESTASSERT_EQUAL(wchCN1, (wchar_t *)str7.GetBuf());
			TESTASSERT_EQUAL(wchCN2, (wchar_t *)str8.GetBuf());
		}
		TESTCASE_END;
	};
};

} //namespace cc

#endif //#ifndef cc_StrTest_H_
