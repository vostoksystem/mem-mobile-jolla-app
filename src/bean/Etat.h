#ifndef ETAT_H
#define ETAT_H

#include <QObject>
#include <QString>

using namespace std;

class Etat : public QObject {

    Q_OBJECT
    Q_PROPERTY(QString source READ getSource CONSTANT)
    Q_PROPERTY(QString mount READ getMount CONSTANT)
    Q_PROPERTY(qint64 actuel READ getActuel CONSTANT)
    Q_PROPERTY(qint64 total READ getTotal CONSTANT)
    Q_PROPERTY(float pourcentage READ getPourcentage CONSTANT)
    Q_PROPERTY(bool valide READ isValide CONSTANT)

private:
    QString source;
    QString mount;
    qint64 actuel;
    qint64 total;

public :
    explicit Etat(QObject *parent = 0);

    /**
     * @brief getSource
     * @return
     */
    QString getSource() const { return source;}
    void setSource(QString const &s ) {source=s;}

    /**
     * @brief getMount
     * @return
     */
    QString getMount() const { return mount;}
    void setMount(QString const &s ) {mount=s;}


    /**
     * @brief taille actuelle du media en kb
     * @return
     */
    qint64 getActuel() const {return actuel;}
    void setActuel(qint64 const s ) {actuel=s;}


    /**
     * @brief taille totale du media en kb
     * @return
     */
    qint64 getTotal() const {return total;}
    void setTotal(qint64 const s ) {total=s;}

    /**
     * @brief helper, evite le test valide, div / 0
     * @return
     */
    float getPourcentage() const {return total != 0 ? (float)actuel / (float)total : 0.0f;}

    /**
     * @brief met l'etat à jour
     */
    void setVal(const qint64 a, const qint64 t);


    /**
     * @brief vrai si l'etat est valide (total != 0)
     * @return
     */
    bool isValide() const {return total != 0;}
};

#endif
