import QtQuick 2.1

Text {
    property real fps: 0.0
    property real previousTime: 0.0
    font.pixelSize: 24
    font.bold: true
    color: '#aaffffff'
    text: fps.toFixed(0)

    function update() {
        var currentTime = +new Date();
        fps = 1000.0 / (currentTime - previousTime);
        previousTime = currentTime;
    }
}
