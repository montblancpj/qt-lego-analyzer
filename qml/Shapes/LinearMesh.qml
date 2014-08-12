import QtQuick 2.0

Item {
    property color lineColor   : '#eeff0000'
    property color shadowColor : '#00000000'
    property color textColor   : '#ffff3333'
    property int   numX        : 10
    property int   numY        : 10
    property real  deltaX      : 1.0 // %
    property real  deltaY      : 0.0 // %
    property var   texts       : []
    property var   backgrounds : []

    onNumXChanged        : meshCanvas.requestPaint();
    onNumYChanged        : meshCanvas.requestPaint();
    onDeltaXChanged      : meshCanvas.requestPaint();
    onDeltaYChanged      : meshCanvas.requestPaint();
    onTextsChanged       : meshCanvas.requestPaint();
    onBackgroundsChanged : meshCanvas.requestPaint();

    Canvas {
        id: meshCanvas
        anchors.fill: parent

        renderTarget   : Canvas.Image
        renderStrategy : Canvas.Immediate

        onPaint: {
            var context = meshCanvas.getContext('2d');
            context.clearRect(0, 0, meshCanvas.width, meshCanvas.height);

            var baseMeshWidth  = meshCanvas.width  / numX;
            var baseMeshHeight = meshCanvas.height / numY;

            context.strokeStyle = lineColor;
            context.font          = 'bold ' + ((baseMeshWidth > baseMeshHeight) ?
                                         Math.floor(baseMeshHeight/2) : Math.floor(baseMeshWidth/3)) + 'px Arial';
            context.textAlign     = 'center';
            context.textBaseline  = 'middle';

            context.beginPath(); {
                getRects().forEach(function(rect, i) {
                    rect.x      *= width;
                    rect.y      *= height;
                    rect.width  *= width;
                    rect.height *= height;

                    if (backgrounds[i]) {
                        context.fillStyle = backgrounds[i];
                        context.fillRect(rect.x, rect.y, rect.width, rect.height);
                    }

                    if (texts[i] !== undefined) {
                        context.fillStyle     = textColor;
                        var x = rect.x + rect.width  / 2;
                        var y = rect.y + rect.height / 2;
                        context.fillText(texts[i], x, y);
                    }

                    context.moveTo(rect.x, rect.y);
                    context.lineTo(rect.x + rect.width, rect.y);
                    context.moveTo(rect.x, rect.y);
                    context.lineTo(rect.x, rect.y + rect.height);
                });
            } context.stroke();
        }
    }

    function getRects() {
        var rects = [];
        var baseMeshWidth  = 1.0 / numX;
        var baseMeshHeight = 1.0 / numY;
        var dw = (deltaX / 100.0 * baseMeshWidth);
        var dh = (deltaY / 100.0 * baseMeshHeight);

        for (var j = 1, y = 0, id = 0; j <= numY; ++j) {
            var height = baseMeshHeight + dh * (j - (numY + 1) / 2);
            for (var i = 1, x = 0; i <= numX; ++i) {
                var width = baseMeshWidth + dw * (i - (numX + 1) / 2);
                rects.push({
                    id     : id++,
                    x      : x,
                    y      : y,
                    width  : width,
                    height : height
                });
                x += width;
            }
            y += height;
        }

        return rects;
    }
}
