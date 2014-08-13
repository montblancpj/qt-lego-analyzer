import QtQuick 2.1
import OSC 1.0

OSCSender {
    id: sender
    property int  fps: 30
    property bool isOutputLog: false

    Timer {
        id: timer
        property int fps: parent.fps
        property var queue: []
        repeat: true
        interval: 1000 / fps
        running: true
        onTriggered: {
            var msg = queue.shift();
            if (msg !== undefined) {
                if (isOutputLog) console.log(msg);
                sender.send.apply(sender, Array.prototype.slice.call(msg, 0));
            }
        }
    }

    function register() {
        timer.queue.push( Array.prototype.slice.call(arguments, 0) );
    }
}
