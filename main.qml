import QtQuick 2.9
import QtQuick.Controls 2.3

ApplicationWindow {
    visible: true
    width: 800
    height: 500
    title: qsTr("Питометр v0.2")



    ScrollView {
        y: topMenu.height
        width: parent.width
        height: parent.height - topMenu.height

        ListView {
            width: parent.width
            model: pitsModel
            delegate: Pit {
                name: targetName
                pic: picName
                width: parent.width
                time:  timeRemaning
            }
        }
    }

    Rectangle
    {
        id: topMenu
        width: parent.width
        height: 30
        color: "#505050"

        ComboBox{
            id: addNewPit
            x: parent.width / 2 - this.width
            width: 250
            font.pixelSize: 14
            height: parent.height
            model: allTargetsModel
        }

        ToolButton {
            id: addNewPitBtn
            x: addNewPit.x + addNewPit.width + 15
            width: 80
            height: parent.height
            icon.source: "images/icons/add.png"
            onClicked:
            {
                pitsModel.addTarget(addNewPit.currentText)
            }
        }

    }
onClosing: {
    pitsModel.saveModel()
}
}
