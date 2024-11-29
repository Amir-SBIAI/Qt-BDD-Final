#ifndef BDD_H
#define BDD_H

#include <QObject>
#include <QtSql/QSqlDatabase>

class Gestion : public QObject {
    Q_OBJECT  // Ceci est nécessaire si vous utilisez des fonctionnalités de Qt comme les signaux et slots

public:
    bool connexionBDD();
};

#endif // BDD_H
