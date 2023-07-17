#ifndef REDIRECTIONAPPLICATION_H
#define REDIRECTIONAPPLICATION_H

#include <QObject>
#include <QList>

/**
 * @brief info pour les app qui ne suivent pas le schema standard
 */
class RedirectionApplication : public QObject
{
    Q_OBJECT
public:
    explicit RedirectionApplication(QObject *parent = 0);

    QStringList getDonnee() const { return donnee; }
    void ajouterDonnee(const QString &v) {donnee.append(v);}

    QString getCache() const { return cache; }
    void setCache(const QString &v) { cache = v;}

    QList<QString> getPartage() const { return partage;}
    void ajouterPartage(const QString &v) {partage.append(v);}

private:
    QStringList donnee; // liste des rep et fichier pour les donnees
    QString cache;
    QStringList partage; // avec qui les donnée sont partagé ; id de l'appli

};

#endif // REDIRECTIONAPPLICATION_H
