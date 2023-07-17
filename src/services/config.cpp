#include "config.h"
#include <qdebug.h>
#include "../bean/Serie.h"

config::config(QObject *parent) : QObject(parent)
{

    // init les valeurs par defaut
    listeMimeDef["video"]=new MimeTypeData(
                QString("avi mpg mpeg mkv mov wmv flv mp4"), true, true, Serie::rouge);
    listeMimeDef["musique"]=new MimeTypeData(
                QString("mp3 ogg m4a flac"), true, true, Serie::jaune);
    listeMimeDef["image"]=new MimeTypeData(
                QString("jpg jpeg png gif"), true, true, Serie::violet);
    listeMimeDef["doc"]=new MimeTypeData(
                QString("pdf doc xls docx xlsx ppt pptx ods odt odp"), true, true, Serie::bleu);
    listeMimeDef["autre"]=new MimeTypeData(
                QString(""), true,false, Serie::gris);
    listeMimeDef["libre"]=new MimeTypeData(
                QString(""), true, false, Serie::vert);

    QList<QString> nom;
    nom.append("video");
    nom.append("musique");
    nom.append("image");
    nom.append("doc");
    nom.append("autre");
    nom.append("libre");

    // charge depuis config et passe les valeur aux bean, def si aucune...
    QList<QString>::iterator i;
    for (i = nom.begin(); i != nom.end(); ++i) {
        QVariant qv;
        qv.setValue(*listeMimeDef[*i]);
        MimeTypeData d = settings.value(MEDIA + *i, qv).value<MimeTypeData>();

        MimeType *tmp = new MimeType();
        tmp->setNom(*i);
        tmp->setAll(&d);
        listeMime.append(tmp);

        // connecter le signal <-> slot pour le modif de valeur
        connect(tmp, SIGNAL(donneeChanged(const QString &)), this, SLOT(changementMime(const QString &)), Qt::QueuedConnection);
    }

    processusSysteme=settings.value(PROCESSUS_TOUS, PROCESSUS_TOUS_DEF).toBool();
    tailleMinProcessus=settings.value(PROCESSUS_TAILLE, PROCESSUS_TAILLE_DEF).toLongLong();
    androidScan=settings.value(ANDROID_SCAN, ANDROID_SCAN_DEF).toBool();
    disclaimer=settings.value(DISCLAIMER, false).toBool();

    // hack pour corriger pb de config initiale
    if(tailleMinProcessus>50 ) {
        tailleMinProcessus=50;
    }

    changementConfig();
    emit statChanged();
    emit processusChanged();
}


/**
 * @brief return singleton instance
 * @return config instance
 */
config& config::getInstance() {
    static config instance;
    return instance;
}

/**
 * @brief config::init
 * @param context
 */
void config::init(QQmlContext *context) {
    context->setContextProperty(CONFIGSERVICE_QML_NAME, &config::getInstance());
}


/**
 * @brief config::setProcessusSysteme
 * @param v
 */
void config::setProcessusSysteme(const bool v) {
    if(processusSysteme == v) {
        return;
    }
    processusSysteme=v;
    settings.setValue(PROCESSUS_TOUS, v);
    settings.sync();
    changementConfig();
    emit processusChanged();
}


/**
 * @brief config::setTailleMinProcessus
 * @param v
 */
void config::setTailleMinProcessus(qint64 v) {
    if(tailleMinProcessus==v) {
        return;
    }
    tailleMinProcessus = v;
    settings.setValue(PROCESSUS_TAILLE, v);
    settings.sync();
    changementConfig();
    emit processusChanged();
}

/**
 * @brief config::changementMime
 * changement dans les valeur de v initié par le bean -> faire une sauvegarde
 * pas la peine de faire un listeMimeChanged,  déja fait par le bean pour sa prop
 * @param v
 */
void  config::changementMime(const QString & v) {

    QList<MimeType *>::iterator i;
    for (i = listeMime.begin(); i != listeMime.end(); ++i)   {
        if( (*i)->getNom().compare(v) == 0 ) {

            MimeTypeData d(
                        (*i)->getExtension(),
                        (*i)->getActif(),
                        (*i)->getExtensionPossible(),
                        (*i)->getCouleur()
                        );
            QVariant qv;
            qv.setValue(d);

            settings.setValue(MEDIA + (*i)->getNom(), qv);
            settings.sync();

            emit statChanged();

            changementConfig();
            return;
        }
    }
}

/**
 * @brief getMime
 * @param type
 * @return
 */
MimeType *config::getMime(QString type) {

    QList<MimeType *>::iterator i;
    for (i = listeMime.begin(); i != listeMime.end(); ++i)   {
        if( (*i)->getNom().compare(type) == 0 ) {
            return *i;
        }
    }

    return NULL;
}

/**
 * @brief config::setAndroidScan
 * @param v
 */
void config::setAndroidScan(const bool v) {
    if(androidScan ==v) {
        return;
    }
   androidScan =v;
    settings.setValue(ANDROID_SCAN, v);
    settings.sync();
    changementConfig();
    emit androidChanged();
}

/**
 * @brief config::setAndroidScan
 * @param v
 */
void config::setDisclaimer(const bool v) {
    if(disclaimer ==v) {
        return;
    }
   disclaimer =v;
    settings.setValue(DISCLAIMER, v);
    settings.sync();
    emit disclaimerChanged();
}

/**
 * @brief config::parDefaut
 */
void config::parDefaut() {
    setProcessusSysteme(PROCESSUS_TOUS_DEF);
    setTailleMinProcessus(PROCESSUS_TAILLE_DEF);
    setAndroidScan(ANDROID_SCAN_DEF);

    QList<MimeType *>::iterator i;
    for (i = listeMime.begin(); i != listeMime.end(); ++i)   {
        (*i)->setAll(listeMimeDef[(*i)->getNom()]);
        changementMime((*i)->getNom());
    }

    emit valeurParDefautChanged();
}

/**
 * @brief config
 */
void config::changementConfig() {
    modifMime = QDateTime::currentDateTime();
}

/**
 * @brief config::dateDerniereModif
 * @return
 */
QDateTime config::dateModifMime() const {
    return modifMime;
}
