import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

ItemDelegate {
    id: control
    property int time : 0
    property var name : ""
    property var pic: ""


    RowLayout {
        id: layout
        anchors.fill: parent
        spacing: 10

        Text {
            //rightPadding: control.spacing
            id: pitName
            text: control.name
            font: control.font
            color: "white" // control.enabled ? (control.down ? "#17a81a" : "#21be2b") : "#bdbebf"
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.minimumWidth: 200
        }

        Text {
            property int hours: control.time/(60*60)
            property int minutes: (control.time-(hours*60*60))/(60)
            property int seconds: (control.time-(minutes*60)-(hours*60*60))
            //rightPadding: control.spacing
            id: pitTime
            text: hours + " : " + minutes + " : " + seconds
            font: control.font
            color: "white" // control.enabled ? (control.down ? "#17a81a" : "#21be2b") : "#bdbebf"
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            Layout.minimumWidth: 100
            onMinutesChanged:
            {
                if (hours < 1 && minutes < 1)
                {
                    console.debug(minutes)
                    if (!redAnim.running)
                        redAnim.start();
                }
            }
        }



        Image {
            id: pitImg
            Layout.maximumWidth: 40
            Layout.maximumHeight: 40
            source: "/Data/"+control.pic+".png"
        }

        Image
        {
            id: killBtn
            Layout.maximumWidth: 40
            Layout.maximumHeight: 40
            source: "images/icons/update.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    pitsModel.updateTimer(control.name)
                }
            }
        }
        Image
        {
            id: deleteBtn
            Layout.maximumWidth: 40
            Layout.maximumHeight: 40
            source: "images/icons/delete.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    pitsModel.removeTarget(control.name)
                }
            }
        }
    }

    background: Rectangle {
        id: itemBackground
        implicitWidth: 100
        implicitHeight: 50
        opacity: enabled ? 1 : 0.3
        color: "#303030"
        MouseArea {
            enabled: !redAnim.running
            anchors.fill: parent
            hoverEnabled: true
            onEntered: itemBackground.color = "#606060"
            onExited: itemBackground.color = "#303030"
        }
        PropertyAnimation {
            id: redAnim
            target: itemBackground
            property: "color"
            to: "red"
            duration: 60000

        }
    }

}
