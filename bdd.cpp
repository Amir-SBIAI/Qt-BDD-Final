#include "bdd.h"
#include <QMessageBox>
#include <QDebug> // Pour le débogage
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

bool Gestion::connexionBDD() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("banque_amir");
    db.setUserName("QTTP");
    db.setPassword("ciel");

    if (!db.open()) {
        // Affiche un message d'erreur si la connexion échoue
        QMessageBox::critical(nullptr, "Erreur de connexion", "Impossible de se connecter à la base de données.\n" + db.lastError().text());
        return false;
    }

    // Si la connexion est réussie, affiche un message de débogage
    qDebug() << "Connexion réussie à la base de données.";

    // Connexion réussie
    return true;
}
