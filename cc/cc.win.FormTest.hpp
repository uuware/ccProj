/* cc.FileTest.hpp
   Test for cc.File
   Author: Shu.KK
   created:2006-01-19
*/
#ifndef cc_win_FormTest_H_
#define cc_win_FormTest_H_

#include <windows.h>
#include <process.h>
#include <stdio.h>

#include "cc.win.Control.h"
#include "cc.win.Form.h"
#include "cc.Test.h"
using namespace cc;
using namespace cc::win;

namespace cc
{
namespace win
{

/// <summary>
/// test of form
/// </summary>
class TestForm1 : public Form
{
private:
	Label* label1;
	Button* button1;
	ListBox* listBox1;
	CheckListBox* checkListBox1;
	ComboBox* comboBox1;
	Edit* edit1;
	HScrollBar* hScrollBar1;
	VScrollBar* vScrollBar1;

public:
	TestForm1()
	{
		InitializeComponent();
	}

protected:
	void InitializeComponent()
	{
		label1 = new Label();
		label1->SetName(_T("label1"));
		label1->SetText(_T("this is label1"));
		label1->Click = Event((Object*)this, (EventFun)&TestForm1::button1_Click);
		label1->SetLocation(2, 2);
		AddControl(label1);

		button1 = new Button();
		button1->SetName(_T("button1"));
		button1->SetText(_T("button1"));
		button1->Click = Event((Object*)this, (EventFun)&TestForm1::button1_Click);
		button1->SetLocation(2, 40);
		AddControl(button1);

		comboBox1 = new ComboBox();
		comboBox1->SetName(_T("comboBox1"));
		comboBox1->SetText(_T("comboBox1"));
		//comboBox1->Click = Event(this, (EventFun)&TestForm1::button1_Click);
		comboBox1->SetLocation(2, 80);
		AddControl(comboBox1);

		edit1 = new Edit();
		edit1->SetName(_T("edit1"));
		edit1->SetText(_T("edit1"));
		//edit1->Click = Event(this, (EventFun)&TestForm1::button1_Click);
		edit1->SetLocation(2, 120);
		AddControl(edit1);

		hScrollBar1 = new HScrollBar();
		hScrollBar1->SetName(_T("hScrollBar1"));
		hScrollBar1->SetText(_T("hScrollBar1"));
		hScrollBar1->Click = Event((Object*)this, (EventFun)&TestForm1::button1_Click);
		hScrollBar1->SetLocation(2, 160);
		AddControl(hScrollBar1);

		vScrollBar1 = new VScrollBar();
		vScrollBar1->SetName(_T("vScrollBar1"));
		vScrollBar1->SetText(_T("vScrollBar1"));
		vScrollBar1->Click = Event((Object*)this, (EventFun)&TestForm1::button1_Click);
		vScrollBar1->SetLocation(130, 2);
		AddControl(vScrollBar1);

		listBox1 = new ListBox();
		listBox1->SetName(_T("listBox1"));
		listBox1->SetText(_T("listBox1"));
		listBox1->Click = Event((Object*)this, (EventFun)&TestForm1::button1_Click);
		listBox1->SetLocation(160, 2);
		AddControl(listBox1);

		SetName(_T("TestForm1"));
		Load = Event((Object*)this, (EventFun)&TestForm1::TestForm1_Load);
		//SetSize(600, 400);
	}

	void button1_Click(Control* sender, EventArgs* e)
	{
		cc::Log::debug(_T("button1_Click"));
		MessageBox(NULL, TEXT("xxxxxxxxxxxxxxxx!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
	}
	void TestForm1_Load(Control* sender, EventArgs* e)
	{
		cc::Log::debug(_T("TestForm1_Load as fasd fasdf"));
	}
};

/// <summary>
/// test of form
/// </summary>
class TestForm2 : public Form
{
private:
	Label* label1;
	Button* button1;
	Button* button2;
	UserControl* userCtrl;

public:
	TestForm2()
	{
		InitializeComponent();
	}

protected:
	void InitializeComponent()
	{
		label1 = new Label();
		label1->SetName(_T("label1"));
		label1->SetText(_T("this is label1"));
		label1->Click = Event((Object*)this, (EventFun)&TestForm2::button1_Click);
		label1->SetLocation(2, 2);
		AddControl(label1);

		button1 = new Button();
		button1->SetName(_T("button1"));
		button1->SetText(_T("open another modal"));
		button1->Click = Event((Object*)this, (EventFun)&TestForm2::button1_Click);
		button1->SetLocation(2, 40);
		button1->SetSize(200, 30);
		AddControl(button1);

		button2 = new Button();
		button2->SetName(_T("button2"));
		button2->SetText(_T("open another modalness"));
		button2->Click = Event((Object*)this, (EventFun)&TestForm2::button2_Click);
		button2->SetLocation(2, 80);
		button2->SetSize(200, 30);
		AddControl(button2);

		userCtrl = new UserControl();
		userCtrl->SetName(_T("userCtrl1"));
		userCtrl->SetText(_T("this is userCtrl1"));
		userCtrl->SetLocation(100, 100);
		userCtrl->SetSize(200, 100);

		Button *button3 = new Button();
		button3->SetName(_T("xxx"));
		button3->SetText(_T("open another modalness"));
		button3->Click = Event((Object*)this, (EventFun)&TestForm2::button2_Click);
		button3->SetLocation(0, 0);
		button3->SetSize(200, 30);
		userCtrl->AddControl(button3);

		AddControl(userCtrl);

		SetName(_T("TestForm1"));
		Load = Event((Object*)this, (EventFun)&TestForm2::TestForm1_Load);
		// button1
		//this.button1.SetLocation = new System.Drawing.Point(16, 168);
		//this.button1.Name = "button1";
		//this.button1.TabIndex = 0;
		//this.button1.Text = "button1";
		//this.button1.Click += new System.EventHandler(this.button1_Click);
		// TestForm1
		//this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
		//this.ClientSize = new System.Drawing.Size(292, 266);
		//this.Controls.Add(this.button1);
		//this.Name = "TestForm1";
		//this.Text = "TestForm1";
		//this.ResumeLayout(false);
	}

	void button1_Click(Control* sender, EventArgs* e)
	{
		TestForm1* form = new TestForm1();
		//form->OnCreate(m_hWnd, NULL);
		form->ShowDialog();
	}
	void button2_Click(Control* sender, EventArgs* e)
	{
		TestForm1* form = new TestForm1();
		//form->OnCreate(m_hWnd, NULL);
		form->Show();
	}
	void TestForm1_Load(Control* sender, EventArgs* e)
	{
		cc::Log::debug(_T("TestForm2_Load as fasd fasdf"));
	}
};


//////////////////////// class fot test of File START ///////////////////
class FormTest : public cc::Test
{
public:
	FormTest()
	{
		TESTCLASS_COMMENT("test of cc::File");
	}

	void Setup()
	{
	}

	void TearDown()
	{
	}

	void TestCase()
	{
		TESTCASE_BEGIN("test:Form.ShowDialog")
		{
			//cc::win::Form form;
			//form.ShowDialog();
			//TestForm1* form1 = new TestForm1();
			//form1->ShowDialog();
			TestForm2 * form2 = new TestForm2();
			form2->ShowDialog();
		}
		TESTCASE_END;
	};
};

} //namespace win
} //namespace cc

#endif //#ifndef cc_win_FormTest_H_
