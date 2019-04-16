#include "musicApp.h"
#include <QDebug>
#include <QUrl>
#include <QNetworkRequest>
#include <QByteArray>
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

const int PAGESIZE = 20;

//音乐搜索API
//参数%1: 搜索关键字
//参数%2: 起始位置，为(page - 1) * 20
const QString ApiOfSearch = "http://music.baidu.com/search?key=%1&start=%2&size=20&s=1";

//搜索建议API
//参数%1: 歌曲id
const QString ApiOfSuggestion = "http://sug.music.baidu.com/info/suggestion?format=json&word=%1&version=2&from=0";

//歌曲信息API
//参数%1: 歌曲id
const QString ApiOfSongInfo = "http://play.baidu.com/data/music/songinfo?songIds=%1";

//歌曲链接API
//参数%1: 歌曲id
const QString ApiOfSongLink = "http://play.baidu.com/data/music/songlink?songIds=%1&type=m4a,mp3";

MusicApp::MusicApp(QObject *parent) : QObject(parent)
{
    m_searchReply       =  nullptr;
    m_suggestionReply   =  nullptr;
    m_songInfoReply     =  nullptr;
    m_songLinkReply     =  nullptr;
    m_lyricReply        =  nullptr;
    m_manager.setCookieJar(&m_cookieJar);
}

MusicApp::~MusicApp()
{

}

void MusicApp::search(const QString &keyWord, int page)
{
    if (m_searchReply)
    {
        m_searchReply->deleteLater();
    }

    int start = (page - 1) * PAGESIZE;

    QUrl url = QUrl(ApiOfSearch.arg(keyWord).arg(start));
    m_searchReply = m_manager.get(QNetworkRequest(url));
    connect(m_searchReply, SIGNAL(finished()), this, SLOT(searchReplyFinished()));
}

void MusicApp::getSuggestion(QString songId)
{
    if (m_suggestionReply)
    {
        m_suggestionReply->deleteLater();
    }

    QUrl url = QUrl(ApiOfSuggestion.arg(songId));
    m_suggestionReply = m_manager.get(QNetworkRequest(url));
    connect(m_suggestionReply, SIGNAL(finished()), this, SLOT(suggestionReplyFinished()));
}

void MusicApp::getSongInfo(QString songId)
{
    if (m_songInfoReply)
    {
        m_songInfoReply->deleteLater();
    }

    QUrl url = QUrl(ApiOfSongInfo.arg(songId));
    m_songInfoReply = m_manager.get(QNetworkRequest(url));
    connect(m_songInfoReply, SIGNAL(finished()), this, SLOT(songInfoReplyFinished()));
}

void MusicApp::getSongLink(QString songId)
{
    if (m_songLinkReply)
    {
        m_songLinkReply->deleteLater();
    }

    QUrl url = QUrl(ApiOfSongLink.arg(songId));
    m_songLinkReply = m_manager.get(QNetworkRequest(url));
    connect(m_songLinkReply, SIGNAL(finished()), this, SLOT(songLinkReplyFinished()));
}

void MusicApp::getLyric(QString lyricUrl)
{
    if (m_lyricReply)
    {
        m_lyricReply->deleteLater();
    }

    m_lyricReply = m_manager.get(QNetworkRequest(lyricUrl));
    connect(m_lyricReply, SIGNAL(finished()), this, SLOT(lyricReplyFinished()));
}

QString MusicApp::unifyResult(QString r)
{
    return r.replace(QRegularExpression("songid   |  songId"),      "sid")
            .replace(QRegularExpression("author   |  artistname"),  "singer")
            .replace(QRegularExpression("songname |  songName"),    "sname");
}

void MusicApp::searchReplyFinished()
{
    QString url = m_searchReply->request().url().toString();

    int     keyWordBegin  =  url.indexOf("key=")      + 4;
    int     keyWordEnd    =  url.indexOf("&start=");
    int     pageBeginPos  =  url.indexOf("start=")    + 6;
    int     pageEndPos    =  url.indexOf("&szie=");

    int     currentPage   = url.mid(pageBeginPos, pageEndPos - pageBeginPos).toInt() / PAGESIZE + 1;
    QString keyWord       = url.mid(keyWordBegin, keyWordEnd - keyWordBegin);

    if (m_searchReply->error())
    {
        emit searchComplete(currentPage, 1, keyWord, "{error: " + m_searchReply->errorString() + "}");
        return;
    }

    QString html = m_searchReply->readAll();
    QStringList songList;
    QRegularExpression re("<li data-songitem = '(.+?)'");
    QRegularExpressionMatchIterator i = re.globalMatch(html);

    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString songData = match.captured(1);
        //&quot; 替换为 " ;删除<em>和</em>
        songData = songData.replace("&quot;", "\"").replace("&lt;em&gt;", "").replace("&lt;\\/em&gt", "");
        songList << songData;
    }

    //构造json数组
    QString songArray = "[" + songList.join(",") + "]";
    QString result = unifyResult(songArray);

    //匹配总页数
    QRegularExpression pageCountRe("\">(\\d+)</a>\\$*<a class=\"page-navigator-next\"");
    QRegularExpressionMatch match = pageCountRe.match(html);

    //页面总数
    int pageCount = match.captured(1).toInt();
    //如果没有 pageCount，则 pageCount 设为 1;
    pageCount = pageCount > 0 ? pageCount : 1;

    emit searchComplete(currentPage,pageCount,keyWord,result);

}

void MusicApp::suggestionReplyFinished()
{
    if (m_suggestionReply->error()){
        emit getSuggestionComplete("{error:" + m_suggestionReply->errorString() + "}");
        return;
    }

    QString sug = m_suggestionReply->readAll();
    emit getSuggestionComplete(unifyResult(sug));
}

void MusicApp::songInfoReplyFinished()
{
    if (m_songInfoReply->error()){
        emit getSongInfoComplete("{error:" + m_songInfoReply->errorString() + "}");
        return;
    }

    QString songinfo = m_songInfoReply->readAll();
    emit getSongInfoComplete(songinfo);
}

void MusicApp::songLinkReplyFinished()
{
    if (m_songLinkReply->error()){
        emit getSongLinkComplete("{error:" + m_songLinkReply->errorString() + "}");
        return;
    }

    QString songlink = m_songLinkReply->readAll();
    emit getSongLinkComplete(unifyResult(songlink));
}

void MusicApp::lyricReplyFinished()
{
    QString url = m_lyricReply->url().toString();

    if (m_lyricReply->error()){
        emit getLyricComplete(url, "");
        return;
    }

    qDebug() << m_lyricReply->rawHeaderList();
    emit getLyricComplete(url, m_lyricReply->readAll());
}
