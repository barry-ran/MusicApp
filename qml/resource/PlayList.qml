import QtQuick 2.4
import QtMultimedia 5.4
import MusicAppCore 1.0

Item {
    property int index: -1
    property var playLists: {
        默认列表: []
    }//播放列表，默认加载【默认列表】
    property string currentList: "默认列表"
    property MediaPlayer mediaPlayer
    property MusicApp musicApp
    property Util util
    property string currentSid

    //列表中的歌曲数目
    function count(list)
    {
        var listname = list ? list : currentList
        console.log("listname: " + listname)
        if (typeof playLists[listname] == 'undefined')
        {
            playLists[listname] = []
        }
        return playLists[listname].length
    }

    function getSong(i, list)
    {
        var listname = list ? list : currentList
        return playLists[listname][i]
    }

    //返回指定列表的歌曲
    function getSongList(list)
    {
        var listname = list ? list : currentList
        return playLists[listname]
    }

    //当前播放位置
    function currentIndex()
    {
        return index
    }

    //添加歌曲列表
    function addSong(song, list)
    {
        var listname = list ? list : currentList
        if (typeof playLists[listname] == 'undefined')
        {
            playLists[listname] = []
        }
        playLists[listname].push(song)
    }

    //插入到指定位置
    function insertSong(pos, song, list)
    {
        playLists[list].splice(pos, 0, song)
    }

    //替换列表中的歌曲
    function replace(pos, song, list)
    {
        playLists[list].splice(pos, 1, song)
    }

    //播放指定位置歌曲
    function setIndex(i)
    {
        console.log("setIndex: " + i + "   length: " + playLists[currentList].length)
        if (i < 0 || i > (playLists[currentList].length - 1))
        {
            return
        }

        mediaPlayer.pause()
        index = i

        //如果是本地音乐，则直接播放
        if (playLists[currentList][index].localpath)
        {
            mediaPlayer.source = playLists[currentList][index].localpath
            mediaPlayer.play()
            return
        }
        var curSid = playLists[currentList][index].sid
        currentSid = curSid
        //如果不是本地音乐，则获取歌曲链接
        musicApp.getSongLink(curSid)
        musicApp.getSongInfo(curSid)
    }

    function next()
    {
        setIndex(index + 1)
    }

    function previous()
    {
        setIndex(index - 1)
    }

    function play()
    {
        if (mediaPlayer.source == "")
        {
            setIndex(index)
            return
        }
        mediaPlayer.play()
    }

    function pause()
    {
        mediaPlayer.pause()
    }

    //从文件加载列表
    function loadFrom(fileName)
    {
        try {
            var savedList = JSON.parse(util.readFile(fileName))
            for (var i in savedList)
            {
                playLists[i] = savedList[i]
            }
            index = 0
        } catch (e) {
            console.log("PlayList[loadFrom]: " + e)
        }
    }

    //保存文件
    function saveTo(fileName)
    {
        util.saveFile(fileName, JSON.stringify(playList.playLists))
    }

    Connections {
        target: musicApp
        onGetSongLinkComplete: {
            try {
                var link = JSON.parse(songLink)
                //如果还是当前播放歌曲，则立即播放，否则不处理
                if (playLists[currentList][index].sid == link.data.songList[0].sid)
                {
                    var mp3Link = link.data.songList[0].songLink
                    mediaPlayer.source = mp3Link
                    mediaPlayer.play()
                }
            } catch (e) {
                console.log("getLink: " + e)
            }
        }
    }

    Connections {
        target: mediaPlayer
        onStopped: {
            if (mediaPlayer.status == MediaPlayer.EndOfMedia)
            {
                next()
            }
        }
    }
}
