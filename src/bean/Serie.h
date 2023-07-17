#ifndef SERIE_H
#define SERIE_H

#include <QObject>
#include <QString>
#include <QColor>
#include <QList>

using namespace std;

class Serie : public QObject {

    Q_OBJECT
    Q_PROPERTY(QString nom READ getNom  NOTIFY valeurChanged)
    Q_PROPERTY(quint64 valeur READ getValeur NOTIFY valeurChanged )
    Q_PROPERTY(QString couleur READ getCouleur NOTIFY valeurChanged )
    Q_PROPERTY(QString identifiant READ getIdentifiant NOTIFY valeurChanged )
    Q_PROPERTY(QString info READ getInfo NOTIFY valeurChanged )

private:
    QString identifiant;
    QString nom;
    quint64 valeur;
    QString couleur;
    QString info;

public :
    static QColor rouge;
    static QColor bleu;
    static QColor vert;
    static QColor jaune;
    static QColor gris;
    static QColor violet;

    explicit Serie(QObject *parent = 0);

    void setNom(const QString &v ) {nom = v;}
    QString getNom() const {return nom;}

    /**
     * @brief en kb
     */
    void setValeur(const quint64 v) {valeur=v;}
    quint64 getValeur() const {return valeur;}

    void setCouleur(const QString &v) {couleur = v;}
    QString getCouleur() const { return couleur;}

    void setIdentifiant(const QString & v) {identifiant=v;}
    QString getIdentifiant() const {return identifiant;}

    void setInfo(const QString & v) {info=v;}
    QString getInfo() const {return info;}


signals:
    void valeurChanged();
};
typedef QList<Serie *> * SerieListe;

#endif // SERIE_H
