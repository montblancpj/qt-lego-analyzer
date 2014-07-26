import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import MontBlanc 1.0
import '../Shapes'
import '../Common'
import '../Forms'

ColumnLayout {
    property alias homographyImage: homographyImage.image

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
            id: pcsdkDepth
            Layout.preferredHeight: parent.height
            Layout.minimumWidth: parent.width / 2

            //PCSDKImage {
            OpenNIImage {
                id: pcsdkDepthImage
                anchors.fill: parent
                //minDistance: minDistanceSlider.value
                //maxDistance: maxDistanceSlider.value
                //gamma: gammaSlider.value

                Timer {
                    interval: 1000/60
                    running: true
                    repeat: true
                    onTriggered: parent.update()
                }

                DeformableBox {
                    id: targetArea
                    anchors.fill: parent
                    normalizeWidth: parent.width
                    normalizeHeight: parent.height

                    leftTopX: localStorage.get('homo-leftTopX') || 10
                    leftTopY: localStorage.get('homo-leftTopY') || 10
                    rightTopX: localStorage.get('homo-rightTopX') || normalizeWidth - 10
                    rightTopY: localStorage.get('homo-rightTopY') || 10
                    rightBottomX: localStorage.get('homo-rightBottomX') || normalizeWidth - 10
                    rightBottomY: localStorage.get('homo-rightBottomY') || normalizeHeight - 10
                    leftBottomX: localStorage.get('homo-leftBottomX') || 10
                    leftBottomY: localStorage.get('homo-leftBottomY') || normalizeHeight - 10

                    onLeftTopXChanged: localStorage.set('homo-leftTopX', leftTopX)
                    onLeftTopYChanged: localStorage.set('homo-leftTopY', leftTopY)
                    onRightTopXChanged: localStorage.set('homo-rightTopX', rightTopX)
                    onRightTopYChanged: localStorage.set('homo-rightTopY', rightTopY)
                    onRightBottomXChanged: localStorage.set('homo-rightBottomX', rightBottomX)
                    onRightBottomYChanged: localStorage.set('homo-rightBottomY', rightBottomY)
                    onLeftBottomXChanged: localStorage.set('homo-leftBottomX', leftBottomX)
                    onLeftBottomYChanged: localStorage.set('homo-leftBottomY', leftBottomY)
                }
            }
        }

        GroupBox {
            id: homography
            Layout.preferredHeight: parent.height
            Layout.minimumWidth: parent.width / 2

            HomographyImage {
                id: homographyImage
                anchors.fill: parent
                image: pcsdkDepthImage.image
                srcPoints: targetArea.points
                onImageChanged: window.homographyImage = image

                LinearMesh {
                    anchors.fill: parent
                    lineColor: '#33ffffff'
                    numX: 24
                    numY: 24
                }
            }
        }
    }

    GroupBox {
        id: forms
        anchors.top: images.bottom

        Layout.minimumHeight: 40
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

        RowLayout {

            InputSlider {
                id: minDistanceSlider
                min: 0
                max: 1000
                defaultValue: localStorage.get('pcsdk-min-distance') || min
                onValueChanged: localStorage.set('pcsdk-min-distance', value)
                label: 'Min Distance'
            }
            InputSlider {
                id: maxDistanceSlider
                min: 0
                max: 1000
                defaultValue: localStorage.get('pcsdk-max-distance') || max
                onValueChanged: localStorage.set('pcsdk-max-distance', value)
                label: 'Max Distance'
            }
            InputSlider {
                id: gammaSlider
                min: 0.0
                max: 10.0
                fixedLength: 3
                defaultValue: localStorage.get('pcsdk-image-gamma') || max
                onValueChanged: localStorage.set('pcsdk-image-gamma', value)
                label: 'Gamma'
            }
        }
    }
}
