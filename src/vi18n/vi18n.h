/*
  Copyright (C) 2016 Jerome levreau / vostoksystem.
  Contact: contact <contact@vostoksytem.eu>
  All rights reserved.

  Apache 2.à Licence
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
  */

#ifndef VI18N_H
#define VI18N_H

#include <QObject>
#include <QString>
#include <QLocale>
#include <QHash>
#include <QQmlContext>
#include <QDebug>

// name of the directory - inside /usr/share - for the translation files
#define I18N_DIR "i18n"

// name of the service which is available into qml
#define VI18N_QML_NAME "vi18n"

typedef QHash<QString,QString> * Vi18nItem;

/**
 * vI18n intended to be a replacement of qt lupdate with a more simple and friendly approach similar to id based properties files in Java world.
 * code and translation are totally separated, All value are dynamically loaded, no need to compil your tranlation files.
 * 
 * Source
 * ----------------
 * iclude the all subdirectory (with translation) into your src dir
 *
 * Pro file
 * ------------------
 * add this to your main pro file :
 *
 * include(src/vi18n/vi18n.pri)
 * 
 * Distribution file
 * ------------------
 * Don't forget to include translation into your package, for yaml :
 * 
 * Files:
 *  - '%{_bindir}'
 *  - '%{_datadir}/%{name}'
 *  - '%{_datadir}/%{name}/i18n'
 * 
 * Properties files name convention
 * ------------------
 * vi18n-{2 chars code language}.properties
 * vi18n-fi.properties
 * vi18n.properties # this is the default / fall back file
 * 
 * Properties files format
 * ------------------
 * Properties files are plain text similar to java's one, but with utf-8 encoding instead. Define one key for each line.
 * If a key is the same, You don't need to redefine all keys on each language, just put it in default file (type english)
 * One difference with lupudate, you have to add keys yourself. No change either on properties or source files will automatically be done.
 *
 * list.title=Hello all
 * # this is a comment
 * 
 *  
 * check the sample project for usage and install instruction
 * 
 * @version 1.4
 * @author levreau.jerome@vostoksystem.eu
 */
class vI18n : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString language READ getLanguage WRITE setLanguage NOTIFY languageChanged )
    Q_PROPERTY(QString languageStr READ getLanguageStr NOTIFY languageChanged )

public:
    /**
     * @brief return instance of vi18n service (singleton)
     * @return
     */
    static vI18n& getInstance();

    /**
	 * @brief IMPORTANT ! you must register this objet into context to use it into qml; this method do it for you.
     * Service will be available on the name "vi18n" (else define in VI18N_QML_NAME).
     * You can also register service by yourself (or not, if you don't need translation in qml)
     * @param context
     * @param name ; name for service, default to VI18N_QML_NAME
     */
    static void init(QQmlContext *context, QString name = VI18N_QML_NAME);

    /**
     * @brief Change current language. Default to user's locale. use a bcp47 (2 letters code)
     * Note call to this function is NOT guarantee to be successful
     * @param bcp47 language to use (i.e en, fr, de, fi ...)
     * @return true if able to swith to new language
     */
    Q_INVOKABLE bool setLanguage(QString bcp47);


    /**
     * @brief return bcp47 code (2 letters name) for current language
     * @return
     */
    Q_INVOKABLE QString const getLanguage() const {
        return QLocale(current).bcp47Name();
    }

    /**
     * @brief return plain name of current language
     * @return
     */
    Q_INVOKABLE QString const getLanguageStr() const {
        return QLocale(current).nativeLanguageName();
    }

    /**
     * @brief return value for key. If not found, try default, e.i. english, still not, return key.
     * From qml use vi18n.get("your key")
     * @param key
     * @return translated key, or just 'key'
     */
    Q_INVOKABLE static QString const get(QString const &key );

    /**
     * @brief return value for key in language bcp47.  If not found, try default, e.i. english.
     * From qml, in german, use vi18n.get("your key", 42 ), check QLocal::Language for list of code
     * @param key
     * @param bcp47 2 char language code, e.i en, fr, de...
     * @return
     */
    Q_INVOKABLE static QString const get(QString const &key, QString const &bcp47) ;

private :
    explicit vI18n(QObject *parent = 0);

    Vi18nItem loadTranslation(QLocale::Language lng);
    QString const getImp(QString const &key );
    QString const getImp(QString const &key, QLocale::Language lng) ;

    QString i18n_path;

    QLocale::Language current; // current locale

    QHash<QLocale::Language,Vi18nItem> data; // language 'tree'
    Vi18nItem data_c; //current language set
    Vi18nItem data_d; // default language set

signals:
    /**
     * @brief emit when current language change
     * @param lng new language too use.
     */
    void languageChanged(QString bcp47);

public slots:
};

#endif // VI18N_H
