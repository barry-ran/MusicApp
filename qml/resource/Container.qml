import QtQuick 2.4
import MusicAppCore 1.0

//内容区域
Rectangle {
    id: containerRoot
    property MusicApp musicApp
    property PlayList playList
    property Lyric lyric

    //播放列表
    SearchResult {
        id: searchResult
        anchors.fill: parent
        visible: false
        musicApp: parent.musicApp
        playList: parent.playList
    }

    PlayListView {
        id: playListView
        anchors.fill: parent
        visible: true
        playList: parent.playList
    }

    function showSearchResult()
    {
        searchResult.visible = true
        playListView.visible = false
        lyric.visible        = false
    }

    function updatePlayList(listName)
    {
        playListView.update(listName)
    }

    Connections {
        target: musicApp
        onSearchComplete: {
            try {
                var songList = JSON.parse(songList)
                if (songList.error)
                {
                    //TODO:搜索错误
                }
            } catch (e) {
                console.log(e)
                return
            }
            searchResult.setResultInfo(currentPage, pageCounr, keyword, songList)
            showSearchResult()
        }
    }
}
