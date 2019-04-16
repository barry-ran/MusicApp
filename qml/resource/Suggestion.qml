import QtQuick 2.4
import MusicAppCore 1.0
import "utils.js" as Func

//搜索建议弹出框
Rectangle {
    color: "white"
    visible: false
    width: 240
    height: 200

    property PlayList playList
    property MusicApp musicApp

    ListModel {
        id: suggestionModel
    }

    Component {
        id: suggestionDelegate
        Item {
            id: wrapper
            width: 200
            height: 20
            Rectangle {
                Text {
                    text: sname + "-" + singer
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                     wrapper.ListView.view.currentIndex = index
                    hide()
                    var song = suggestionModel.get(index)
                    playList.addSong(Func.cloneObject(song))
                    var last = playList.count() - 1
                    playList.setIndex(last)
                }
            }
        }
    }

    ListView {
        id: suggestionView
        height: parent.height
        width: parent.width
        model: suggestionModel
        delegate: suggestionDelegate
        clip: true
    }

    function show()
    {
        visible = true
    }

    function hide()
    {
        visible = false
    }

    function setDisplaySongs(songs)
    {
        suggestionModel.clear()
        for (var i in songs)
        {
            //转换字符串
            songs[i].sid = "" + songs[i].sid
            suggestionModel.append(songs[i])
        }
    }

    Connections {
        target: musicApp
        onGetSuggestionComplete: {
            try {
                var sug   = JSON.parse(suggestion)
                var data  = sug.data
                var songs = data.song
                setDisplaySongs(songs)
                show()
            } catch (e) {
                console.log("Suggestion[onGetSuggestionComplete]: " + e)
            }
        }
    }
}
