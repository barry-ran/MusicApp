import QtQuick 2.4
import QtQuick.Window 2.4
import QtMultimedia 5.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import "./resource"
import MusicAppCore 1.0

Window {
    id: root
    visible: true
    width: 1000
    height: 600
    title: qsTr("MusicApp")
    color: "#333333"

    MediaPlayer {
        id: mediaPlayer;
    }

    Util {
        id: util;
    }

    MusicApp {
        id: musicApp;
    }

    //顶栏
    TopBar {
        id: topBar
        anchors.left: parent.left
        anchors.right: parent.right
    }
    //底部栏
    BottomBar {
        id: bottomBar
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width
        color: "#444444"
    }

    //边栏
    SideBar {
        id: sideBar
        anchors.left: parent.left
        anchors.top: topBar.bottom
        anchors.bottom: bottomBar.top
    }
    //搜索建议
    Suggestion {
        id: suggestion
        anchors.left: sideBar.right
        anchors.leftMargin: 28
        anchors.top: topBar.bottom
        anchors.topMargin: -15
        z: 100
    }
}
