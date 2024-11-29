#include "window.h"
#include "bdd.h"
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QSqlError>
#include <QDebug>
#include <QStackedLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window w;
    Gestion gestionBDD;
    gestionBDD.connexionBDD();

    w.show();
    return a.exec();
}
