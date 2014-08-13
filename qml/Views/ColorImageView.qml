import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import MontBlanc 1.0
import '../Shapes'
import '../Common'
import '../Forms'

ColumnLayout {
    id: viewContainer
    property string dummyImageFilePath: '/Users/hecomi/Pictures/zzz.png'

    RowLayout {
        id: images
        clip: true
        spacing: 0
        anchors.fill: parent

        GroupBox {
            Layout.minimumHeight: parent.height
            Layout.minimumWidth: parent.width

            clip: true

            OpenNIImage {
                id: colorImage;
                isColor: true
                anchors.fill: parent

                Fps {
                    id: homographyImageFpsCounter
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.margins: 5
                }

                Timer {
                    property int fps: 30
                    interval: 1000 / fps
                    running: true
                    repeat: false
                    onTriggered: {
                        if ( !parent.isOpen() ) {
                            parent.filePath = dummyImageFilePath;
                        }
                        parent.update();
                        running = true;
                    }
                }

                onImageChanged: homographyImageFpsCounter.update()
            }
        }
    }
}
