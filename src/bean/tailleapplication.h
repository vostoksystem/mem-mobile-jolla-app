#ifndef TAILLEAPPLICATION_H
#define TAILLEAPPLICATION_H

#include <QObject>

class TailleApplication : public QObject {

    Q_OBJECT
    Q_PROPERTY(quint64 application READ getApplication CONSTANT)
    Q_PROPERTY(quint64 config READ getConfig CONSTANT)
    Q_PROPERTY(quint64 donnee READ getDonnee CONSTANT)
    Q_PROPERTY(quint64 cache READ getCache CONSTANT)

private :
    quint64 application;
    quint64 config;
    quint64 donnee;
    quint64 cache;

public:
    explicit TailleApplication(QObject *parent = 0);

    void setApplication(const quint64 v) {application=v;}
    quint64 getApplication() {return application;}

    void setConfig(const quint64 v) {config=v;}
    quint64 getConfig() {return config;}

    void setDonnee(const quint64 v) {donnee=v;}
    quint64 getDonnee() {return donnee;}

    void setCache(const quint64 v) {cache=v;}
    quint64 getCache() {return cache;}

};

#endif // TAILLEAPPLICATION_H
