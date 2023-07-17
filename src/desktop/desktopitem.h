#ifndef DESKTOPITEM_H
#define DESKTOPITEM_H

#include <QObject>
#include <qqml.h>

class DesktopItem : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString id READ getId() CONSTANT )
    Q_PROPERTY(QString name READ getName() CONSTANT )
    Q_PROPERTY(QString icon READ getIcon CONSTANT )
    Q_PROPERTY(QString exec READ getExec CONSTANT )
    Q_PROPERTY(QString type READ getType CONSTANT )


private:
    QString id;
    QString name;
    QString exec;
    QString icon;
    QString type; // "ANDROID" / "SAILFISH" / "SYSTEM"

public:
    explicit DesktopItem(QObject *parent = 0);

    /**
     * @brief id, e.i. process name
     * @return
     */
    QString getId() const {return id;}
    void setId(const QString &v) {id=v;}

    /**
     * @brief plain name of application
     * @return
     */
    QString getName() const {return name;}
    void setName(const QString &v) {name=v;}

    /**
     * @brief path to icon
     * @return full path or empty if none, never null
     */
    QString getIcon() const {return icon;}
    void setIcon(const QString &v) {icon=v;}

    /**
     * @brief full name of command
     * @return
     */
    QString getExec() const {return exec;}
    void setExec(const QString &v) {exec=v;}

    /**
     * @brief "ANDROID" / "SAILFISH" / "SYSTEM"
     * @return
     */
    QString getType() const {return type;}
    void setType(const QString &v) {type=v;}


};
typedef QList<DesktopItem *> * DesktopItemList;

#endif
