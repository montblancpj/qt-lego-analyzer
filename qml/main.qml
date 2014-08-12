import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import MontBlanc 1.0
import "Views"

Window {
    id: window
    property variant homographyImage: null

    visible: true
    width: 1600
    height: 960

    SystemPalette { id: syspal }
    color: syspal.window

    TabView {
        id: tabs
        frameVisible: true
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 12
        clip: true

        Tab {
            id: homographyTab
            title: "Homography"
            anchors.margins: 4

            HomographyView {
                id: homographyView
                anchors.fill: parent
            }
        }

        Tab {
            id: calibrationTab
            title: "Calibration"
            anchors.margins: 4

            CalibrationView {
                id: calibrationView
                anchors.fill: parent
            }
        }
    }
}
