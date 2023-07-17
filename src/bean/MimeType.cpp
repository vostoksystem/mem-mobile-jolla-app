#include "MimeType.h"
#include "Serie.h"
#include <QDebug>
#include "../vi18n/vi18n.h"

QRegularExpression MimeType::blancmultipleEx(" +");
QRegularExpression MimeType::expressionEx("[^a-z0-9 \\$\\-\\._]");

/**
 * @brief MimeType::MimeType
 * @param parent
 */
MimeType::MimeType(QObject *parent) : QObject(parent)
{
    data.actif=true;
    data.extensionPossible=true;
    data.extension="";
    data.couleur= Serie::jaune;
}

/**
 * @brief MimeType::setNom
 * @param v
 */
void MimeType::setNom(const QString &v ){
    nom=v;
}

/**
 * @brief MimeType::getNom
 * @return
 */
QString MimeType::getNom() const {
    return nom;
}

/**
 * @brief MimeType::getNomI18n
 * @return
 */
QString MimeType::getNomI18n() const {
    QString id("media.");
    id.append(nom);
    return vI18n::getInstance().get(id.toUtf8().constData());
}

/**
 * @brief MimeType::getLegende
 * @return
 */
QString MimeType::getLegende() const {
    return legende;
}

/**
 * @brief MimeType::setExtension
 * @param v
 */
void MimeType::setExtension(const QString &v ){
    if(data.extension.compare(v, Qt::CaseInsensitive)==0) {
        return;
    }

    data.extension=v.toLower().replace(expressionEx, " ").replace(blancmultipleEx, " ").trimmed();

    if(data.extension.length()==0) {
        data.actif=false;
        emit actifChanged();
    }

    emit donneeChanged(nom);
    emit extensionChanged();
}

/**
 * @brief MimeType::getExtension
 * @return
 */
QString MimeType::getExtension() const {
    return data.extension;
}

/**
 * @brief MimeType::setActif
 * @param v
 */
void MimeType::setActif(const bool v ){

    if( data.actif ==v) {
        return;
    }

    data.actif =v;
    emit donneeChanged(nom);
    emit actifChanged();
}

/**
 * @brief MimeType::getActif
 * @return
 */
bool MimeType::getActif() const {
    return data.actif;
}

/**
 * @brief MimeType::setExtensionPossible
 * @param v
 */
void MimeType::setExtensionPossible(const bool v ){
    if( data.extensionPossible ==v) {
        return;
    }

    data.extensionPossible =v;
    emit donneeChanged(nom);
    emit extensionPossibleChanged();
}

/**
 * @brief MimeType::getExtensionPossible
 * @return
 */
bool MimeType::getExtensionPossible() const {
    return data.extensionPossible;
}

/**
 * @brief MimeType::getCouleur
 * @return
 */
QColor MimeType::getCouleur() const {
    return data.couleur;
}

/**
 * @brief setCouleur
 */
void MimeType::setCouleur(const QColor &v) {
    if(data.couleur==v) {
        return;
    }

    data.couleur = v;
    emit donneeChanged(nom);
    emit couleurChanged();
}

/**
 * @brief MimeType::setAll
 */
void  MimeType::setAll(MimeTypeData *v) {

    if( data.actif !=v->actif) {
        data.actif =v->actif;
        emit actifChanged();
    }

    if(data.couleur!=v->couleur) {
        data.couleur=v->couleur;
        emit couleurChanged();
    }

    if(data.extension.compare(v->extension,Qt::CaseInsensitive)!=0) {
        data.extension=v->extension;
        emit extensionChanged();
    }

    if(data.extensionPossible!=v->extensionPossible) {
        data.extensionPossible=v->extensionPossible;
        emit extensionPossibleChanged();
    }

}


