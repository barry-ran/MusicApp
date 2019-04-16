#include "cookieJar.h"

CookieJar::CookieJar()
{

}

CookieJar::~CookieJar()
{

}

QList<QNetworkCookie> CookieJar::getCookies() const
{
    return allCookies();
}

void CookieJar::setCookies(const QList<QNetworkCookie> &cookieList)
{
    setAllCookies(cookieList);
}
