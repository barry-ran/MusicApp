#ifndef MUSICAPP_H
#define MUSICAPP_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "cookieJar.h"

class MusicApp : public QObject
{
    Q_OBJECT
public:
    explicit MusicApp(QObject *parent = nullptr);
    ~MusicApp();



    /**
     *  @brief search    搜索歌曲
     *  @param keyWord   关键字
     *  @param page      页数
     */
    Q_INVOKABLE void search(const QString& keyWord, int page);

    /**
     *  @brief getSuggestion   获取搜索建议
     *  @param songId         音乐歌曲id
     */
    Q_INVOKABLE void getSuggestion(QString songId);

    /**
     *  @brief getSongInfo    获取歌曲信息
     *  @param songId         音乐歌曲id
     */
    Q_INVOKABLE void getSongInfo(QString songId);

    /**
     *  @brief getSongLink    获取歌曲链接信息
     *  @param songId         音乐歌曲id
     */
    Q_INVOKABLE void getSongLink(QString songId);

    /**
     *  @brief getLyric       获取歌曲歌词信息
     *  @param lyricUrl            歌词链接地址
     */
    Q_INVOKABLE void getLyric(QString lyricUrl);


private:
    QNetworkAccessManager  m_manager;
    QNetworkReply*         m_searchReply;
    QNetworkReply*         m_suggestionReply;
    QNetworkReply*         m_songInfoReply;
    QNetworkReply*         m_songLinkReply;
    QNetworkReply*         m_lyricReply;

    //保存cookie
    CookieJar m_cookieJar;

    //统一结果，如songId转换为sId, songName转换为sName
    QString unifyResult(QString r);

private slots:
    void searchReplyFinished();
    void suggestionReplyFinished();
    void songInfoReplyFinished();
    void songLinkReplyFinished();
    void lyricReplyFinished();

signals:
    /**
     * @brief searchComplete   搜索完毕
     * @param currentPage      当前页
     * @param pageCount        总页数
     * @param keyWord          关键字
     * @param songList         歌曲列表, json数据
     * [
     *     {
     *         "songItem": {
     *                          "sid": 877578,
     *                          "author": "Beyond",
     *                          "sname": "海阔天空",
     *                          "oid": 877578,
     *                          "pay_type": "2"
     *                     }
     *     },
     *     {
     *         "songItem":
     *                 ...
     *     },
     *     ...
     * ]
     */
    void searchComplete(int currentPage, int pageCount, QString keyWord, QString songList);

    /**
        * @brief getSuggestionComplete 获取搜索建议完毕
        * @param suggestion 搜索建议json数据
        * {
        *    "data": {
        *           "song": [{
        *               "songid": "877578",
        *               "songname": "\u6d77\u9614\u5929\u7a7a",
        *               "encrypted_songid": "",
        *               "has_mv": "1",
        *               "yyr_artist": "0",
        *               "artistname": "Beyond"
        *           },
        *           ...
        *          ],
        *           "artist": [{
        *               "artistid": "2345733",
        *               "artistname": "\u6d77\u9614\u5929\u7a7a",
        *               "artistpic": "http:\/\/a.hiphotos.baidu.com\/ting\/pic\/item\/6d81800a19d8bc3eb42695cc808ba61ea8d3458d.jpg",
        *               "yyr_artist": "0"
        *           },
        *           ...
        *          ],
        *           "album": [{
        *               "albumid": "197864",
        *               "albumname": "\u6d77\u9614\u5929\u7a7a",
        *               "artistname": "Beyond",
        *               "artistpic": "http:\/\/a.hiphotos.baidu.com\/ting\/pic\/item\/6c224f4a20a4462314dd8c409a22720e0cf3d7f8.jpg"
        *           },
        *           ...
        *           ]
        *       },
        *       "Pro": ["artist", "song", "album"]
        *   }
        *
        */
    void getSuggestionComplete(QString suggestion);

    /**
     * @brief getSongInfoComplete     获取歌曲信息完毕
     * @param songInfo                歌曲信息
     */
    void getSongInfoComplete(QString songInfo);

    /**
     * @brief getSongLinkComplete     获取歌曲链接完毕
     * @param songLink                歌曲链接信息
     */
    void getSongLinkComplete(QString songLink);

    /**
     * @brief getLyricComplete        获取歌词信息完毕
     * @param lyricUrl                歌词链接地址
     * @param lyricContent            歌词内容
     */
    void getLyricComplete(QString lyricUrl, QString lyricContent);

};

#endif // MUSICAPP_H
