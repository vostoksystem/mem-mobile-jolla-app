#include "Etat.h"
#include <qdebug.h>

/**
 * @brief Etat::Etat
 * @param parent
 */
Etat::Etat(QObject *parent) : QObject(parent),actuel(0),total(0) {
}

/**
 * @brief miseAJour
 */
void Etat::setVal(const qint64 a, const qint64 t) {
    actuel = a;
    total = t;

    if(actuel <0) {
        actuel = 0;
    }
    if(total <0) {
        total = 0;
    }

    if(actuel> total) {
        actuel = total;
    }
}
