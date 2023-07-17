#ifndef MEMOIRE_H
#define MEMOIRE_H

#include <QObject>
#include <QQmlContext>
#include "../bean/Etat.h"

using namespace std;

#define MEMOIRE_QML_NAME "MemoireService"


class memoireService: public QObject {
    Q_OBJECT

public :
    static memoireService& getInstance();
    static void init(QQmlContext *context, QString name=MEMOIRE_QML_NAME);

    Q_INVOKABLE  Etat* utilisation();

    /**
      * @brief memoire::etat
      * @param v
      * @deprecated ; on garde l'algo ; ça peu servir plus tard
      */
    Etat* utilisationOld();

private :
    explicit memoireService(QObject *parent = 0);

};

#endif // MEMOIRE_H

