const String index_html = "\r\n\
\r\n\
\r\n\
\r\n\
<!DOCTYPE html> \r\n\
<html lang='ja'> \r\n\
\r\n\
\r\n\
\r\n\
<head> \r\n\
<meta charset='UTF-8'/> \r\n\
<meta name='viewport' content='width=320, initial-scale=1.0, user-scalable=no'> \r\n\
<title>Car Controller</title> \r\n\
<style type='text/css'><!-- \r\n\
.btn_LX { width:124px; height: 36px; text-align:center; padding:8px; border-radius:7px; background-color:#CCCCCC; } \r\n\
--> \r\n\
</style> \r\n\
</head> \r\n\
<body> \r\n\
 \r\n\
<section>\r\n\
<CENTER>\r\n\
　　X座標：<input type='text' id='txtX' />\r\n\
　　Y座標：<input type='text' id='txtY' />\r\n\
　　Steer%：<input type='text' id='Steer' />\r\n\
　　Forward%：<input type='text' id='Forward' />\r\n\
\r\n\
<div id='relative'>\r\n\
<canvas id='drag_area' />  \r\n\
</CENTER>\r\n\
</div>\r\n\
\r\n\
</section>\r\n\
\r\n\
<script type='text/javascript'> \r\n\
  var canvas = document.getElementById('drag_area');\r\n\
\r\n\
  window.onload = function() {\r\n\
    var ctx = canvas.getContext('2d');\r\n\
\r\n\
    function fitCanvasSize() {\r\n\
      canvas.width  = document.documentElement.clientWidth*0.9;\r\n\
      canvas.height = document.documentElement.clientHeight*0.9;\r\n\
      \r\n\
      \r\n\
      if( canvas.width < canvas.height ){\r\n\
        canvas.height = canvas.width ;\r\n\
      }else{\r\n\
        canvas.width = canvas.height ;\r\n\
      }\r\n\
\r\n\
  \r\n\
      ctx.lineWidth = 10;\r\n\
      linew = canvas.width*0.05;\r\n\
\r\n\
      // 角丸四角形 \r\n\
      ctx.fillStyle = 'rgb(100, 100, 100)';\r\n\
      fillRoundRect(ctx, 0, 0, canvas.width, canvas.height, canvas.width*0.06);\r\n\
      ctx.fillStyle = 'rgb(200, 200, 200)';\r\n\
      fillRoundRect(ctx, linew/2, linew/2, canvas.width-linew, canvas.height-linew, canvas.width*0.05);\r\n\
      \r\n\
      var w = canvas.width;\r\n\
      var h = canvas.height;\r\n\
      var per5 = w * 0.05;\r\n\
      ctx.beginPath();\r\n\
      ctx.moveTo(0+per5,h/2);\r\n\
      ctx.lineTo(w/2-per5,h/2);\r\n\
      ctx.moveTo(w/2+per5,h/2);\r\n\
      ctx.lineTo(w-per5,h/2);\r\n\
\r\n\
      ctx.moveTo(w/2,0+per5);\r\n\
      ctx.lineTo(w/2,h/2-per5);\r\n\
      ctx.moveTo(w/2,h/2+per5);\r\n\
      ctx.lineTo(w/2,h-per5);\r\n\
\r\n\
      ctx.lineWidth = 0.5;\r\n\
      ctx.stroke();\r\n\
      \r\n\
    }\r\n\
\r\n\
    fitCanvasSize();\r\n\
    window.onresize = fitCanvasSize;\r\n\
  }\r\n\
\r\n\
\r\n\
var elements = document.getElementsByClassName('drag-and-drop');\r\n\
var x;\r\n\
var y;\r\n\
\r\n\
  var xhr = new XMLHttpRequest(), reqSend = 0, reqRet = 0; \r\n\
  xhr.onreadystatechange = HttpRes; \r\n\
  function HttpRes()  { \r\n\
    if (xhr.readyState == 4 && xhr.status == 200) reqRet = 1; \r\n\
  } \r\n\
\r\n\
  function HttpReq(v) { \r\n\
    xhr.open('GET', '/rc?' + v); \r\n\
    xhr.send(null); \r\n\
    reqRet = 0; \r\n\
  } \r\n\
\r\n\
  function nmn(n) { \r\n\
    if (n > -5 && n < 5) return(0); \r\n\
    if (n < -64) n = -64; \r\n\
    if (n >  64) n =  64; \r\n\
    return(n); \r\n\
  } \r\n\
\r\n\
  var vals = '', Fow = 0, St = 0; \r\n\
  var nowDrag = 0;\r\n\
\r\n\
  function ratioCmp(n) { \r\n\
\r\n\
    var abs_n_wk0 = (Math.abs(n) - 5)*100/90;\r\n\
    \r\n\
    if( abs_n_wk0 < 0 ){\r\n\
        abs_n_wk0 = 0;\r\n\
    }\r\n\
    if( abs_n_wk0 > 100 ){\r\n\
        abs_n_wk0 = 100;\r\n\
    }\r\n\
    \r\n\
    if( n < 0 ){\r\n\
        return ( -1 * abs_n_wk0 );\r\n\
    }else{\r\n\
        return ( abs_n_wk0 );\r\n\
    }\r\n\
   \r\n\
  }\r\n\
\r\n\
/**\r\n\
 * 角が丸い四角形のパスを作成する\r\n\
 * @param  {CanvasRenderingContext2D} ctx コンテキスト\r\n\
 * @param  {Number} x   左上隅のX座標\r\n\
 * @param  {Number} y   左上隅のY座標\r\n\
 * @param  {Number} w   幅\r\n\
 * @param  {Number} h   高さ\r\n\
 * @param  {Number} r   半径\r\n\
 */\r\n\
function createRoundRectPath(ctx, x, y, w, h, r) {\r\n\
    ctx.beginPath();\r\n\
    ctx.moveTo(x + r, y);\r\n\
    ctx.lineTo(x + w - r, y);\r\n\
    ctx.arc(x + w - r, y + r, r, Math.PI * (3/2), 0, false);\r\n\
    ctx.lineTo(x + w, y + h - r);\r\n\
    ctx.arc(x + w - r, y + h - r, r, 0, Math.PI * (1/2), false);\r\n\
    ctx.lineTo(x + r, y + h);       \r\n\
    ctx.arc(x + r, y + h - r, r, Math.PI * (1/2), Math.PI, false);\r\n\
    ctx.lineTo(x, y + r);\r\n\
    ctx.arc(x + r, y + r, r, Math.PI, Math.PI * (3/2), false);\r\n\
    ctx.closePath();\r\n\
}\r\n\
\r\n\
/**\r\n\
 * 角が丸い四角形を塗りつぶす\r\n\
 * @param  {CanvasRenderingContext2D} ctx コンテキスト\r\n\
 * @param  {Number} x   左上隅のX座標\r\n\
 * @param  {Number} y   左上隅のY座標\r\n\
 * @param  {Number} w   幅\r\n\
 * @param  {Number} h   高さ\r\n\
 * @param  {Number} r   半径\r\n\
 */\r\n\
function fillRoundRect(ctx, x, y, w, h, r) {\r\n\
    createRoundRectPath(ctx, x, y, w, h, r);\r\n\
    ctx.fill();\r\n\
}\r\n\
\r\n\
/**\r\n\
 * 角が丸い四角形を描画\r\n\
 * @param  {CanvasRenderingContext2D} ctx コンテキスト\r\n\
 * @param  {Number} x   左上隅のX座標\r\n\
 * @param  {Number} y   左上隅のY座標\r\n\
 * @param  {Number} w   幅\r\n\
 * @param  {Number} h   高さ\r\n\
 * @param  {Number} r   半径\r\n\
 */\r\n\
function strokeRoundRect(ctx, x, y, w, h, r) {\r\n\
    createRoundRectPath(ctx, x, y, w, h, r);\r\n\
    ctx.stroke();       \r\n\
}\r\n\
\r\n\
\r\n\
function pt_move(e){\r\n\
\r\n\
    if(e.type === 'mousemove') {\r\n\
        var ev = e;\r\n\
    } else {\r\n\
        var ev = e.changedTouches[0];\r\n\
    }\r\n\
    \r\n\
    if( nowDrag == 1){\r\n\
  \r\n\
    var mX = ev.pageX - canvas.offsetLeft; \r\n\
    var mY = ev.pageY - canvas.offsetTop; \r\n\
    \r\n\
    var mX_ratio = (ev.pageX - canvas.offsetLeft)/(canvas.width); \r\n\
    var mY_ratio = (ev.pageY - canvas.offsetTop) /(canvas.height); \r\n\
    \r\n\
    var mX_ratio100 = mX_ratio * 200 - 100;\r\n\
    var mY_ratio100 = (mY_ratio * 200 - 100)*-1; \r\n\
    \r\n\
    var mX_ratioCmd =  ratioCmp( mX_ratio100 );\r\n\
    var mY_ratioCmd =  ratioCmp( mY_ratio100 );\r\n\
    St  = mX_ratioCmd; \r\n\
    Fow = mY_ratioCmd;    \r\n\
    document.getElementById('txtX').value = mX;\r\n\
    document.getElementById('txtY').value = mY;\r\n\
    \r\n\
    document.getElementById('Steer').value   = Math.round( St );\r\n\
    document.getElementById('Forward').value = Math.round( Fow );\r\n\
\r\n\
    reqSend++; \r\n\
    }\r\n\
    \r\n\
}\r\n\
\r\n\
function pt_up(){\r\n\
      nowDrag = 0;\r\n\
      St = 0;\r\n\
      Fow = 0;\r\n\
      document.getElementById('Steer').value   = Math.round( St );\r\n\
      document.getElementById('Forward').value = Math.round( Fow );\r\n\
}\r\n\
\r\n\
function pt_down(){\r\n\
      nowDrag = 1;  \r\n\
      \r\n\
      document.getElementById('Steer').value   = Math.round( St );\r\n\
      document.getElementById('Forward').value = Math.round( Fow );    \r\n\
\r\n\
}\r\n\
  \r\n\
  canvas.addEventListener('mousemove', (ev) => { \r\n\
    pt_move(ev);\r\n\
    \r\n\
  });   \r\n\
  canvas.addEventListener('touchmove', (ev) => { \r\n\
    pt_move(ev);\r\n\
    \r\n\
  }); \r\n\
  \r\n\
    window.addEventListener('mouseup', () => { \r\n\
      pt_up();\r\n\
    });\r\n\
    window.addEventListener('touchend', () => { \r\n\
      pt_up();\r\n\
    });\r\n\
    \r\n\
    canvas.addEventListener('mousedown', () => { \r\n\
       pt_down();\r\n\
    });    \r\n\
    canvas.addEventListener('touchstart', () => { \r\n\
       pt_down();\r\n\
    }); \r\n\
\r\n\
  var timer = window.setInterval(() => { \r\n\
    vals = ''; \r\n\
    //if (nowDrag > 0) { \r\n\
    vals = 'Fow=' + Fow + '&St=' + St ; \r\n\
    //} \r\n\
    HttpReq(vals); \r\n\
  }, 200); \r\n\
\r\n\
for(var i = 0; i < elements.length; i++) {\r\n\
    elements[i].addEventListener('mousedown', mdown, false);\r\n\
    elements[i].addEventListener('touchstart', mdown, false);\r\n\
}\r\n\
  \r\n\
</script> \r\n\
</body> \r\n\
</html>\r\n\
\r\n\
\r\n\
\r\n\
";