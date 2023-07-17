#ifndef APPLICATIONTYPE_H
#define APPLICATIONTYPE_H

#include <QObject>

using namespace std;

class ApplicationType : public QObject {
    Q_OBJECT
    Q_ENUMS(Type)

public:
    explicit ApplicationType(QObject *parent = 0);

    enum Type {
        SYSTEM=0, SAILFISH, ANDROID
    };

    /**
     * @brief plain name of application
     * @return
     */
    QString getName() const;
    void setName(const QString &v);

    /**
     * @brief type of application
     * @return
     */
    Type getType() const;
    void setType(Type v);

private:
    QString name;
    ApplicationType::Type type;

};

#endif // APPLICATIONTYPE_H
