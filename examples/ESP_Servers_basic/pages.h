static const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta charset="utf-8" />
    <title>Basic</title>
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
        function gotMessage(data) {
          var ar = JSON.parse(data);
          for (id in ar) {
              switch (id) {
                  case 'url':
                      currentUrl = ar.url;
                      $("#prog").val(ar.url);
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
            sock.connect();
        });
    </script>
</head>
<body>
    <div style="margin:auto;width:320px">
        <button id="line" style="float:right;background-color:red;color:white">Offline</button>
        <h1>Basic</h1>
    </div>
</body>
</html>
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

