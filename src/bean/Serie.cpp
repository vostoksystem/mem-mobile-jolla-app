#include "Serie.h"

QColor Serie::rouge = QColor(250,85,85,200);
QColor Serie::bleu= QColor(0,180,240,200);
QColor Serie::vert= QColor(177,190,50,200);
QColor Serie::jaune= QColor(255,139,54,200);
QColor Serie::gris= QColor(236,236,236,200);
QColor Serie::violet= QColor(88,153,183,200);

Serie::Serie(QObject *parent) : QObject(parent),
    identifiant(""),nom(""),valeur(0),couleur("#ff88a0"),info("") {
}
