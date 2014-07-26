import QtQuick 2.0

Item {
    property color lineColor   : '#eeff0000'
    property color shadowColor : '#88ffffff'
    property int   numX        : 10
    property int   numY        : 10
    property var   texts       : []
    property var   changes     : []

    onNumXChanged  : canvas.requestPaint();
    onNumYChanged  : canvas.requestPaint();
    onTextsChanged : canvas.requestPaint();

    Canvas {
        id: canvas
        anchors.fill: parent

        renderTarget   : Canvas.Image
        renderStrategy : Canvas.Immediate

        onPaint: {
            var context = canvas.getContext('2d');
            context.clearRect(0, 0, canvas.width, canvas.height);
            var grid = {
                width  : canvas.width  / numX,
                height : canvas.height / numY
            }

            context.strokeStyle   = lineColor;
            context.fillStyle     = lineColor;
            context.shadowColor   = shadowColor;
            //context.shadowOffsetX = 1;
            //context.shadowOffsetY = 1;
            //context.shadowBlur    = 3;
            context.font          = ((grid.width > grid.height) ? Math.floor(grid.height/2) : Math.floor(grid.width/3)) + 'px bold';
            context.textAlign     = 'center';
            context.textBaseline  = 'middle';

            context.beginPath(); {
                for (var i = 0; i <= numX; ++i) {
                    context.moveTo(grid.width * i, 0);
                    context.lineTo(grid.width * i, canvas.height);
                }
                for (var i = 0; i <= numY; ++i) {
                    context.moveTo(0, grid.height * i);
                    context.lineTo(canvas.width, grid.height * i);
                }
            } context.stroke();

            for (var i = 0; i < texts.length; ++i) {
                for (var j = 0; j < texts[0].length; ++j) {
                    var x = grid.width  * i + grid.width/2;
                    var y = grid.height * j + grid.height/2;
                    if (changes[i] !== undefined && changes[i][j] !== undefined) {
                        texts[i][j] += changes[i][j] > 0 ? '+' : changes[i][j] < 0 ? '-' : '';
                    }
                    context.fillText(texts[i][j], x, y);
                }
            }
        }
    }
}
