#ifndef MimeType_H
#define MimeType_H

#include <QObject>
#include <QDataStream>
#include <QColor>
#include <QRegularExpression>
#include <qqml.h>

using namespace std;

struct MimeTypeData
{
    QString extension;
    bool actif;
    bool extensionPossible;
    QColor couleur;

    MimeTypeData(){}

    MimeTypeData(QString e,bool a,bool ep,QColor c): extension(e), actif(a), extensionPossible(ep), couleur(c){}

    friend QDataStream& operator<<(QDataStream &out, const MimeTypeData &obj)
    {
        out  << obj.extension << obj.actif << obj.extensionPossible << obj.couleur;
        return out;
    }

    friend  QDataStream& operator>>(QDataStream &in, MimeTypeData &obj)
    {
        in >>  obj.extension >>  obj.actif >> obj.extensionPossible >> obj.couleur;
        return in;
    }
};
Q_DECLARE_METATYPE(MimeTypeData)

class MimeType : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool actif READ getActif WRITE setActif NOTIFY actifChanged )
    Q_PROPERTY(bool extensionPossible READ getExtensionPossible WRITE setExtensionPossible NOTIFY extensionPossibleChanged )
    Q_PROPERTY(QString extension READ  getExtension WRITE setExtension NOTIFY extensionChanged )
    Q_PROPERTY(QString nom READ getNom  NOTIFY nomChanged )
    Q_PROPERTY(QString legende READ getLegende NOTIFY legendeChanged )
    Q_PROPERTY(QColor couleur READ getCouleur WRITE setCouleur NOTIFY couleurChanged )
    Q_PROPERTY(QString i18n READ getNomI18n NOTIFY nomChanged)

private:
    QString nom;
    QString legende;
    MimeTypeData data;

    static QRegularExpression blancmultipleEx;
    static QRegularExpression expressionEx;


public:
    explicit MimeType(QObject *parent = 0);

    QString getNom() const;
    QString getNomI18n() const;

    QString getLegende() const;
    QString getExtension() const;
    bool getActif() const;
    bool getExtensionPossible() const;
    QColor getCouleur() const;

    // utilisé par qml et cie, fait un emit *Changed
    void setNom(const QString & );
    void setExtension(const QString & );
    void setActif(const bool);
    void setExtensionPossible(const bool);
    void setCouleur(const QColor &);

    // utilisé par config pour initialiser le bean
    void setAll(MimeTypeData *);

signals:
    void donneeChanged(const QString &); // emit par chaque setter -sauf setdata- -> mise à jour du (et sauvegarde par le) service config

    void nomChanged();
    void legendeChanged();

    void extensionChanged();
    void actifChanged();
    void extensionPossibleChanged();
    void couleurChanged();
};
typedef QList<MimeType *> * MimeTypeListe;

#endif // MimeType_H

// http://developer.nokia.com/community/wiki/Saving_custom_structures_and_classes_to_QSettings
