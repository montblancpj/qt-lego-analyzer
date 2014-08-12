import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import MontBlanc 1.0
import '../Shapes'
import '../Common'
import '../Forms'

ColumnLayout {
    id: viewContainer

    Storage {
        id: localStorage
        name: 'LegoAnalyzerV2'
        description: 'Lego Analyzer v2 Settings'
    }

    RowLayout {
        id: images
        clip: true
        spacing: 0

        width: parent.width
        Layout.maximumHeight: parent.height - forms.height
        Layout.maximumWidth: parent.width


        GroupBox {
            Layout.minimumHeight: parent.height
            Layout.minimumWidth: parent.width / 2

            clip: true

            OpenCVImage {
                id: homographyImage;
                anchors.fill: parent
                image: window.homographyImage
            }

            Rectangle {
                anchors.fill: parent
                color: '#00000000'

                LinearMesh {
                    id: targetArea
                    lineColor: '#ff0000'
                    numX: meshNumXSlider.value
                    numY: meshNumYSlider.value
                    deltaX: meshDeltaXSlider.value
                    deltaY: meshDeltaYSlider.value
                    x: meshXSlider.value * homographyImage.width;
                    y: meshYSlider.value * homographyImage.height;
                    width: meshWidthSlider.value * homographyImage.width;
                    height: meshHeightSlider.value * homographyImage.height;

                    Rectangle {
                        border.color: targetArea.lineColor
                        border.width: 2
                        anchors.fill: parent
                        color: '#00000000'
                    }

                    MouseArea {
                        id: dragMouseArea
                        anchors.fill: parent
                        drag.target: targetArea
                        drag.axis: Drag.XAndYAxis
                        cursorShape: Qt.PointingHandCursor
                        property int deltaX: 0
                        property int deltaY: 0
                        property int deltaMax: 20
                        onPositionChanged: {
                            meshXSlider.setValue(targetArea.x / homographyImage.width);
                            meshYSlider.setValue(targetArea.y / homographyImage.height);
                        }
                    }

                    MouseArea {
                        id: expandMouseArea
                        width: 10
                        height: 10
                        cursorShape: Qt.SizeFDiagCursor
                        anchors.right: dragMouseArea.right
                        anchors.bottom: dragMouseArea.bottom
                        property int baseX: 0
                        property int baseY: 0
                        onClicked: {
                            baseX = mouse.x;
                            baseY = mouse.y;
                        }
                        onPositionChanged: {
                            var newWidth  = meshWidthSlider.value  + (mouse.x - baseX) / homographyImage.width;
                            var newHeight = meshHeightSlider.value + (mouse.y - baseY) / homographyImage.height;
                            meshWidthSlider.setValue(newWidth);
                            meshHeightSlider.setValue(newHeight);
                        }
                    }

                    function calcTargetRects() {
                        var meshRects = getRects();
                        var x      = meshXSlider.value;
                        var y      = meshYSlider.value;
                        var width  = meshWidthSlider.value;
                        var height = meshHeightSlider.value;
                        return meshRects.map(function(rect, i) {
                            rect.x = x + rect.x * width;
                            rect.y = y + rect.y * height;
                            rect.width  *= width;
                            rect.height *= height;
                            return rect;
                        });
                    }
                }
            }

        }

        GroupBox {
            Layout.minimumHeight: parent.height
            Layout.minimumWidth: parent.width / 2

            LevelAnalyzedImage {
                id: levelAnalyzedImage
                anchors.fill: parent
                inputImage: homographyImage.image
                targetRects: targetArea.calcTargetRects()
                onInputImageChanged: analyze()
                onResultChanged: {
                    //console.log('Result:', result);
                }
                onHeightsChanged: {
                    //console.log('Heights:', heights);
                }

                LinearMesh {
                    id: detectedArea
                    lineColor: '#33ffffff'
                    numX: meshNumXSlider.value
                    numY: meshNumYSlider.value
                    deltaX: meshDeltaXSlider.value
                    deltaY: meshDeltaYSlider.value
                    x: meshXSlider.value * homographyImage.width;
                    y: meshYSlider.value * homographyImage.height;
                    width: meshWidthSlider.value * homographyImage.width;
                    height: meshHeightSlider.value * homographyImage.height;
                    texts: levelAnalyzedImage.heights
                    backgrounds: levelAnalyzedImage.result.map(function(change) {
                        switch (change) {
                            case 1  : return '#66ff0000';
                            case -1 : return '#660000ff';
                        }
                        return undefined;
                    })
                }
            }
        }
    }

    GroupBox {
        property int sliderWidth: 50

        id: forms
        anchors.top: images.bottom

        Layout.maximumHeight: 100
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

        ScrollView {
            anchors.fill: parent

            RowLayout {

                InputSlider {
                    id: meshNumXSlider
                    width: sliderWidth
                    label: 'Mesh Num X'
                    min: 1
                    max: 50
                    fixedLength: 0
                    defaultValue: localStorage.get('calib-numXSlider') || 10
                    onValueChanged: localStorage.set('calib-numXSlider', value)
                }

                InputSlider {
                    id: meshNumYSlider
                    width: sliderWidth
                    label: 'Mesh Num Y'
                    min: 1
                    max: 50
                    fixedLength: 0
                    defaultValue: localStorage.get('calib-numYSlider') || 10
                    onValueChanged: localStorage.set('calib-numYSlider', value)
                }

                InputSlider {
                    id: meshDeltaXSlider
                    width: sliderWidth
                    label: 'delta X'
                    min: -2.0
                    max: 2.0
                    defaultValue: localStorage.get('calib-deltaXSlider') || 0
                    onValueChanged: localStorage.set('calib-deltaXSlider', value)
                }

                InputSlider {
                    id: meshDeltaYSlider
                    width: sliderWidth
                    label: 'delta Y'
                    min: -2.0
                    max: 2.0
                    defaultValue: localStorage.get('calib-deltaYSlider') || 0
                    onValueChanged: localStorage.set('calib-deltaYSlider', value)
                }

                InputSlider {
                    id:thresholdSlider
                    width: sliderWidth
                    label: 'threshold'
                    min: 0
                    max: 255
                    fixedLength: 0
                    defaultValue: localStorage.get('calib-thresholdSlider') || 128
                    onValueChanged: localStorage.set('calib-thresholdSlider', value)
                }

                InputSlider {
                    id: meshXSlider
                    width: sliderWidth
                    label: 'Target X'
                    min: 0.0
                    max: 1.0
                    fixedLength: 4
                    defaultValue: localStorage.get('calib-xSlider') || 0.25
                    onValueChanged: localStorage.set('calib-xSlider', value)
                }

                InputSlider {
                    id: meshYSlider
                    width: sliderWidth
                    label: 'TargetY'
                    min: 0.0
                    max: 1.0
                    fixedLength: 4
                    defaultValue: localStorage.get('calib-ySlider') || 0.25
                    onValueChanged: localStorage.set('calib-ySlider', value)
                }

                InputSlider {
                    id: meshWidthSlider
                    width: sliderWidth
                    label: 'Target Width'
                    min: 0.0
                    max: 1.0
                    fixedLength: 4
                    defaultValue: localStorage.get('calib-widthSlider') || 0.5
                    onValueChanged: localStorage.set('calib-widthSlider', value)
                }

                InputSlider {
                    id: meshHeightSlider
                    width: sliderWidth
                    label: 'Target Height'
                    min: 0.0
                    max: 1.0
                    fixedLength: 4
                    defaultValue: localStorage.get('calib-heightSlider') || 0.5
                    onValueChanged: localStorage.set('calib-heightSlider', value)
                }

            }
        }

    }
}
