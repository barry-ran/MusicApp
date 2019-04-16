import QtQuick 2.4

Rectangle {
    width: parent.width
    height: 40
    color: "#444"
    //左上角
    Rectangle {
        id: brand
        width: 200
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left
        color: "#333"
        Text {
            id: brandText
            text: qsTr("MusicApp")
            font.pixelSize: 28
            color: "#eee"
            anchors.centerIn: parent
        }
    }
    //搜索框
    Rectangle {
        width: 300
        height: 28
        radius: 14
        anchors.left: brand.right
        anchors.leftMargin: 15
        anchors.verticalCenter: parent.verticalCenter
        //输入框
        TextInput {
            id: input
            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.right: searchButton.left
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 15
            clip: true
            //Enter响应
            onAccepted: {
                search()
            }
        }
        //搜索按钮
        Rectangle {
            id: searchButton
            height: 20
            width: 20
            anchors.right: parent.right
            anchors.rightMargin: 14
            anchors.verticalCenter: parent.verticalCenter
            Image {
                id: searchIcon
                anchors.fill: parent
                source: "qrc:/image/search.png"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: search()
            }
        }
    }
    //边条
    Rectangle {
        width: parent.width
        height: 2
        color: "#E61E16"
        anchors.bottom: parent.bottom
    }
    function search() {
        if (input.text == "")
        {
            return
        }
    }
}
