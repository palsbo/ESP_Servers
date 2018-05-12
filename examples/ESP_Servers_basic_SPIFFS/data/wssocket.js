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
