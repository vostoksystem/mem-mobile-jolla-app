#include "util.h"
#include <QDebug>
#include <QRegularExpression>
#include <QDir>
#include <QTextStream>

Util::Util() {

}

/**
 * @brief open a file into a string from a full path
 * @param path
 * @return
 */
QString Util::fileToString(const QString &path) {

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }

    QByteArray data = file.readAll();
    data.replace('\0', ' ');

    QString res = QString( data.simplified() );

    //  qDebug() << "size : " << data.length() << " " << res.length() << " " << res;

    //    QTextStream stream(&file);
    //    stream.setAutoDetectUnicode(true);
    //    stream .setCodec(charset);

    //    QString res = stream.readAll();
    file.close();

    return res;
}

/**
 * @brief convenience method to get the name of a file/ app from a full path
 * added here to prevent a new dependecy to expternal project
 * @param v
 * @return
 */
QString Util::getName(const QString &v) {
    static QRegularExpression nameExp("([^\\/]+)$");

    QRegularExpressionMatch m = nameExp.match(v);
    if(m.hasMatch()) {
        return m.captured(1);
    }
    return v;
}

/**
 * @brief try to fill info with the real process from command line arguments and type of app
 * @param commandLine
 * @param ApplicationType
 * @return true if found a match
 */
bool Util::processName(const QString &commandLine, ApplicationType &info) {

    static QRegularExpression sailReg("^(?:/usr/bin/)?invoker .*--type=(?:silica-qt5|qt5|generic) (.+)$");
    static QRegExp noArgsReg("^[^-].+");
    static QRegularExpression harbourReg("(harbour-[^ /]+)");
    static QRegularExpression andReg("^((?:\\w+\\.)+\\w+)(?::\\w+)?$", QRegularExpression::CaseInsensitiveOption);

    if(commandLine.length()==0) {
        return false;
    }

    // look for harbour
    // /usr/bin/harbour-vostok_muuzik
    // invoker --type=silica-qt5 --single-instance harbour-saildbmeter

    QRegularExpressionMatch hs = harbourReg.match(commandLine);
    if(hs.hasMatch()) {
        info.setName(hs.captured(1) );
        info.setType(ApplicationType::SAILFISH);
        return true;
    }

    // look for sailfish app
    // invoker --type=silica-qt5 /usr/bin/store-client show
    // invoker --type=silica-qt5 --single-instance harbour-saildbmeter
    // /usr/bin/invoker -o --type=qt5 /usr/bin/voicecall-manager
    // /usr/bin/invoker --type=generic /usr/libexec/telepathy-ring
    // /usr/bin/invoker -s --type=generic /usr/bin/sailjail -p jolla-email.desktop /usr/bin/jolla-email -prestart
    QRegularExpressionMatch ms = sailReg.match(commandLine);
    if( ms.hasMatch()) {
        QStringList msl = ms.captured(1).split(' ');

        // on cherche un programme par full path, si ok, on prend le dernier
        QStringList prg = msl.filter("/");
        if(prg.size()>0) {
            info.setName( getName(prg.last()) );
            info.setType(ApplicationType::SAILFISH);
            return true;
        }

        // y a pas, on prend le dernier qui ne commence par par "-"
        QStringList noarg = msl.filter(noArgsReg);
        if(noarg.size()>0) {
            info.setName(noarg.last() );
            info.setType(ApplicationType::SAILFISH);
            return true;
        }
    }

    // look for android ; impossible à determiner avec certitude
    // com.here.app.maps:remote
    QRegularExpressionMatch am = andReg.match(commandLine);
    if( am.hasMatch()) {
        info.setName( am.captured(1));
        info.setType(ApplicationType::ANDROID);
        return true;
    }

    // process système

    return false;
}
