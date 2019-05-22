/*************************************************
Function: Common Class for JavaScript
Author: Shu.KK
2005.10
*************************************************/

//////////////////////////////////////////////////////////////////////////////
//ClsMWin
//can use like:var o = new ClsMWin(mwinID, sTitle, nX, nY, nW, nH);o.show();
//also can like:ClsMWin(mwinID, sTitle, nX, nY, nW, nH).show();
var ClsMWin_ZIndex = 0;
function ClsMWin(mwinID, sTitle_or_oParent, nX, nY, nW, nH, bSizable, oParent)
{
  var l_mwinID = "" + mwinID;
  var sTitle = sTitle_or_oParent;
  if(sTitle == null || typeof(sTitle) != "string") sTitle = "Message...";
  if(l_mwinID.substring(0, 8) != "ClsMWin_") l_mwinID = "ClsMWin_" + l_mwinID;
  var oDiv = document.all(l_mwinID);
  if(oDiv == null || (oDiv != null && typeof(oDiv.objMWin) == "object"))
  {
    if(oDiv == null || typeof(oDiv.objMWin) != "object")
    {
      oDiv = document.createElement("DIV");
      oDiv.id = l_mwinID;
      if(sTitle_or_oParent != null && typeof(sTitle_or_oParent) == "object")
      {
        setParent(sTitle_or_oParent);
      }
      else if(oParent != null && typeof(oParent) == "object")
      {
        setParent(oParent);
      }
      else
      {
        setParent(event.srcElement);
      }
      oDiv.objMWin = new ClsMWin(l_mwinID, sTitle_or_oParent, nX, nY, nW, nH, bSizable, oParent);
    }
    return oDiv.objMWin;
  }

  var isInitOK = false;
  var Window_ex = 0;
  var Window_ey = 0;
  var Window_drag = false;
  var l_Top = 0;
  var l_Left = 0;
  var Window_dragS = false;
  var l_Sizable = true;
  //init process
  init();
  setSizable(bSizable);

  //private function
  function isValid()
  {
    return (oDiv != null && isInitOK);
  }

  //private function
  function init()
  {
    if(oDiv == null) return true;

    var bord1 = 'BORDER-TOP:#f0f0f0 2px solid;BORDER-RIGHT:#606060 2px solid;'
              + 'BORDER-BOTTOM:#404040 2px solid;BORDER-LEFT:#d0d0d0 2px solid;';
    var bord2 = 'BORDER-TOP:#404040 2px solid;BORDER-RIGHT:#e0e0e0 2px solid;'
              + 'BORDER-BOTTOM:#f0f0f0 2px solid;BORDER-LEFT:#505050 2px solid;';

    if(typeof(nW) != "number" || nW < 8) nW = 8;
    if(typeof(nH) != "number" || nH < 31) nH = 31;
    if(typeof(nX) == "undefined") nX = 0;
    if(typeof(nY) == "undefined") nY = 0;
    l_Top = nY;
    l_Left = nX;
    var subw = nW - 7;
    var subh = nH - 30;
    var sdiv = '<IFRAME FRAMEBORDER=0 style="position:absolute;top:0px;left:0px;height:100%;width:100%;'
      + 'z-index:0;"></IFRAME><DIV style="position:absolute;BACKGROUND-COLOR:#c0c0c0;'
      + 'top:0px;left:0px;z-index:1;height:100%;width:100%;' + bord1 + '"></DIV>'
      + '<DIV  id="'+l_mwinID+'_head" style="position:absolute;top:4px;left:4px;z-index:2;height:22px;width:'
      + subw + 'px;cursor:default;overflow:scroll;overflow-x:hidden;overflow-y:hidden;" '
      + ' onMousedown="ClsMWin(\'' + l_mwinID + '\').mouseDown();" onMousemove="ClsMWin(\'' + l_mwinID + '\').mouseMove();" '
      + 'onMouseup="ClsMWin(\'' + l_mwinID + '\').mouseUp();" onSelectStart="return false">'
      + '<table style="width:100%;BACKGROUND-COLOR:#008080;COLOR:#ffffff;FONT-SIZE:8pt;MARGIN:0px;">'
      + '<tr><td id="'+l_mwinID+'_title" nowrap>' + sTitle + '</td><td style="width:10px;">'
      + '<span style="font-family:wingdings;FONT-SIZE:9pt;cursor:hand;height:13;" '
      + 'onclick="ClsMWin(\'' + l_mwinID + '\').hide();">&#253;</span>'
      + '</td></tr></table></DIV>'
      + '<DIV id="' + l_mwinID + '_body" style="position:absolute;top:26px;left:4px;height:'
      + subh + 'px;width:' + subw + 'px;overflow-x:auto;overflow-y:auto;z-index:2;'
      + 'BACKGROUND-COLOR:#ffffff;COLOR:#000000;' + bord2+'"></DIV>'
      + '<div style="position:absolute;top:100%;left:100%;z-index:20;">'
      + '<table id="'+l_mwinID+'_sizable" style="position:absolute;top:-8;left:-8;z-index:21;cursor:hand;width:8;height:8;'
      + 'BORDER:#f0f0f0 1px solid;" onMousedown="ClsMWin(\'' + l_mwinID + '\').mouseDownS();" '
      + 'onmousemove="ClsMWin(\'' + l_mwinID + '\').mouseMoveS();" '
      + 'onMouseup="ClsMWin(\'' + l_mwinID + '\').mouseUp();"><tr><td></td></tr></table></div>';

    oDiv.innerHTML = sdiv;
    oDiv.style.cssText = 'position:absolute;left:' + nX + ';top:' + nY 
      + ';height:' + nH + ';width:' + nW + ';BACKGROUND-COLOR:#008080;';
    oDiv.style.zIndex = ++ClsMWin_ZIndex;
    oDiv.onmousedown = function(){eval("this.style.zIndex = ++ClsMWin_ZIndex");};
    if(typeof(sTitle) == "undefined") sTitle = "Msg";
    document.all(l_mwinID + '_title').innerHTML = sTitle;

    isInitOK = true;
    return true;
  }

  this.mouseDown = mouseDown;
  function mouseDown()
  {
    if(!isValid()) return false;
    Window_ex = window.event.clientX;
    Window_ey = window.event.clientY;
    Window_drag = true;
    document.onmousemove = mouseMove;
  }

  this.mouseMove = mouseMove;
  function mouseMove()
  {
    if(!isValid()) return false;
    if(!Window_drag || !window.event || window.event.button != 1) return;
    l_Left = parseInt(l_Left, 10) + parseInt(event.clientX, 10) - parseInt(Window_ex, 10);
    l_Top = parseInt(l_Top, 10) + parseInt(event.clientY, 10) - parseInt(Window_ey, 10);
    show(l_Left, l_Top);
    Window_ex = window.event.clientX;
    Window_ey = window.event.clientY;
  }

  this.mouseUp = mouseUp;
  function mouseUp()
  {
    Window_drag = false;
    Window_dragS = false;
    document.onmousemove = null;
  }

  this.mouseDownS = mouseDownS;
  function mouseDownS()
  {
    if(!isValid()) return false;
    Window_ex = window.event.clientX;
    Window_ey = window.event.clientY;
    Window_dragS = true;
    document.onmousemove = mouseMoveS;
  }

  this.mouseMoveS = mouseMoveS;
  function mouseMoveS()
  {
    if(!isValid()) return false;
    if(!l_Sizable || !Window_dragS || !window.event || window.event.button != 1) return;
    var w = parseInt(oDiv.style.width, 10) + parseInt(event.clientX, 10) - parseInt(Window_ex, 10);
    var h = parseInt(oDiv.style.height, 10) + parseInt(event.clientY, 10) - parseInt(Window_ey, 10);
    show(l_Left, l_Top, w, h);
    Window_ex = window.event.clientX;
    Window_ey = window.event.clientY;
  }

  this.show = show;
  function show(nX, nY, nW, nH)
  {
    if(!isValid()) return false;
    oDiv.style.zIndex = ++ClsMWin_ZIndex;
    oDiv.style.display = '';

    if(typeof(nX) != "undefined")
    {
      l_Left = nX;
      oDiv.style.left = nX;
    }
    if(typeof(nY) != "undefined")
    {
      l_Top = nY;
      oDiv.style.top = nY;
    }
    if(typeof(nW) != "undefined" && typeof(nH) != "undefined")
    {
      if(nW < 8) nW = 8;
      if(nH < 31) nH = 31;
      document.all(l_mwinID + '_head').style.width = nW - 7;
      document.all(l_mwinID + '_body').style.width = nW - 7;
      document.all(l_mwinID + '_body').style.height = nH - 30;
      oDiv.style.width = nW;
      oDiv.style.height = nH;
    }
    return true;
  }

  this.hide = hide;
  function hide()
  {
    if(!isValid()) return false;
    oDiv.style.display = 'none';
    return true;
  }

  this.setTitle = setTitle;
  function setTitle(str)
  {
    if(!isValid()) return false;
    document.all(l_mwinID + '_title').innerHTML = str;
    return true;
  }

  this.setSizable = setSizable;
  function setSizable(bSizable)
  {
    if(!isValid()) return false;
    l_Sizable = true;
    if(typeof(bSizable) == "boolean") l_Sizable = bSizable;
    if(l_Sizable)
    {
      document.all(l_mwinID + '_sizable').style.border = '#f0f0f0 1px solid';
      document.all(l_mwinID + '_sizable').style.cursor = 'hand';
    }
    else
    {
      document.all(l_mwinID + '_sizable').style.border = '';
      document.all(l_mwinID + '_sizable').style.cursor = '';
    }
    return true;
  }

  this.setBody = setBody;
  function setBody(str)
  {
    if(!isValid()) return false;
    document.all(l_mwinID + '_body').innerHTML = str;
    return true;
  }

  //can not show in blank page!
  this.setURL = setURL;
  function setURL(sURL)
  {
    if(!isValid()) return false;
    show();
    setTitle(sURL);
    var sIFrm = '<IFRAME src="' + sURL + '" FRAMEBORDER=0 style="width:100%;height:100%;"></IFRAME>';
    document.all(l_mwinID + '_body').innerHTML = sIFrm;
    return true;
  }

  this.setParent = setParent;
  function setParent(oParent)
  {
    if(oDiv == null || typeof(oDiv) != "object") return false;
    if(oParent != null && typeof(oParent) == "object")
    {
      if(oParent.canHaveChildren)
      {
        oParent.insertBefore(oDiv);
      }
      else if(oParent.parentElement)
      {
        oParent.parentElement.insertBefore(oDiv);
      }
      else
      {
        document.body.insertBefore(oDiv);
      }
    }
  }

  //avoids this error:ClsXXX("not exist").xxx();
  return this;
}

