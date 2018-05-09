static const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta charset="utf-8" />
    <title>Webradio</title>
    <meta name="viewport" content="width=device-width">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
    <script src="simple-slider.min.js"></script>
    <link href="simple-slider.css" rel="stylesheet" type="text/css" />
    <!--
        Using the module 'wssocket.js' gives auto-connect if server disconnect.
        register the websocket object as this:
        const ws = new WEBSOCKET(url)
    -->
    <script src="wssocket.js"></script>
    <script type="text/javascript">
        var currentUrl;
        
        function selecting(obj) {
            var ar = { 'url': obj.value };
            if (sock) sock.send(JSON.stringify(ar));
        }

        function gotMessage(data) {
          var ar = JSON.parse(data);
          for (id in ar) {
              switch (id) {
                  case 'gain':
                      $("[data-slider]").simpleSlider("setValue", ar.gain/100);
                      break;
                  case 'url':
                      currentUrl = ar.url;
                      $("#prog").val(ar.url);
                      $("#aud").attr("src", ar.url).trigger("play");
                      break;
                  case 'bitrate':
                      $("#bitrate").html(ar['bitrate']);
                      break;
                  case 'genre':
                      $("#genre").html(ar['genre']);
                      break;
                  case 'name':
                      $("#name").html(ar['name']);
                      break;
                  case 'title':
                      $("#title").html(ar.title);
                      break;
              }
          }
        }

        function gotStatus(status) {
            if (status) {
                document.getElementById("line").style.backgroundColor = "green";
                document.getElementById("line").innerText = "Online";
            } else {
                document.getElementById("line").style.backgroundColor = "red";
                document.getElementById("line").innerText = "Offline";
            }
        }

        var sock = new SOCK('ws://'+location.hostname +':81', gotMessage, gotStatus);
        $(function () {
            document.getElementById("aud").addEventListener('volumechange', function () {
                console.log('changed.', Math.round(this.volume * 100));
            }, false);
            $("#vol").bind("slider:ready slider:changed", function (event, data) {
                    var ar = {};
                    ar['gain'] = Math.round(data.value * 100);
                    if (sock) sock.send(JSON.stringify(ar));
                });

            sock.connect();
        });
    </script>
</head>
<body>
    <div style="margin:auto;width:320px">
        <button id="line" style="float:right;background-color:red;color:white">Offline</button>
        <h1>Webradio</h1>
        <h4>Client volume</h4>
        <input id="vol" style="width:100%" type="text" data-slider="true" value="0.8" data-slider-highlight="true">
        <audio id="aud" style="width:100%" src="http://live-icy.gss.dr.dk:80/A/A07H.mp3" controls preload></audio><br />
        <div style="margin-bottom:10px" id="info"></div>
        <select style="width:100%;font-size:larger" onchange="selecting(this)" id="prog">
            <option value="">select station</option>
            <option value="http://streaming.shoutcast.com/80sPlanet?lang=eng-US">80' Planet</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A02H.mp3">DR Nyheder</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A03H.mp3">DR P1</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A04H.mp3">DR P2 Klassisk</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A05H.mp3">DR P3</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A06H.mp3">DR P4 Bornholm</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A15H.mp3">DR P4 Esbjerg</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A07H.mp3">DR P4 Fyn</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A08H.mp3">DR P4 København</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A09H.mp3">DR P4 Midt & Vest</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A10H.mp3">DR P4 Nordjylland</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A11H.mp3">DR P4 Sjælland</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A12H.mp3">DR P4 Syd</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A13H.mp3">DR P4 Trekanten</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A14H.mp3">DR P4 Østjylland</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A25H.mp3">DR P5</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A29H.mp3">DR P6 Beat</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A21H.mp3">DR P7 Mix</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A22H.mp3">DR P8 Jazz</option>
            <option value="http://live-icy.gss.dr.dk:8000/A/A24H.mp3">DR Ramasjang/Ultra</option>
            <option value="http://onair.100fmlive.dk:80/klassisk_live.mp3">Klassisk Live</option>
            <option value="http://stream.novafm.dk:80/nova128">Nova FM</option>
            <option value="http://stream.popfm.dk:80/pop128">Pop FM</option>
            <option value="http://onair.100fmlive.dk:80/100fm_live.mp3">Radio 100 FM</option>
            <option value="http://195.184.101.203/voice128">The Voice</option>
            <option value="http://streammp3.retro-radio.dk/retro-mp3?type=.mp3">Retro-radio</option>
            <option value="http://tx-2.retro-radio.dk/TX-2?type=.mp3">Retro Millenium</option>
        </select>
        <table style="width:100%">
            <tr><td style="vertical-align:top;font-weight:bold">Title:</td><td id="title"></td></tr>
        </table>
        <table style="width:100%">
            <tr>
                <td style="vertical-align:top;font-weight:bold">Station:</td>
                <td style="vertical-align:top;font-weight:bold">Genre:</td>
                <td style="vertical-align:top;font-weight:bold">Bitrate::</td>
            </tr>
            <tr>
                <td id="name"></td>
                <td id="genre"></td>
                <td id="bitrate"></td>
            </tr>
        </table>
    </div>
</body>
</html>
)=====";
static const char simple_slider_min_js[] PROGMEM = R"=====(
/*
 * jQuery Simple Slider: Unobtrusive Numerical Slider
 * Version 1.0.0
 *
 * Copyright (c) 2014 James Smith (http://loopj.com)
 *
 * Licensed under the MIT license (http://mit-license.org/)
 *
 */

var __slice=[].slice,__indexOf=[].indexOf||function(e){for(var t=0,n=this.length;t<n;t++)if(t in this&&this[t]===e)return t;return-1};(function(e,t){var n;return n=function(){function t(t,n){var r,i=this;this.input=t,this.defaultOptions={animate:!0,snapMid:!1,classPrefix:null,classSuffix:null,theme:null,highlight:!1},this.settings=e.extend({},this.defaultOptions,n),this.settings.theme&&(this.settings.classSuffix="-"+this.settings.theme),this.input.hide(),this.slider=e("<div>").addClass("slider"+(this.settings.classSuffix||"")).css({position:"relative",userSelect:"none",boxSizing:"border-box"}).insertBefore(this.input),this.input.attr("id")&&this.slider.attr("id",this.input.attr("id")+"-slider"),this.track=this.createDivElement("track").css({width:"100%"}),this.settings.highlight&&(this.highlightTrack=this.createDivElement("highlight-track").css({width:"0"})),this.dragger=this.createDivElement("dragger"),this.slider.css({minHeight:this.dragger.outerHeight(),marginLeft:this.dragger.outerWidth()/2,marginRight:this.dragger.outerWidth()/2}),this.track.css({marginTop:this.track.outerHeight()/-2}),this.settings.highlight&&this.highlightTrack.css({marginTop:this.track.outerHeight()/-2}),this.dragger.css({marginTop:this.dragger.outerHeight()/-2,marginLeft:this.dragger.outerWidth()/-2}),this.track.mousedown(function(e){return i.trackEvent(e)}),this.settings.highlight&&this.highlightTrack.mousedown(function(e){return i.trackEvent(e)}),this.dragger.mousedown(function(e){if(e.which!==1)return;return i.dragging=!0,i.dragger.addClass("dragging"),i.domDrag(e.pageX,e.pageY),!1}),e("body").mousemove(function(t){if(i.dragging)return i.domDrag(t.pageX,t.pageY),e("body").css({cursor:"pointer"})}).mouseup(function(t){if(i.dragging)return i.dragging=!1,i.dragger.removeClass("dragging"),e("body").css({cursor:"auto"})}),this.pagePos=0,this.input.val()===""?(this.value=this.getRange().min,this.input.val(this.value)):this.value=this.nearestValidValue(this.input.val()),this.setSliderPositionFromValue(this.value),r=this.valueToRatio(this.value),this.input.trigger("slider:ready",{value:this.value,ratio:r,position:r*this.slider.outerWidth(),el:this.slider})}return t.prototype.createDivElement=function(t){var n;return n=e("<div>").addClass(t).css({position:"absolute",top:"50%",userSelect:"none",cursor:"pointer"}).appendTo(this.slider),n},t.prototype.setRatio=function(e){var t;return e=Math.min(1,e),e=Math.max(0,e),t=this.ratioToValue(e),this.setSliderPositionFromValue(t),this.valueChanged(t,e,"setRatio")},t.prototype.setValue=function(e){var t;return e=this.nearestValidValue(e),t=this.valueToRatio(e),this.setSliderPositionFromValue(e),this.valueChanged(e,t,"setValue")},t.prototype.trackEvent=function(e){if(e.which!==1)return;return this.domDrag(e.pageX,e.pageY,!0),this.dragging=!0,!1},t.prototype.domDrag=function(e,t,n){var r,i,s;n==null&&(n=!1),r=e-this.slider.offset().left,r=Math.min(this.slider.outerWidth(),r),r=Math.max(0,r);if(this.pagePos!==r)return this.pagePos=r,i=r/this.slider.outerWidth(),s=this.ratioToValue(i),this.valueChanged(s,i,"domDrag"),this.settings.snap?this.setSliderPositionFromValue(s,n):this.setSliderPosition(r,n)},t.prototype.setSliderPosition=function(e,t){t==null&&(t=!1);if(t&&this.settings.animate){this.dragger.animate({left:e},200);if(this.settings.highlight)return this.highlightTrack.animate({width:e},200)}else{this.dragger.css({left:e});if(this.settings.highlight)return this.highlightTrack.css({width:e})}},t.prototype.setSliderPositionFromValue=function(e,t){var n;return t==null&&(t=!1),n=this.valueToRatio(e),this.setSliderPosition(n*this.slider.outerWidth(),t)},t.prototype.getRange=function(){return this.settings.allowedValues?{min:Math.min.apply(Math,this.settings.allowedValues),max:Math.max.apply(Math,this.settings.allowedValues)}:this.settings.range?{min:parseFloat(this.settings.range[0]),max:parseFloat(this.settings.range[1])}:{min:0,max:1}},t.prototype.nearestValidValue=function(t){var n,r,i,s;return i=this.getRange(),t=Math.min(i.max,t),t=Math.max(i.min,t),this.settings.allowedValues?(n=null,e.each(this.settings.allowedValues,function(){if(n===null||Math.abs(this-t)<Math.abs(n-t))return n=this}),n):this.settings.step?(r=(i.max-i.min)/this.settings.step,s=Math.floor((t-i.min)/this.settings.step),(t-i.min)%this.settings.step>this.settings.step/2&&s<r&&(s+=1),s*this.settings.step+i.min):t},t.prototype.valueToRatio=function(e){var t,n,r,i,s,o,u,a;if(this.settings.equalSteps){a=this.settings.allowedValues;for(i=o=0,u=a.length;o<u;i=++o){t=a[i];if(typeof n=="undefined"||n===null||Math.abs(t-e)<Math.abs(n-e))n=t,r=i}return this.settings.snapMid?(r+.5)/this.settings.allowedValues.length:r/(this.settings.allowedValues.length-1)}return s=this.getRange(),(e-s.min)/(s.max-s.min)},t.prototype.ratioToValue=function(e){var t,n,r,i,s;return this.settings.equalSteps?(s=this.settings.allowedValues.length,i=Math.round(e*s-.5),t=Math.min(i,this.settings.allowedValues.length-1),this.settings.allowedValues[t]):(n=this.getRange(),r=e*(n.max-n.min)+n.min,this.nearestValidValue(r))},t.prototype.valueChanged=function(t,n,r){var i;if(t.toString()===this.value.toString())return;return this.value=t,i={value:t,ratio:n,position:n*this.slider.outerWidth(),trigger:r,el:this.slider},this.input.val(t).trigger(e.Event("change",i)).trigger("slider:changed",i)},t}(),e.extend(e.fn,{simpleSlider:function(){var t,r,i;return i=arguments[0],t=2<=arguments.length?__slice.call(arguments,1):[],r=["setRatio","setValue"],e(this).each(function(){var s,o;return i&&__indexOf.call(r,i)>=0?(s=e(this).data("slider-object"),s[i].apply(s,t)):(o=i,e(this).data("slider-object",new n(e(this),o)))})}}),e(function(){return e("[data-slider]").each(function(){var t,n,r,i;return t=e(this),r={},n=t.data("slider-values"),n&&(r.allowedValues=function(){var e,t,r,s;r=n.split(","),s=[];for(e=0,t=r.length;e<t;e++)i=r[e],s.push(parseFloat(i));return s}()),t.data("slider-range")&&(r.range=t.data("slider-range").split(",")),t.data("slider-step")&&(r.step=t.data("slider-step")),r.snap=t.data("slider-snap"),r.equalSteps=t.data("slider-equal-steps"),t.data("slider-theme")&&(r.theme=t.data("slider-theme")),t.attr("data-slider-highlight")&&(r.highlight=t.data("slider-highlight")),t.data("slider-animate")!=null&&(r.animate=t.data("slider-animate")),t.simpleSlider(r)})})})(this.jQuery||this.Zepto,this);
)=====";
static const char simple_slider_css[] PROGMEM = R"=====(
.slider {
  width: 300px;
}

.slider > .dragger {
  background: #8DCA09;
  background: -webkit-linear-gradient(top, #8DCA09, #72A307);
  background: -moz-linear-gradient(top, #8DCA09, #72A307);
  background: linear-gradient(top, #8DCA09, #72A307);

  -webkit-box-shadow: inset 0 2px 2px rgba(255,255,255,0.5), 0 2px 8px rgba(0,0,0,0.2);
  -moz-box-shadow: inset 0 2px 2px rgba(255,255,255,0.5), 0 2px 8px rgba(0,0,0,0.2);
  box-shadow: inset 0 2px 2px rgba(255,255,255,0.5), 0 2px 8px rgba(0,0,0,0.2);

  -webkit-border-radius: 10px;
  -moz-border-radius: 10px;
  border-radius: 10px;

  border: 1px solid #496805;
  width: 16px;
  height: 16px;
}

.slider > .dragger:hover {
  background: -webkit-linear-gradient(top, #8DCA09, #8DCA09);
}


.slider > .track, .slider > .highlight-track {
  background: #ccc;
  background: -webkit-linear-gradient(top, #bbb, #ddd);
  background: -moz-linear-gradient(top, #bbb, #ddd);
  background: linear-gradient(top, #bbb, #ddd);

  -webkit-box-shadow: inset 0 2px 4px rgba(0,0,0,0.1);
  -moz-box-shadow: inset 0 2px 4px rgba(0,0,0,0.1);
  box-shadow: inset 0 2px 4px rgba(0,0,0,0.1);

  -webkit-border-radius: 8px;
  -moz-border-radius: 8px;
  border-radius: 8px;

  border: 1px solid #aaa;
  height: 4px;
}

.slider > .highlight-track {
  background-color: #8DCA09;
  background: -webkit-linear-gradient(top, #8DCA09, #72A307);
  background: -moz-linear-gradient(top, #8DCA09, #72A307);
  background: linear-gradient(top, #8DCA09, #72A307);
  
  border-color: #496805;
}
)=====";
static const char wssocket_js[] PROGMEM = R"=====(
SOCK = function (host, onMessage, onChange) {
    var obj = this;
    var ws;
    this.debug= false;
    this.send = function (data) {
        //console.log("TX: "+ data)
        ws.send(data);
    }
    this.connect = function () {
        ws = new WebSocket(host);
        function ping() {
            if (connectflag) ws.send('__ping__');
            clearTimeout(tm);
            tm = setTimeout(function () {
                onChange(false);
                if (obj.debug) console.log("WS Disconnected");
                ws.close();
            }, 2000);
        }
        function pong() {
            clearTimeout(tm);
            tn = setTimeout(ping, 2000);
        }
        ws.onopen = function () {
            connectflag = true;
            ws.send(JSON.stringify({ 'time': new Date() }));
            tm = setTimeout(ping, 2000);
            onChange(true);
            if (obj.debug) console.log("WS Connected");
        };

        ws.onclose = function (e) {
            onChange(false);
            setTimeout(function () {
                connectflag = false;
                if (obj.debug) console.log("WS Reconnecting");
                obj.connect();
            }, 100);
        };

        ws.onerror = function (err) {
            if (obj.debug) console.error('Socket encountered error: ', err.message, 'Closing socket');
            ws.close();
        };
        
        ws.onmessage = function (e) {
            var msg = e.data;
            if (msg == '__pong__') {
                pong();
                return;
            }
            if (obj.debug) console.log("RX: " + e.data);
            gotMessage(e.data);
        }
    };
}
)=====";

