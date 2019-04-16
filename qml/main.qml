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

    //工具函数
    Util {
        id: util;
    }

    //音乐API
    MusicApp {
        id: musicApp;
    }

    //播放列表
    PlayList {
        id: playList
        mediaPlayer: mediaPlayer
        musicApp: musicApp
        util: util
    }

    //顶栏
    TopBar {
        id: topBar
        anchors.left: parent.left
        anchors.right: parent.right

        musicApp: musicApp
        suggestion: suggestion
    }

    //底部栏
    BottomBar {
        id: bottomBar
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width
        color: "#444444"

        mediaPlayer: mediaPlayer
        playList: playList
        musicApp: musicApp
        onLyricHiddenChanged: {
            lyricView.visible = !lyricHidden
        }
    }

    //边栏
    SideBar {
        id: sideBar
        anchors.left: parent.left
        anchors.top: topBar.bottom
        anchors.bottom: bottomBar.top

        playList: playList
        container: container
    }

    //搜索建议
    Suggestion {
        id: suggestion
        anchors.left: sideBar.right
        anchors.leftMargin: 28
        anchors.top: topBar.bottom
        anchors.topMargin: -15
        z: 100

        musicApp: musicApp
        playList: playList
    }

    //歌词
    Lyric {
        id: lyricView

        mediaPlayer: mediaPlayer
        playList: playList
        musicApp: musicApp

        z: 2
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: sideBar.bottom
        anchors.bottom: bottomBar.top
        visible: false
    }

    //内容区域
    Container {
        id: container

        lyric: lyricView

        anchors.left: sideBar.right
        anchors.top: topBar.bottom
        anchors.bottom: bottomBar.top
        anchors.right: parent.right

        musicApp: musicApp
        playList: playList
    }

    MouseArea {
        anchors.fill: parent
        z: -1
        onClicked: {
            suggestion.hide()
        }
    }

    //关闭时
    Component.onDestruction: {
        //保存播放列表
        playList.saveTo("playList")
    }

    //加载结束
    Component.onCompleted: {
        //加载播放列表
        playList.loadFrom("playList")
        sideBar.update()

        //列表中第一首为默认歌曲
        if (playList.count() > 0)
        {
            playList.index = 0
        }
    }
}
