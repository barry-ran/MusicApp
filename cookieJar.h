#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QObject>
#include <QNetworkCookieJar>
#include <QNetworkCookie>

class CookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    CookieJar();
    ~CookieJar();

    QList<QNetworkCookie> getCookies() const;
    void setCookies(const QList<QNetworkCookie>& cookieList);
};

#endif // COOKIEJAR_H
