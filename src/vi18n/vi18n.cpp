#include "vi18n.h"
#include <QRegularExpression>
#include <QFile>
#include <QDir>
#include <sailfishapp.h>
#include <QCoreApplication>

#include <QDebug>

#define DEF_LNG QLocale::English

/**
 * @brief return singleton instance
 * @return vI18n instance
 */
vI18n& vI18n::getInstance() {
    static vI18n instance;
    return instance;
}

/**
 * @brief IMPORTANT ! you must register this objet into context to use it into qml; this method do it for you.
 * Service will be available on the name "vi18n" (else define in VI18N_QML_NAME).
 * You can also register service by yourself (or not, if you don't need translation in qml)
 * @param view
 * @param name
 */
void vI18n::init(QQmlContext *context,QString name) {
#ifdef QT_DEBUG
    qDebug() << "vi18n, registering qml service as : " << name;
#endif
    context->setContextProperty(name, &vI18n::getInstance());
}

/**
 * @brief private contructor
 * @param parent
 */
vI18n::vI18n(QObject *parent) : QObject(parent) {

    i18n_path=SailfishApp::pathTo(I18N_DIR).toString(QUrl::PreferLocalFile);

#ifdef QT_DEBUG
    qDebug() << "vi18n, using config directory : " << i18n_path;
#endif

    data_d =loadTranslation(DEF_LNG);
    // let's avoid null pointer
    data_d = data_d == NULL ? new QHash<QString,QString>() : data_d;
    data.insert(DEF_LNG,data_d );

    if(setLanguage(QLocale::system().bcp47Name()) ==false) {
        current=DEF_LNG;
        // again avoiding null pointer if user locale not defined
        data_c =  data_d ;
    }
}

/**
  * @brief Change current language. Default to user's locale. use a bcp47 (2 letters code)
  * Note call to this function is NOT guarantee to be successful
  * @param bcp47 language to use (i.e en, fr, de, fi ...)
  * @return true if able to swith to new language
 */
bool vI18n::setLanguage(QString bcp47)  {
    QLocale::Language lng= QLocale(bcp47).language();

    if(data.contains(lng)==false) {
        Vi18nItem d=loadTranslation(lng);
        if(d==NULL) {
            return false;
        }
        data.insert(lng,d);
    }

    current = lng;
    data_c = data.value(lng);
    emit languageChanged(bcp47);
    return true;
}

/**
 * @brief return value for key. If not found, try default, e.i. english, still not, return key.
 * From qml use vi18n.get("your key")
 * @param key
 * @return translated key, or just 'key'
 */
QString const vI18n::get(QString const &key) {
    return getInstance().getImp(key);
}

/**
 * @brief vI18n::getImp
 * @param key
 * @return
 */
QString const vI18n::getImp(QString const &key) {
    if(data_c->contains(key)) {
        return data_c->value(key);
    }

    // ok, try with default...
    if(data_d->contains(key)) {
        // add it to current lng for further use
        data_c->insert(key, data_d->value(key));

        return data_d->value(key);
    }

    // not your lucky day...
    return key;
}

/**
 * @brief return value for key in language bcp47.  If not found, try default, e.i. english.
 * From qml, in german, use vi18n.get("your key", 42 ), check QLocal::Language for list of code
 * @param key
 * @param bcp47
 * @return
 */
QString const vI18n::get(QString const &key, QString const &bcp47)  {
    QLocale::Language lng= QLocale(bcp47).language();
    return getInstance().getImp(key,lng);
}

/**
 * @brief vI18n::get
 * @param key
 * @param lng
 * @return
 */
QString const vI18n::getImp(QString const &key, QLocale::Language lng)  {
    if(data.contains(lng)) {
        Vi18nItem d=data.value(lng);
        if(d->contains(key) ) {
            return d->value(key);
        }
    } else {
        // lng not found, try to load it
        Vi18nItem dl= loadTranslation(lng);

        if( dl!=NULL) {
            data.insert(lng,dl);
        } else {
            // can't found, use default (avoiding triy to load each time)
            data.insert(lng,data_d);
        }

        return getImp(key,lng);
    }

    // ok, try with default...
    if(data_d->contains(key)) {
        return data_d->value(key);
    }

    return key;
}

/**
 * @brief vI18n::loadTranslation
 * @param lng
 * @return
 */
Vi18nItem vI18n::loadTranslation(QLocale::Language lng) {
    static QRegularExpression keyvalreg("^\\s*([^#][^=\\s]+)\\s*=\\s*(.+)\\s*$");

/*
    QFile f(i18n_path + "/" +QCoreApplication::applicationName() +
            ( lng == QLocale::English ? "" : "_" + QLocale(lng).bcp47Name() )+
            ".properties");
*/
    QFile f(i18n_path + "/vi18n" +
            ( lng == QLocale::English ? "" : "-" + QLocale(lng).bcp47Name() )+
            ".properties");


    if(!f.open(QIODevice::ReadOnly| QIODevice::Text)) {
#ifdef QT_DEBUG
        qDebug() << "vi18n, cannot open : " << f.fileName();
#endif
        return NULL;
    }

    Vi18nItem d = new QHash<QString,QString>();

    QTextStream in(&f);
    in.setCodec("UTF-8");
    QString key;
    QString val;
    bool wait=false;
    while(!in.atEnd()) {
        if(wait) {
            // on multiline key
            val = val + in.readLine().trimmed();
            if(val.at(val.length()-1) == '\\') {
                // still
                continue;
            }

            wait=false;
            d->insert( key, val.replace("\\n", "\n").replace('\\', ""));
            continue;
        }

        QRegularExpressionMatch m= keyvalreg.match(in.readLine());
        if(m.hasMatch()) {
            key=m.captured(1);
            val=m.captured(2);

            if(val.at(val.length()-1) == '\\') {
                // found multiline key
                wait=true;
                continue;
            }

            d->insert( key, val.replace("\\n", "\n"));
        }
    }

    f.close();

#ifdef QT_DEBUG
    qDebug() << "vi18n, loaded " << QLocale::languageToString(lng) << "read : " << d->size() <<" value(s)";
    QList<QString> kk= d->keys();
    foreach (QString k, kk) {
        qDebug() << "key : " << k << " values : " << d->value(k);
    }
#endif

    return d;
}
