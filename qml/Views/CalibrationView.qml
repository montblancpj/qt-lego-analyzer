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
            Layout.preferredHeight: parent.height
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

                Mesh {
                    id: targetArea
                    lineColor: '#ff0000'
                    numX: meshNumXSlider.value
                    numY: meshNumYSlider.value
                    x: meshXSlider.value * homographyImage.width;
                    y: meshYSlider.value * homographyImage.height;
                    width: meshWidthSlider.value * homographyImage.width;
                    height: meshHeightSlider.value * homographyImage.height;

                    Rectangle {
                        border.color: targetArea.lineColor
                        border.width: 2
                        anchors.fill: parent
                        color: '#44000000'
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
                            var newWidth  = meshWidthSlider.value + (mouse.x - baseX) / homographyImage.width;
                            var newHeight = meshHeightSlider.value + (mouse.y - baseY) / homographyImage.height;
                            meshWidthSlider.setValue(newWidth);
                            meshHeightSlider.setValue(newHeight);
                        }
                    }
                }
            }

        }

        GroupBox {
            Layout.preferredHeight: parent.height
            Layout.minimumWidth: parent.width / 2

            OpenCVImage {
                id: calibratedImage
                anchors.fill: parent
                image: homographyImage.image
            }
        }
    }

    GroupBox {
        id: forms
        anchors.top: images.bottom

        Layout.minimumHeight: 40
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

        RowLayout {
            anchors.fill: parent
            property int sliderWidth: 80

            InputSlider {
                id: meshXSlider
                sliderWidth: sliderWidth
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

            InputSlider {
                id: meshNumXSlider
                width: sliderWidth
                label: 'Mesh Num X'
                min: 1
                max: 50
                defaultValue: localStorage.get('calib-numXSlider') || 10
                onValueChanged: localStorage.set('calib-numXSlider', value)
            }

            InputSlider {
                id: meshNumYSlider
                width: sliderWidth
                label: 'Mesh Num Y'
                min: 1
                max: 50
                defaultValue: localStorage.get('calib-numYSlider') || 10
                onValueChanged: localStorage.set('calib-numYSlider', value)
            }
        }
    }

}
