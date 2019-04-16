#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QString>

class Util : public QObject
{
    Q_OBJECT
public:
    explicit Util(QObject *parent = nullptr);
    ~Util();

    Q_INVOKABLE QString readFile(QString fileName);
    Q_INVOKABLE void saveFile(QString fileName, QString content);
};

#endif // UTIL_H
