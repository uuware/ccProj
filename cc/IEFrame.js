/*************************************************
Function: Common Class for JavaScript
Author: Shu.KK
2005.10
*************************************************/

//for access window.external
function ext()
{
  m_ext = window.external;
  if(typeof(dialogArguments) == "object") m_ext = dialogArguments.external;

  this.newObject = newObject;
  function newObject(objName)
  {
    try
    {
      return (m_ext.CreateObject(objName));
    }
    catch(exp)
    {
      return (new ActiveXObject(objName));
    }
  }

  this.getFileName = getFileName;
  function getFileName()
  {
    return (m_ext.getFileName());
  }

  this.quit = quit;
  function quit()
  {
    try{m_ext.Quit();}catch(exp){window.close();}
  }

  this.getVersion = getVersion;
  function getVersion()
  {
    return (m_ext.getVersion());
  }

  //see csidl in shlobj.h from _SHFOLDER_H_
  this.getSpecialFolder = getSpecialFolder;
  function getSpecialFolder(int_csidl)
  {
    return (m_ext.getSpecialFolder(int_csidl));
  }

  this.setObject = setObject;
  function setObject(name, jsObj)
  {
    return (m_ext.setObject(name, jsObj));
  }

  //sample Filters:_T("All Files (*.*)\0*.*\0HTM Files(*.htm)\0*.htm\0")
  //sample Ext:_T("txt")
  this.selectOpenFile = selectOpenFile;
  function selectOpenFile(location, filter, defaultExt, caption)
  {
    try{return m_ext.selectOpenFile(location, filter, defaultExt, caption);}catch(exp){return null;};
  }

  this.selectSaveAs = selectSaveAs;
  function selectSaveAs(location, filter, defaultExt, caption)
  {
    try{return m_ext.selectSaveAs(location, filter, defaultExt, caption);}catch(exp){return null;};
  }

  this.selectDir = selectDir;
  function selectDir(location, caption)
  {
    try{return m_ext.selectDir(location, caption);}catch(exp){return null;};
  }

  this.sleep = sleep;
  function sleep(nMilliseconds)
  {
    try{m_ext.Sleep(nMilliseconds);}catch(exp){};
  }

  this.update = update;
  function update()
  {
    try{m_ext.Update();}catch(exp){};
  }

  //SW_HIDE:0,SW_SHOWNORMAL:1,SW_SHOW:5,SW_RESTORE:9(winuser.h)
  //return -1:win state;-2:x;-3:y;-4:w;-5:h
  this.show = show;
  function show(nShow)
  {
    try{return m_ext.Show(nShow);}catch(exp){return -1;};
  }

  this.move = move;
  function move(nX, nY, nW, nH)
  {
    try{m_ext.Move(nX, nY, nW, nH);}catch(exp){};
  }
  return this;
}

//global function used for String
GlobalFunction();
function GlobalFunction()
{
  String.prototype.trim = function()
  {
    var nHead = 0;
    var nEnd = this.length;
    while(nHead < nEnd && this.substring(nHead, nHead + 1) == " ") nHead++;
    while(nEnd > nHead && this.substring(nEnd - 1, nEnd) == " ") nEnd--;
    return this.substring(nHead, nEnd); //this.replace(/(^\s*)|(\s*$)/g, ""); //eat /r/n
  }
  String.prototype.replaceAll = function(from, to)
  {
    if(from == null || to == null || from == "") return this;
    var pos = this.indexOf(from);
    var posold = 0;
    if(pos < 0) return this;
    var sRetu = "";
    while(pos >= 0)
    {
      sRetu = sRetu + this.substring(posold, pos) + to;
      posold = pos + from.length;
      pos = this.indexOf(from, posold);
    }
    if(posold != this.length)
    {
      sRetu = sRetu + this.substring(posold);
    }
    return sRetu; //not good:return this.replace(new RegExp(from, "g"), to);
  }
}

//////////////////////////////////////////////////////////////////////////////
//TabPage
//can use like:var o = new ClsTabPage(pageID, pageIndex);o.show(pageIndex);
//also can like:ClsTabPage(pageID, pageIndex).show(pageIndex);
function ClsTabPage(pageID, pageIndex)
{
  var oDiv = document.all(pageID);
  if(oDiv != null && typeof(oDiv.length) == "number"
    && (typeof(oDiv[0].objCreated) != "number" || typeof(oDiv[0].objTabPage) == "object"))
  {
    if(typeof(oDiv[0].objTabPage) == "object")
    {
      oDiv[0].objTabPage.show(pageIndex);
    }
    else
    {
      oDiv[0].objCreated = 0;
      oDiv[0].objTabPage = new ClsTabPage(pageID, pageIndex);
    }
    return oDiv[0].objTabPage;
  }

  var TabPages;
  var TabTitles;
  var objBody = null;
  var isInitOK = false;
  var TabPageIndex = 0;

  //init process
  init();
  //if not defined pageIndex,not show it in show()
  show(pageIndex);

  //private function
  function isValid()
  {
    return (oDiv != null && typeof(oDiv.length) == "number" && isInitOK);
  }

  //private function
  function init()
  {
    if(oDiv == null || typeof(oDiv.length) != "number") return false;

    //get all TabPage HTML
    var pageCnt = 0;
    TabPages = new Array();
    TabTitles = new Array();
    for(var i = 0; i < oDiv.length; i++)
    {
      var obji = oDiv[i];
      var sType = obji.type;
      if(typeof(sType) != "string") continue;
      if(sType == "TabPageBody")
      {
        objBody = obji;
      }
      else if(sType == "TabPagePage")
      {
        if(typeof(obji.title) != "string") obji.title = "TabPage" + pageCnt;
        TabPages[pageCnt] = obji;
        pageCnt++;
      }
    }
    if(objBody == null || TabPages.length <= 0)
    {
      return false;
    }

    //create title
    var sTitle = '<div style="POSITION:absolute;left:1px;top:0px;overflow:scroll;'
               + 'overflow-x:hidden;overflow-y:hidden;height:24;width:100%;">';
    for(var i = 0; i < TabPages.length; i++)
    {
      sTitle += '<span id="' + pageID + i + '" style="z-index:3;'
             +  'POSITION:absolute;left:100px;top:3px;height:25;ARGIN:0px;font:12px;text-align:center;'
             +  'border-left:1px solid ThreeDHighlight;border-right:2px outset;'
             +  'border-top:1px solid ThreeDHighlight;'
             +  'border-bottom:0;background-color:ThreeDFace;padding:2px 5px 3px 5px;cursor:default;"'
             +  ' onSelectStart="return false" onclick="ClsTabPage(\'' + pageID + '\', ' + i + ')">'
             +  TabPages[i].title + '</span>';
    }
    sTitle += '<div style="z-index:4;POSITION:absolute;left:-3px;top:0px;height:23;width:100%;'
           +  'border-bottom:1px solid ThreeDHighlight;"></div></div>';

    //show title
    objTable = document.createElement("<table BORDER=0 CELLPADDING=0 CELLSPACING=1 width=100% height=100%>");
    objBody.insertBefore(objTable, objBody.firstChild);
    var oCell1 = objTable.insertRow().insertCell();
    var oCell2 = objTable.insertRow().insertCell();
    oCell1.innerHTML = sTitle;
    oCell1.style.height = "21px";

    objBody.style.display = "";
    var cntW = 0;
    var sStyle = "display:none;POSITION:absolute;left:1px;top:23px;overflow:auto;width:100%;height:100%;"
               + "border-left:1px solid ThreeDHighlight;border-right:2px outset;border-bottom:2px outset;"
               + "background-color:ThreeDFace;padding:3px 5px 3px 8px;";
    for(var i = 0; i < TabPages.length; i++)
    {
      TabTitles[i] = document.getElementById(pageID + i);
      TabTitles[i].style.left = cntW;
      if(TabTitles[i].clientWidth == 0)
      {
        var oTmp = document.createElement("<span>");
        oTmp.style.cssText = "height:25;ARGIN:0px;font:12px;text-align:center;padding:2px 5px 3px 5px;"
                           + "border-left:1px solid ThreeDHighlight;border-right:2px outset;";
        oTmp.innerHTML = TabPages[i].title
        document.body.appendChild(oTmp);
        cntW += oTmp.clientWidth + 3;
        document.body.removeChild(oTmp);
        oTmp.outerHTML = "";
      }
      else
      {
        cntW += TabTitles[i].clientWidth + 3;
      }

      oCell2.appendChild(TabPages[i]);
      TabPages[i].style.cssText = sStyle;
    }

    isInitOK = true;
    return true;
  }

  //get TabPageIndex
  this.getTabPageIndex = getTabPageIndex;
  function getTabPageIndex()
  {
    return TabPageIndex;
  }

  //show TabPage
  this.show = show;
  function show(pageIndex)
  {
    if(!isValid()) return false;
    if(typeof(pageIndex) == "undefined") return false;

    if(typeof(pageIndex) == "string")
    {
      for(var i = 0; i < TabPages.length; i++)
      {
        if(TabPages[i].title == pageIndex)
        {
          pageIndex = i;
          break;
        }
      }
    }

    if(pageIndex <= 0) pageIndex = 0;
    if(pageIndex >= TabPages.length) pageIndex = TabPages.length - 1;
    for(var i = 0; i < TabPages.length; i++)
    {
      TabPages[i].style.display = "none";
      TabTitles[i].style.zIndex = 3;
      TabTitles[i].style.paddingTop = 2;
      TabTitles[i].style.top = 3;
    }

    TabPageIndex = pageIndex;
    TabPages[pageIndex].style.display = "";
    TabTitles[pageIndex].style.zIndex = 5;
    TabTitles[pageIndex].style.paddingTop = 4;
    TabTitles[pageIndex].style.top = 0;
    return true;
  }

  //avoids this error:ClsTabPage("not exist").show(0);
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//List
//can use like:var o = new ClsList(listID);o.add(s);
//also can like:ClsList(listID).add(s);
function ClsList(listID)
{
  var oTable = document.getElementById(listID);
  if(oTable != null && (typeof(oTable.objCreated) != "number" || typeof(oTable.objList) == "object"))
  {
    if(typeof(oTable.objList) != "object")
    {
      oTable.objCreated = 0;
      oTable.objList = new ClsList(listID);
    }
    return oTable.objList;
  }

  var listType = "checkbox";
  if(oTable != null && typeof(oTable.type) == "string")
  {
    if(oTable.type == "List")
    {
      listType = "";
    }
  }
  var loc_selectIndex = -1;
  var loc_lastselect = -1;
  this.SelectedIndexChanged = null;
  var nZIndex = 0;
  if(isValid())
  {
    nZIndex = oTable.rows.length;
    oTable.onkeydown = list_onkeydown;
    oTable.objList = this;
  }

  //private function
  function isValid()
  {
    return (oTable != null && typeof(oTable) == "object");
  }

  //private function
  //key of up&down
  function list_onkeydown()
  {
    if(!isValid()) return true;
    if(oTable.rows.length == 0) return true;
    if(event.keyCode != 38 && event.keyCode != 40 && event.keyCode != 9) return true;
    if((event.shiftKey && event.keyCode == 9) && loc_selectIndex == 0) return true;
    if((!event.shiftKey && event.keyCode == 9) && loc_selectIndex == oTable.rows.length - 1) return true;

    var nIndexNew = -1;
    if(loc_selectIndex == -1)
    {
      nIndexNew = 0;
    }
    //up,shift+tab
    else if((event.keyCode == 38 || (event.shiftKey && event.keyCode == 9)) && loc_selectIndex > 0)
    {
      nIndexNew = loc_selectIndex - 1;
    }
    //down,tab
    else if((event.keyCode == 40 || (!event.shiftKey && event.keyCode == 9)) && loc_selectIndex < oTable.rows.length)
    {
      nIndexNew = loc_selectIndex + 1;
    }
    select(nIndexNew);
    return false;
  }

  //private fun
  function setTRValue(oTR, str, tag)
  {
    if(typeof(tag) == "undefined") tag = "";
    var oTCell = oTR.cells[0];
    var id = oTR.ChildID;
    oTCell.style.whiteSpace = "nowrap";
    oTCell.style.backgroundColor = "";
    var sTxt = '<input id="' + id + '" type="' + listType + '" value="on" ';
    if(listType == "")
    {
      sTxt += 'style="width:0;" ';
    }
    sTxt += '/><label for="' + id + '" style="width:100%;" onSelectStart="return false">' + str + '</label>';
    oTCell.innerHTML = sTxt;
    oTR.Tag = tag;
    oTR.Text = str;
    eval(id).oTR = oTR;
    eval(id).onfocus = function(){eval(listID).objList.select(this.oTR.rowIndex);};
    eval(id).onclick = function(){eval(listID).objList.click(this.oTR.rowIndex);};
  }

  this.clear = clear;
  function clear()
  {
    if(!isValid()) return false;

    for(var i = oTable.rows.length - 1; i >= 0; i--)
    {
      oTable.deleteRow(i);
    }
    loc_selectIndex = -1;
    return true;
  }

  this.select = select;
  function select(index)
  {
    if(!isValid()) return false;

    if(index < 0 || index > oTable.rows.length - 1) return false;
    if(loc_selectIndex > -1 && loc_selectIndex < oTable.rows.length)
    {
      oTable.rows[loc_selectIndex].cells[0].style.backgroundColor = "";
    }
    loc_selectIndex = index;
    oTable.rows[loc_selectIndex].cells[0].style.backgroundColor = "buttonface";
    var id = oTable.rows[loc_selectIndex].ChildID;
    eval(id).focus();
    if(typeof(this.SelectedIndexChanged) == "function")
    {
      this.SelectedIndexChanged(this);
    }
    return true;
  }

  this.click = click;
  function click(index)
  {
    if(!isValid()) return false;

    if(index < 0 || index > oTable.rows.length - 1) return false;
    if(event.shiftKey && loc_lastselect > -1 && loc_lastselect < oTable.rows.length)
    {
      var i = loc_lastselect;
      var j = index;
      if(loc_lastselect > index)
      {
        i = index;
        j = loc_lastselect;
      }
      for(; i < j; i++)
      {
        setCheck(i, !getCheck(i));
      }
    }
    loc_lastselect = index;
    return true;
  }

  this.getCount = getCount;
  function getCount()
  {
    if(!isValid()) return -1;
    return oTable.rows.length;
  }

  this.getSelectedCount = getSelectedCount;
  function getSelectedCount()
  {
    if(!isValid()) return 0;
    var nCnt = 0;
    for(var i = 0; i < oTable.rows.length; i++)
    {
      var id = oTable.rows[i].ChildID;
      if(eval(id).checked) nCnt++;
    }
    return nCnt;
  }

  this.getSelectIndex = getSelectIndex;
  function getSelectIndex()
  {
    return loc_selectIndex;
  }

  this.add = add;
  function add(str, tag, isChecked)
  {
    if(!isValid()) return -1;
    return insert(oTable.rows.length, str, tag, isChecked);
  }

  this.insert = insert;
  function insert(index, str, tag, isChecked)
  {
    if(!isValid()) return -1;
    if(index > oTable.rows.length) index = oTable.rows.length;
    var oTR = oTable.insertRow(index);
    oTR.insertCell();
    nZIndex++;
    oTR.ChildID = listID + '_' + nZIndex;
    setTRValue(oTR, str, tag);
    if(typeof(isChecked) != "undefined") setCheck(index, isChecked);
    return index;
  }

  //this is index or str
  this.remove = remove;
  function remove(index_or_str)
  {
    var index = exist(index_or_str);
    if(index == -1) return false;
    oTable.deleteRow(index);
    return true;
  }

  this.getValue = getValue;
  function getValue(index)
  {
    if(!isValid()) return null;
    if(index < 0 || index > oTable.rows.length - 1) return null;
    return oTable.rows(index).Text;
  }

  this.getTag = getTag;
  function getTag(index)
  {
    if(!isValid()) return null;
    if(index < 0 || index > oTable.rows.length - 1) return null;
    if(typeof(oTable.rows(index).Tag) == "undefined") return "";
    return oTable.rows(index).Tag;
  }

  this.setValue = setValue;
  function setValue(index, str, tag, isChecked)
  {
    if(!isValid()) return false;
    if(index < 0 || index > oTable.rows.length - 1) return false;
    if(typeof(isChecked) == "undefined") isChecked = getCheck(index);
    setTRValue(oTable.rows[index], str, tag)
    if(index == loc_selectIndex)
    {
      select(index);
    }
    setCheck(index, isChecked);
    return true;
  }

  this.getCheck = getCheck;
  function getCheck(index)
  {
    if(!isValid()) return false;
    if(index < 0 || index > oTable.rows.length - 1) return false;
    var id = oTable.rows[index].ChildID;
    return eval(id).checked;
  }

  this.setCheck = setCheck;
  function setCheck(index, isChecked)
  {
    if(!isValid()) return false;
    if(index < 0 || index > oTable.rows.length - 1) return false;
    if(typeof(isChecked) == "undefined") isChecked = true;
    if(typeof(isChecked) == "string") isChecked = (isChecked.toLowerCase() == "true");
    var id = oTable.rows[index].ChildID;
    eval(id).checked = isChecked;
  }

  //return the index of index_or_str,-1 for not found
  this.exist = exist;
  function exist(index_or_str)
  {
    if(!isValid()) return -1;
    if(typeof(index_or_str) == "string")
    {
      for(var i = 0; i < oTable.rows.length; i++)
      {
        if(oTable.rows(i).Text == index_or_str)
        {
          return i;
        }
      }
    }
    else if(typeof(index_or_str) == "number")
    {
      if(index_or_str < 0 || index_or_str > oTable.rows.length - 1) return -1;
      return index_or_str;
    }
    return -1;
  }

  //move the selected item up
  this.moveUp = moveUp;
  function moveUp()
  {
    if(!isValid()) return -1;
    if(loc_selectIndex < 1 || oTable.rows.length < 2)
    {
      return -2;
    }
    var str = oTable.rows(loc_selectIndex).Text;
    var tag = oTable.rows(loc_selectIndex).Tag;
    var isCheck = eval(oTable.rows[loc_selectIndex].ChildID).checked;
    setValue(loc_selectIndex, oTable.rows(loc_selectIndex - 1).Text, oTable.rows(loc_selectIndex - 1).Tag
                  , eval(oTable.rows[loc_selectIndex - 1].ChildID).checked);
    setValue(loc_selectIndex - 1, str, tag, isCheck);
    select(loc_selectIndex - 1);
    return (loc_selectIndex - 1);
  }

  //move the selected item down
  this.moveDown = moveDown;
  function moveDown()
  {
    if(!isValid()) return -1;
    if(loc_selectIndex == -1 || loc_selectIndex == oTable.rows.length - 1 || oTable.rows.length < 2)
    {
      return -2;
    }
    var str = oTable.rows(loc_selectIndex).Text;
    var tag = oTable.rows(loc_selectIndex).Tag;
    var isCheck = eval(oTable.rows[loc_selectIndex].ChildID).checked;
    setValue(loc_selectIndex, oTable.rows(loc_selectIndex + 1).Text, oTable.rows(loc_selectIndex + 1).Tag
                  , eval(oTable.rows[loc_selectIndex + 1].ChildID).checked);
    setValue(loc_selectIndex + 1, str, tag, isCheck);
    select(loc_selectIndex + 1);
    return (loc_selectIndex + 1);
  }

  //avoids this error:ClsList("not exist").add(s);
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//Log
function ClsLog()
{
  var IsRunning = false;
  var isCanceling = false;
  var sRunning = "Cancel";
  var sCanceling = "Canceling...";
  var IsAlive = false;
  var ActEval = "";
  var timeStart = new Date();

  this.setParent = setParent;
  function setParent(objParent, actEval)
  {
    try
    {
      var sHTML = ''
         + '<table border=0 style="height:100%;width:100%;MARGIN:0px;"'
         + ' CELLPADDING=0 CELLSPACING=0><tr><td style="height:10px">'
         + '<input type=button id="log_btnExit" value="' + sRunning + '" disabled style="font:9px;width:80px;">'
         + '&nbsp;<input type=button value="ClearLog"'
         + 'onclick="log_output.innerHTML=\'\'" style="font:9px;width:80px;">'
         + '<LABEL for=log_chkScroll><INPUT autosave id=log_chkScroll type=checkbox checked>Auto Scroll</LABEL><br>'
         + '</td></tr><tr><td>'
         + '<div style="width:100%;height:100%;BACKGROUND:Window;overflow:auto;BORDER-TOP:#404040 2px solid;'
         + 'BORDER-RIGHT:#e0e0e0 2px solid;BORDER-BOTTOM:#f0f0f0 2px solid;BORDER-LEFT:#505050 2px solid;" '
         + 'id="log_output" contenteditable></div>'
         + '</td></tr></table>';
      objParent.innerHTML = sHTML;
    }
    catch(exp)
    {
      setStatus("log of setParent error:" + exp.message);
    }
    ActEval = actEval;
  }

  //this is private function
  function isAlive()
  {
    return IsAlive;
  }

  function setStatus(str)
  {
    if(StatusBar) StatusBar.innerHTML = str;
    else window.status = str;
  }

  //this is private function
  function btnExitOnClick()
  {
    if(IsRunning)
    {
      isCanceling = true;
      log_btnExit.value = sCanceling;
      log_btnExit.disabled = true;
    }
  }

  function show()
  {
    try
    {
      eval(ActEval);
      if(IsAlive)
      {
        return true;
      }
      log_btnExit.onclick = btnExitOnClick;
      IsAlive = true;
      return true;
    }
    catch(exp)
    {
      setStatus("show log error:" + exp.message);
    }
    return false;
  }

  this.clear = clear;
  function clear()
  {
    if(!IsAlive) return false;
    log_output.innerHTML = "";
    return true;
  }

  this.write = write;
  function write(str, color)
  {
    if(!IsAlive && !show()) return false;
    try
    {
      if(typeof(color) != "string") color = "windowtext";
      log_output.insertAdjacentHTML("beforeEnd", "<font color=" + color + ">"
           + str.replaceAll("<", "&lt;").replaceAll(">", "&gt;").replaceAll("\r\n", "<br>") + "</font>");
      if(log_chkScroll.checked) log_output.scrollTop = log_output.scrollHeight;
      return true;
    }
    catch(exp)
    {
      setStatus("Err of log.write:" + exp.message);
    }
    return false;
  }

  this.writeln = writeln;
  function writeln(str, color)
  {
    return write(str + "\r\n", color);
  }

  this.isCancel = isCancel;
  function isCancel(sMsg)
  {
    ext().update();
    if(typeof(sMsg) != "string") sMsg = "Cancel?";
    if(!isAlive() && !show()) return true;
    if(isCanceling)
    {
      eval(ActEval);
      if(confirm(sMsg))
      {
        writeln("Canceled.", "green");
        return true;
      }
      else
      {
        isCanceling = false;
        log_btnExit.value = sRunning;
      }
    }
    return false;
  }

  this.runBegin = runBegin;
  function runBegin(sMsg)
  {
    if(typeof(sMsg) != "string") sMsg = "Run Start?";
    if(IsRunning)
    {
      if(!show()) return false;
      alert("others is running, can not run this time.");
      return false;
    }
    if(isCanceling) return false;

    if(!confirm(sMsg)) return false;
    if(!show()) return false;

    IsRunning = true;
    isCanceling = false;
    log_btnExit.value = sRunning;
    log_btnExit.disabled = false;
    timeStart = new Date();
    objLog.clear();
    write("=====Start:" + timeStart.toLocaleString() + "\r\n", "green");
    return true;
  }

  this.runEnd = runEnd;
  function runEnd(sMsg)
  {
    var tmpCanceling = isCanceling;
    isCanceling = false;
    IsRunning = false;
    if(isAlive())
    {
      eval(ActEval);
      log_btnExit.value = sRunning;
      log_btnExit.disabled = true;
      if(log_chkScroll.checked) log_output.scrollTop = log_output.scrollHeight;
      write("=====End:" + new Date().toLocaleString() + "(elapsed:" + (new Date() - timeStart)/1000 + " Seconds)" + "\r\n", "green");
    }
    if(!tmpCanceling && typeof(sMsg) == "string") alert(sMsg);
    return true;
  }

  this.isRunning = isRunning;
  function isRunning()
  {
    return IsRunning;
  }

  this.isUnloadOk = isUnloadOk;
  function isUnloadOk()
  {
    if(IsRunning)
    {
      alert("can not quit this time, need cancel your action first!");
      return false;
    }
    if(typeof(objHTA) == "object") objHTA.sys_onbeforeunload();
    return true;
  }

  //avoids this error:ClsXXX("not exist").xxx();
  return this;
}

//this class can not run in HTML,should be run in WSF
//Class for Util
function ClsUtil()
{
  //for some functions
  objShell = ext().newObject("WScript.Shell");
  this.objShell = objShell;
  //for access to file
  objFile = ext().newObject("Scripting.FileSystemObject");
  this.objFile = objFile;
  //e.description,e.message,e.name,e.number,e.toString();
  this.Error = new Error();

  //this is private function
  var objAdoStream;
  function loc_AdoStream()
  {
    if(typeof(objAdoStream) != "object")
    {
      objAdoStream = ext().newObject("ADODB.Stream");
    }
    return objAdoStream;
  }

  //file is exist ot not
  this.fexist = fexist;
  function fexist(filename)
  {
    try
    {
      return objFile.FileExists(filename.trim());
    }
    catch(exp)
    {
    }
    return false;
  }

  //charset:_autodetect_all,unicode,utf-8,gb2312,Big5,shift_jis,euc-jp
  //adType=1(adTypeBinary),=2(adTypeText)
  this.fread = fread;
  function fread(file_name, adCharSet, adType)
  {
    if(arguments.length < 1) return null;
    if(typeof(adCharSet) == "undefined") adCharSet = "_autodetect_all";
    if(typeof(adType) == "undefined") adType = 2;
    try
    {
      if(!objFile.FileExists(file_name)) return null;
      var pAdoStream = loc_AdoStream();
      if(pAdoStream.State == 1) pAdoStream.Close();
      pAdoStream.Type = adType;
      if(adType != 1) pAdoStream.CharSet = adCharSet;
      pAdoStream.Open();
      pAdoStream.LoadFromFile(file_name);
      var s;
      if(adType == 1) s = pAdoStream.Read();
      else s = pAdoStream.ReadText();
      pAdoStream.Close();
      return s;
    }
    catch(exp)
    {
      this.Error = exp;
      this.Error.name = "fread";
    }
    return null;
  }

  //charset,adType:see fread
  this.fwrite = fwrite;
  function fwrite(file_name, writestr, isAppend, adCharSet, adType)
  {
    if(arguments.length < 2) return false;
    if(typeof(isAppend) == "undefined") isAppend = false;
    if(typeof(adCharSet) == "undefined") adCharSet = "_autodetect_all";
    if(typeof(adType) == "undefined") adType = 2;
    try
    {
      if(!objFile.FileExists(file_name))
      {
        file_name = file_name.replace(/\//g, "\\")
        var t_1 = file_name.lastIndexOf("\\");
        if(t_1 > 0)
        {
          if(!dcreate(file_name.substring(0, t_1 + 1))) return false;
        }
      }
      var pAdoStream = loc_AdoStream();
      if(pAdoStream.State == 1) pAdoStream.Close();
      pAdoStream.Type = adType;
      if(adType != 1) pAdoStream.CharSet = adCharSet;
      pAdoStream.Open();
      var nWriteFile = 1;
      if(objFile.FileExists(file_name))
      {
        nWriteFile = 2;
        if(isAppend)
        {
          pAdoStream.LoadFromFile(file_name);
          pAdoStream.Position = pAdoStream.Size;
        }
      }
      if(adType == 1) pAdoStream.Write(writestr);
      else pAdoStream.WriteText(writestr);
      pAdoStream.saveToFile(file_name, nWriteFile); //adSaveCreateNotExist=1,adSaveCreateOverWrite=2
      pAdoStream.Close();
      return true;
    }
    catch(exp)
    {
        this.Error = exp;
        this.Error.name = "fwrite";
    }
    return false;
  }

  this.fappend = fappend;
  function fappend(filename, writestr, adCharSet, adType){
    return fwrite(filename, writestr, true, adCharSet, adType);
  }

  this.fcopy = fcopy;
  function fcopy(sFrom, sTo)
  {
    try
    {
      if(objFile.FileExists(sFrom))
      {
        objFile.CopyFile(sFrom, sTo);
        return true;
      }
    }
    catch(exp)
    {
      this.Error = exp;
      this.Error.name = "fcopy";
    }
    return false;
  }

  //can only one path
  this.fdel = fdel;
  function fdel(sFile)
  {
    try
    {
      if(objFile.FileExists(sFile))
      {
        var oFile = objFile.GetFile(sFile);
        if(oFile.attributes != 0 && oFile.attributes != 32) oFile.attributes = 0;
        oFile.Delete();
      }
      if(!objFile.FileExists(sFile))
      {
        return true;
      }
    }
    catch(exp)
    {
      this.Error = exp;
      this.Error.name = "fdel";
    }
    return false;
  }

  //dir is exist ot not,can only one path
  this.dexist = dexist;
  function dexist(path)
  {
    try
    {
      var file_name = path.trim();
      if(file_name == "")
      {
        return false;
      }
      return objFile.FolderExists(file_name);
    }
    catch(exp)
    {
      this.Error = exp;
      this.Error.name = "dexist";
    }
    return false;
  }

  this.dcopy = dcopy;
  function dcopy(sFrom, sTo)
  {
    try
    {
      if(objFile.FolderExists(sFrom))
      {
        objFile.CopyFolder(sFrom, sTo, true);
        return true;
      }
    }
    catch(exp)
    {
      this.Error = exp;
      this.Error.name = "dcopy";
    }
    return false;
  }

  //can only one path
  this.ddel = ddel;
  function ddel(sDir)
  {
    try
    {
      if(objFile.FolderExists(sDir))
      {
        var oFold = objFile.GetFolder(sDir);
        if(oFold.attributes != 16 && oFold.attributes != 48) oFold.attributes = 16;
        oFold.Delete();
      }
      if(objFile.FolderExists(sDir))
      {
        return false;
      }
      return true;
    }
    catch(exp)
    {
      this.Error = exp;
      this.Error.name = "ddel";
    }
    return false;
  }

  this.dcreate = dcreate;
  function dcreate(path)
  {
    try
    {
      if(objFile.FolderExists(path)) return true;
      var Hnd;
      path = path.replace(/\//g, "\\") + "\\";
      var position = 1;
      var path1;
      while(path.indexOf("\\", position) > 0)
      {
        position = path.indexOf("\\", position) + 1;
        path1 = path.substring(0, position - 1);
        if(!objFile.FolderExists(path1)) objFile.CreateFolder(path1);
      }
      return true;
    }
    catch(exp)
    {
      this.Error = exp;
      this.Error.name = "dcreate";
    }
    return false;
  }

  //work as dir:(from:XCACLS.vbs.DoesThisOneMatch())
  // t*.txt or a??bc or *a?.a??
  this.isMatch = isMatch;
  function isMatch(filename, wildcard)
  {
    if(wildcard == "")
    {
      return true;
    }

    filename = filename.toLowerCase();
    wildcard = wildcard.toLowerCase();
    var posf = 0;
    var tmpwild = "";
    var hadStar = false;
    for(var i = 0; i <= wildcard.length; i++)
    {
      var sone = wildcard.substring(i, i + 1);
      if(sone == "*" || sone == "?" || i == wildcard.length)
      {
        if(tmpwild != "")
        {
          posf = filename.indexOf(tmpwild, posf);
          if(posf < 0 || (!hadStar && posf > 0) || (i == wildcard.length && posf != filename.length - tmpwild.length))
          {
            return false;
          }
          posf += tmpwild.length;
          tmpwild = "";
          hadStar = false;
        }
        if(sone == "*")
        {
          hadStar = true;
        }
        else if(sone == "?")
        {
          posf++;
          if(posf > filename.length)
          {
            return false;
          }
        }
      }
      else
      {
        tmpwild += sone;
      }
    }
    return true;
  }

  //you can use like:
  //  c:\\t*.txt
  this.getFiles = getFiles;
  function getFiles(path, wildcard)
  {
    var retu = Array();
    if(path == "" || !objFile.FolderExists(path))
    {
      return retu;
    }
    var isWild = false;
    if(typeof(wildcard) == "string") isWild = true;

    var fc = new Enumerator(objFile.GetFolder(path).Files);
    var i = 0;
    for(; !fc.atEnd(); fc.moveNext())
    {
      var sName = "" + fc.item();
      if(!isWild || isMatch(getFileName(sName), wildcard))
      {
        retu[i] = sName;
        i++;
      }
    }
    return retu.sort();
  }

  this.getDirs = getDirs;
  function getDirs(path, wildcard)
  {
    var retu = Array();
    if(path == "" || !objFile.FolderExists(path))
    {
      return retu;
    }
    var isWild = false;
    if(typeof(wildcard) == "string") isWild = true;

    var fc = new Enumerator(objFile.GetFolder(path).SubFolders);
    var i = 0;
    for(; !fc.atEnd(); fc.moveNext())
    {
      var sName = "" + fc.item();
      if(!isWild || isMatch(getFileName(sName), wildcard))
      {
        retu[i] = sName;
        i++;
      }
    }
    return retu.sort();
  }

  this.getFolder = getFolder;
  function getFolder(path)
  {
    return objFile.GetParentFolderName(path);
  }

  this.getFileName = getFileName;
  function getFileName(path)
  {
    return objFile.GetFileName(path);
  }

  this.getBaseName = getBaseName;
  function getBaseName(path)
  {
    return objFile.GetBaseName(path);
  }

  this.getFileExt = getFileExt;
  function getFileExt(path)
  {
    return objFile.GetExtensionName(path);
  }

  //////////////////////////////////////////////////////////////////////////////
  //WEB
  //////////////////////////////////////////////////////////////////////////////
  this.getHTTPPage = getHTTPPage;
  function getHTTPPage(sURL, sCharset, sGETPOST, bWAIT, funFunction)
  {
    if(typeof(sGETPOST) != "string") sGETPOST = "GET";
    if(typeof(bWAIT) != "boolean") bWAIT = false;
    if(typeof(funFunction) != "function") bWAIT = false;
    var objXMLHTTP = ext().newObject("Microsoft.XMLHTTP");
    objXMLHTTP.open(sGETPOST, sURL, bWAIT);
    if(bWAIT)
    {
      objXMLHTTP.onreadystatechange = funFunction;
    }
    try
    {
      objXMLHTTP.send();
      if(!bWAIT)
      {
        if(typeof(sCharset) != "string")
        {
          return objXMLHTTP.responseText;
        }
        else
        {
          return Stream2Char(objXMLHTTP.responseBody, sCharset);
        }
      }
    }
    catch(exp)
    {
      this.Error = exp;
      this.Error.name = "getHTTPPage";
    }
    return null;
  }

  this.getHTTPFile = getHTTPFile;
  function getHTTPFile(sURL, bWAIT, funFunction)
  {
    if(typeof(bWAIT) != "boolean") bWAIT = false;
    if(typeof(funFunction) != "function") bWAIT = false;
    var objXMLHTTP = ext().newObject("Microsoft.XMLHTTP");
    objXMLHTTP.open("GET", sURL, bWAIT);
    if(bWAIT)
    {
      objXMLHTTP.onreadystatechange = funFunction;
    }
    try
    {
      objXMLHTTP.send();
      if(!bWAIT)
      {
        return objXMLHTTP.responseBody;
      }
    }
    catch(exp)
    {
      this.Error = exp;
      this.Error.name = "getHTTPFile";
    }
    return null;
  }

  this.Stream2Char = Stream2Char;
  function Stream2Char(sStream, sCharset)
  {
    try
    {
      if(typeof(sCharset) != "string") sCharset = "ascii";
      var pAdoStream = loc_AdoStream();
      if(pAdoStream.State == 1)
      {
        pAdoStream.Close();
      }
      pAdoStream.type = 1;
      pAdoStream.Mode = 3;
      pAdoStream.Open() ;
      pAdoStream.Write(sStream);
      pAdoStream.Position = 0;
      pAdoStream.Type = 2;
      pAdoStream.Charset = sCharset;
      var result= pAdoStream.ReadText();
      pAdoStream.Close();
      return result;
    }
    catch(exp)
    {
      this.Error = exp;
      this.Error.name = "Stream2Char";
    }
    return null;
  }
}

//////////////////////////////////////////////////////////////////////////////
//ClsHTA
function ClsHTA()
{
  //Config
  this.getConfig = getConfig;
  function getConfig()
  {
    var wState = ext().show(-1);
    ext().show(1); //restore window
    var sRetu = "[main]\r\n";
    sRetu += "windows_state=" + wState + "\r\n";
    sRetu += "windows_x=" + ext().show(-2) + "\r\n";
    sRetu += "windows_y=" + ext().show(-3) + "\r\n";
    sRetu += "windows_w=" + ext().show(-4) + "\r\n";
    sRetu += "windows_h=" + ext().show(-5) + "\r\n";

    for(var i = 0; i < document.all.length; i++)
    {
      var id = document.all[i].autosave;
      var ty = document.all[i].type;
      if(typeof(id) == "string" && id.toLowerCase() == "true" || id == "")
      {
        id = document.all[i].id;
      }
      if(typeof(id) == "string" && id.length > 0 && typeof(ty) == "string")
      {
        var val = "";
        if(ty == "text" || ty == "textarea")
        {
          val = document.all[i].value;
        }
        else if(ty == "checkbox" || ty == "radio")
        {
          val = document.all[i].checked;
        }
        else if(ty == "TabPageBody")
        {
          val = ClsTabPage(id).getTabPageIndex();
        }
        else if(ty == "CheckBoxList" || ty == "List")
        {
          var obj = ClsList(id);
          for(var arrI = 0; arrI < obj.getCount(); arrI++)
          {
            var chk = obj.getCheck(arrI) ? "1" : "0";
            val += escape(obj.getValue(arrI)) + "\t" + chk + "\t" + escape(obj.getTag(arrI)) + "\r\n";
          }
        }
        sRetu += id + "=" + escape(val) + "\r\n";
      }
    }
    return sRetu;
  }

  this.splitIni = splitIni;
  function splitIni(sCfg)
  {
    var arrIni = new Object;
    if(typeof(sCfg) != "string" || sCfg == null || sCfg == "") return arrIni;

    var srcArr = sCfg.split("\r\n");
    for(var i = 0; i < srcArr.length; i++)
    {
      var wpos1 = srcArr[i].indexOf('=');
      if(wpos1 > 0)
      {
        arrIni[srcArr[i].substring(0, wpos1)] = srcArr[i].substring(wpos1 + 1);
      }
    }
    return arrIni;
  }

  this.setConfig = setConfig;
  function setConfig(arrIni)
  {
    if(typeof(arrIni) != "object" || arrIni == null) return false;

    try
    {
      if(arrIni["windows_x"] != null && arrIni["windows_y"] != null
        && arrIni["windows_w"] != null && arrIni["windows_h"] != null)
      {
        var nX = parseInt(arrIni["windows_x"], 10);
        var nY = parseInt(arrIni["windows_y"], 10);
        var nW = parseInt(arrIni["windows_w"], 10);
        var nH = parseInt(arrIni["windows_h"], 10);
        ext().move(nX, nY, nW, nH);
      }
    }catch(exp){}
    //restore win state
    if(arrIni["windows_state"]) ext().show(parseInt(arrIni["windows_state"], 10))

    for(var i = 0; i < document.all.length; i++)
    {
      var id = document.all[i].autosave;
      var ty = document.all[i].type;
      if(typeof(id) == "string" && id.toLowerCase() == "true" || id == "")
      {
        id = document.all[i].id;
      }
      if(typeof(id) == "string" && id.length > 0 && typeof(ty) == "string")
      {
        var val = arrIni[id];
        if(val == null) continue;
        val = unescape(val);
        if(ty == "text" || ty == "textarea")
        {
          document.all[i].value = val;
        }
        else if(ty == "checkbox" || ty == "radio")
        {
          if(val == "true")
            document.all[i].checked = true;
          else
            document.all[i].checked = false;
        }
        else if(ty == "TabPageBody")
        {
          ClsTabPage(id).show(val);
        }
        else if(ty == "CheckBoxList" || ty == "List")
        {
          var obj = ClsList(id);
          var arrItem = val.split("\r\n");
          obj.clear();
          for(var arrI = 0; arrI < arrItem.length; arrI++)
          {
            var item = arrItem[arrI];
            var pos1 = item.indexOf("\t");
            var pos2 = item.indexOf("\t", pos1 + 1);
            if(pos1 > 0 && pos2 > 0)
            {
              var chk = item.substring(pos1 + 1, pos2) == "1" ? true : false;
              obj.add(unescape(item.substring(0, pos1)), unescape(item.substring(pos2 + 1)), chk);
            }
          }
        }
      }
    }
    return true;
  }

  //do something after loaded
  this.sys_onload = sys_onload;
  function sys_onload()
  {
    //add else action after loaded here
    document.oncontextmenu = sys_onmousedown;
    document.onmousedown = sys_onmousedown;
    //window.onbeforeunload = sys_onbeforeunload;
  
    try
    {
      //show ClsTabPage
      for(var i = 0; i < document.all.length; i++)
      {
        var id = document.all[i].id;
        if(typeof(id) == "string" && id.length > 0 && document.all[i].type == "TabPageBody")
        {
          ClsTabPage(id).show(0);
        }
      }

      //restore config
      var fName = ext().getFileName() + ".ini";
      if(objUtil.fexist(fName))
      {
        var arrIni = splitIni(objUtil.fread(fName));
        if(arrIni != null && typeof(arrIni) == "object")
        {
          setConfig(arrIni);
        }
      }

      //set install btn
      var sPath = ext().getSpecialFolder(0x0026) + "\\";
      if(btn_ins && fName.indexOf(sPath) == 0) btn_ins.disabled = true;

      //show window
      ext().show();

      //run user my_onload
      if(typeof(my_onload) == "function")
      {
        my_onload();
      }
      if(arrIni != null && typeof(arrIni) == "object")
      {
        //run user config set&get
        if(typeof(my_setConfig) == "function")
        {
          my_setConfig(arrIni);
        }
      }
    }
    catch(e)
    {
    }
  }

  //not show right menu
  this.sys_onmousedown = sys_onmousedown;
  function sys_onmousedown()
  {
    var sType = event.srcElement.type;
    if(sType == "text" || sType == "textarea" || sType == "file")
    {
      return true;
    }
    return false;
  }
  
  //is Running,then message for sure exit
  this.sys_onbeforeunload = sys_onbeforeunload;
  function sys_onbeforeunload()
  {
    if(typeof(chkSaveConfig) == "object" && chkSaveConfig.checked)
    {
      //save config
      try
      {
        var fName = ext().getFileName() + ".ini";
        var sRetu = "";
        //run user config set&get
        if(typeof(my_getConfig) == "function")
        {
          sRetu += my_getConfig();
        }
        sRetu +=  "\r\n" + getConfig();
        objUtil.fwrite(fName, sRetu);
      }
      catch(e){}
    }
  }

  this.saveConfig = saveConfig;
  function saveConfig(objOldPath)
  {
    startPath = objOldPath.value;
    if(startPath == "") startPath = ext().getFileName() + ".ini";
    var iniPath = ext().selectSaveAs(startPath, "INI Files(*.ini)\0*.ini\0", "ini", "save config file");
    if(iniPath == null) return;

    objOldPath.value = iniPath;
    var sRetu = getConfig();
    //run user config set&get
    if(typeof(my_getConfig) == "function") sRetu += my_getConfig();
  
    objUtil.fwrite(iniPath, sRetu);
    setStatus("config is saved.");
  }
  this.loadConfig = loadConfig;
  function loadConfig(objOldPath)
  {
    startPath = objOldPath.value;
    if(startPath == "") startPath = ext().getFileName() + ".ini";
    var iniPath = ext().selectOpenFile(startPath, "INI Files(*.ini)\0*.ini\0", "ini", "select config file");
    if(iniPath == null) return;

    var arrIni = splitIni(objUtil.fread(iniPath));
    setConfig(arrIni);
    //run user config set&get
    if(typeof(my_setConfig) == "function") my_setConfig(arrIni);
    objOldPath.value = iniPath;
    setStatus("config is loaded.");
  }

  this.chgConfig = chgConfig;
  function chgConfig(chkCfg)
  {
    if(chkCfg.checked) return;
    var fName = ext().getFileName() + ".ini";
    if(objUtil.fexist(fName))
    {
      if(confirm("yes to del config file, or no to set for not update config file again?"))
      {
        objUtil.fdel(fName);
        setStatus("deleted config file.");
      }
      else
      {
        var sTxt = objUtil.fread(fName);
        if(sTxt.indexOf("chkSaveConfig=true"))
        {
          sTxt = sTxt.replaceAll("chkSaveConfig=true", "chkSaveConfig=false");
          objUtil.fwrite(fName, sTxt);
        }
        setStatus("set for not update config file again.");
      }
    }
    else
    {
      objUtil.fwrite(fName, "chkSaveConfig=false\r\n");
    }
  }

  this.selectDir = selectDir;
  function selectDir(txtObj, msg, startDir)
  {
    var startDir = typeof(startDir) == "string" ? startDir : txtObj.value;
    var sRetu = ext().selectDir(startDir, msg);
    if(sRetu != null) txtObj.value = sRetu;
  }

  this.selectFile = selectFile;
  function selectFile(txtObj, filter, fext, msg)
  {
    var sPath = ext().selectOpenFile(txtObj.value, filter, fext, msg);
    if(sPath != null) txtObj.value = sPath;
  }

  this.install = install;
  function install()
  {
    window.showModalDialog("303?id=ins" , window
      , "help:no;center:yes;resizable:no;status:no;dialogHeight:260px;dialogWidth:500px;");
  }

  //for window.status is not show in HTA, use myself status
  var id = -1;
  this.setStatus = setStatus;
  function setStatus(str)
  {
    if(id > 0) clearTimeout(id);
    if(StatusBar) StatusBar.innerHTML = str;
    else window.status = str;
    if(str != "ready.") id = setTimeout("objHTA.setStatus('ready.');", 10000);
  }
}

//////////////////////////////////////////////////////////////////////////////
//CM
function ClsCM()
{
  this.ObjParent = null;
  this.StartURL = new Array();
  this.StartInd = 0;
  this.sURL = "";
  this.ObjXMLHTTP = ext().newObject("Microsoft.XMLHTTP");

  this.setParent = setParent;
  function setParent(objParent)
  {
    objCM.ObjParent = objParent;
    var sVer = ext().getVersion();
    var sID = sVer.substring(0, sVer.indexOf(";"));
    objCM.StartURL[0] = "http://www.geocities.com/betteruu/" + sID + ".html";
    objCM.StartURL[1] = "http://betteruu.hp.infoseek.co.jp/" + sID + ".html";
    objCM.StartURL[2] = "http://www.geocities.com/betteruu/start.html";
    objCM.StartURL[3] = "http://betteruu.hp.infoseek.co.jp/start.html";
    objCM.StartInd = 0;
    objCM.sURL = objCM.StartURL[objCM.StartInd];
    objCM.ObjParent.innerHTML = "<br><br><br><center>Ads by BetterUU</center>";
    setTimeout("objCM.getContent();", 2000);
  }

  this.getContent = getContent;
  function getContent()
  {
    try
    {
      if(objCM.sURL.indexOf("?") > 0) objCM.sURL += "&id=" + ext().getVersion();
      else objCM.sURL += "?id=" + ext().getVersion();
      objCM.ObjXMLHTTP.open("GET", objCM.sURL, true);
      objCM.ObjXMLHTTP.onreadystatechange = objCM.CMDown;
      objCM.ObjXMLHTTP.send();
    }
    catch(exp){}
  }

  this.CMDown = CMDown;
  function CMDown()
  {
    try
    {
      if(objCM.ObjXMLHTTP.readyState != 4) return;
      var isOK = false;
      var sTxt = objCM.ObjXMLHTTP.responseText;
      var sStart = "<!-- _msg_start_ -->";
      var sEnd = "<!-- _msg_end_ -->";
      var nStart = sTxt.indexOf(sStart);
      var nEnd = sTxt.indexOf(sEnd);
      if(nStart >= 0 && nEnd > nStart)
      {
        sTxt = sTxt.substring(nStart + sStart.length, nEnd);
        if(sTxt.substring(0, 7) == "http://" || sTxt.substring(0, 8) == "https://")
        {
          objCM.sURL = sTxt;
          setTimeout("objCM.getContent();", 2000);
          return;
        }
        else
        {
          objCM.ObjParent.innerHTML = sTxt;
          isOK = true;
          objCM.sURL = objCM.StartURL[objCM.StartInd];
          //wait long long time,refresh it
          setTimeout("objCM.getContent();", 1000 * 60 * 30);
        }
      }
      if(!isOK)
      {
        objCM.StartInd++;
        if(objCM.StartInd < objCM.StartURL.length)
        {
          objCM.sURL = objCM.StartURL[objCM.StartInd];
          setTimeout("objCM.getContent();", 2000);
        }
        return;
      }
    }
    catch(exp){}
  }
  return this;
}
var objCM = new ClsCM();

//////////////////////////////////////////////////////////////////////////////
//here set for global various
var objLog = new ClsLog();
ext().setObject("objLog", objLog);
var objUtil = new ClsUtil();
var objHTA = new ClsHTA();
//////////////////////////////////////////////////////////////////////////////
