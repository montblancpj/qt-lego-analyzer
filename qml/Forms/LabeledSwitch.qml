import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

ColumnLayout {
    property string label: ""
    property real   switchWidth: 100
    property alias  on: toggleSwitch.checked

    Text {
        text: label
        font.pixelSize: 14
    }

    Item {
        height: 1 // margin
    }

    Switch {
        id: toggleSwitch
        width: switchWidth
    }
}

