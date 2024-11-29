#include "window.h"
#include "ui_window.h"
#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QStackedLayout>




Window::Window(QWidget *parent)
    : QWidget(parent)
{
    // id
    QLabel *idLabel = new QLabel(tr("Identifiant:"));
    idLineEdit = new QLineEdit;
    idLineEdit->setPlaceholderText("Entrer votre identifiant");

    // mdp
    QLabel *passwordLabel = new QLabel(tr("Mot de passe:"));
    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setPlaceholderText("Entrer votre mot de passe");

    // case afficher/masquer
    showIDCheckBox = new QCheckBox(tr("Afficher l'identifiant"));
    connect(showIDCheckBox, &QCheckBox::toggled, this, &Window::toggleIDField);

    // case afficher/masquer
    showPasswordCheckBox = new QCheckBox(tr("Afficher le mot de passe"));
    connect(showPasswordCheckBox, &QCheckBox::toggled, this, &Window::togglePasswordField);

    // bouton de connexion
    QPushButton *connectButton = new QPushButton(tr("Se connecter"));
    connect(connectButton, &QPushButton::clicked, this, &Window::onConnectButtonClicked);


    // Layout principal
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(idLabel, 0, 0);
    layout->addWidget(idLineEdit, 0, 1);
    layout->addWidget(showIDCheckBox, 0, 2);
    layout->addWidget(passwordLabel, 1, 0);
    layout->addWidget(passwordLineEdit, 1, 1);
    layout->addWidget(showPasswordCheckBox, 1, 2);
    layout->addWidget(connectButton, 2, 1);

    setLayout(layout);
    setWindowTitle(tr("AMIR BANQUE"));
}

void Window::toggleIDField(bool checked)
{
    idLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::PasswordEchoOnEdit);
}

void Window::togglePasswordField(bool checked)
{
    passwordLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}


void Window::onConnectButtonClicked()
{
    QString id = idLineEdit->text();
    QString password = passwordLineEdit->text();

    // Créer une requête pour vérifier les identifiants dans la base de données
    QSqlQuery query;
    query.prepare("SELECT * FROM clients WHERE id = :id AND password = :password");
    query.bindValue(":id", id);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        // Les identifiants sont corrects, ouvrir la nouvelle fenêtre
        QWidget *newWindow = new QWidget;
        newWindow->setWindowTitle("MENU DU COMPTE");
        newWindow->resize(400, 300);

        soldeLabel = new QLabel;
        soldeLabel->setAlignment(Qt::AlignCenter);
        soldeLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
        updateSolde(); // Initialiser l'affichage du solde
        QVBoxLayout *layout = new QVBoxLayout; layout->addWidget(soldeLabel); // Ajouter le label en haut de la fenêtre
        // Créer un layout pour la nouvelle fenêtre


        QPushButton *CrediterButton = new QPushButton(tr("Crediter"));
        QPushButton *DebiterButton = new QPushButton(tr("Debiter"));
        QPushButton *TransfertButton = new QPushButton(tr("Transfert"));
        QPushButton *InfosButton = new QPushButton(tr("Afficher Infos"));
        QPushButton *quitButton = new QPushButton(tr("Deconnexion"));

        // Ajouter les boutons au layout vertical
        layout->addWidget(CrediterButton);
        layout->addWidget(DebiterButton);
        layout->addWidget(TransfertButton);
        layout->addWidget(InfosButton);
        layout->addWidget(quitButton);

        // Connexions des boutons
        connect(quitButton, &QPushButton::clicked, newWindow, &QWidget::close);
        connect(CrediterButton, &QPushButton::clicked, this, &Window::openCrediterPage);
        connect(DebiterButton, &QPushButton::clicked, this, &Window::openDebiterPage);
        connect(TransfertButton, &QPushButton::clicked, this, &Window::openTransfertPage);
        connect(InfosButton, &QPushButton::clicked, this, &Window::openInfosPage);

        // Appliquer le layout à la nouvelle fenêtre
        newWindow->setLayout(layout);
        newWindow->show();

    } else {
        QMessageBox::warning(this, tr("Erreur de connexion"), tr("Identifiants ou mot de passe incorrects."));
    }
}



void Window::openCrediterPage()
{
    QWidget *creditWindow = new QWidget;
    creditWindow->setWindowTitle("CREDITER");
    creditWindow->resize(400, 300);

    // Champ pour saisir le montant à créditer
    QLabel *creditLabel = new QLabel(tr("Montant à créditer:"));
    creditAmountLineEdit = new QLineEdit;
    creditAmountLineEdit->setPlaceholderText("Entrer le montant");

    // Bouton pour confirmer le crédit
    confirmCreditButton = new QPushButton(tr("Confirmer le crédit"));
    connect(confirmCreditButton, &QPushButton::clicked, this, &Window::creditAmount);

    // Layout pour la fenêtre de crédit
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(creditLabel);
    layout->addWidget(creditAmountLineEdit);
    layout->addWidget(confirmCreditButton);

    creditWindow->setLayout(layout);
    creditWindow->show();
}
void Window::creditAmount()
{
    bool ok;
    double amount = creditAmountLineEdit->text().toDouble(&ok);

    if (!ok || amount <= 0) {
        updateSolde();
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez entrer un montant valide."));
        return;
    }

    QString id = idLineEdit->text();

    QSqlQuery query;
    query.prepare("UPDATE clients SET solde = solde + :amount WHERE id = :id");
    query.bindValue(":amount", amount);
    query.bindValue(":id", id);

    if (query.exec()) {
        QMessageBox::information(this, tr("Succès"), tr("Le montant a été crédité avec succès."));
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Échec de la mise à jour du solde."));
    }
}


void Window::openDebiterPage()
{
    QWidget *debitWindow = new QWidget;
    debitWindow->setWindowTitle("DEBITER");
    debitWindow->resize(400, 300);

    // Champ pour saisir le montant à débiter
    QLabel *debitLabel = new QLabel(tr("Montant à débiter:"));
    debitAmountLineEdit = new QLineEdit;
    debitAmountLineEdit->setPlaceholderText("Entrer le montant");

    // Bouton pour confirmer le débit
    confirmDebitButton = new QPushButton(tr("Confirmer le débit"));
    connect(confirmDebitButton, &QPushButton::clicked, this, &Window::debitAmount);

    // Layout pour la fenêtre de débit
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(debitLabel);
    layout->addWidget(debitAmountLineEdit);
    layout->addWidget(confirmDebitButton);

    debitWindow->setLayout(layout);
    debitWindow->show();
}
void Window::debitAmount()
{
    bool ok;
    double amount = debitAmountLineEdit->text().toDouble(&ok);

    if (!ok || amount <= 0) {
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez entrer un montant valide."));
        return;
    }

    QString id = idLineEdit->text();

    QSqlQuery query;
    query.prepare("SELECT solde FROM clients WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        updateSolde();
        double solde = query.value(0).toDouble();

        if (solde < amount) {
            QMessageBox::warning(this, tr("Erreur"), tr("Solde insuffisant."));
            return;
        }

        query.prepare("UPDATE clients SET solde = solde - :amount WHERE id = :id");
        query.bindValue(":amount", amount);
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(this, tr("Succès"), tr("Le montant a été débité avec succès."));
        } else {
            QMessageBox::critical(this, tr("Erreur"), tr("Échec de la mise à jour du solde."));
        }
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Échec de la récupération du solde."));
    }
}


void Window::openTransfertPage()
{
    QWidget *transfertWindow = new QWidget;
    transfertWindow->setWindowTitle("TRANSFERT");
    transfertWindow->resize(400, 300);

    // Champ pour saisir l'ID du destinataire
    QLabel *destinationLabel = new QLabel(tr("Identifiant du destinataire:"));
    transferDestinationLineEdit = new QLineEdit;
    transferDestinationLineEdit->setPlaceholderText("Entrer l'ID du destinataire");

    // Champ pour saisir le montant à transférer
    QLabel *amountLabel = new QLabel(tr("Montant à transférer:"));
    transferAmountLineEdit = new QLineEdit;
    transferAmountLineEdit->setPlaceholderText("Entrer le montant");

    // Bouton pour confirmer le transfert
    confirmTransferButton = new QPushButton(tr("Confirmer le transfert"));
    connect(confirmTransferButton, &QPushButton::clicked, this, &Window::transferAmount);

    // Layout pour la fenêtre de transfert
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(destinationLabel);
    layout->addWidget(transferDestinationLineEdit);
    layout->addWidget(amountLabel);
    layout->addWidget(transferAmountLineEdit);
    layout->addWidget(confirmTransferButton);

    transfertWindow->setLayout(layout);
    transfertWindow->show();
}
void Window::transferAmount()
{
    QString destinationID = transferDestinationLineEdit->text();
    bool ok;
    double amount = transferAmountLineEdit->text().toDouble(&ok);

    if (!ok || amount <= 0) {
        updateSolde();
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez entrer un montant valide."));
        return;
    }

    QString sourceID = idLineEdit->text();

    // Vérifier si le solde est suffisant
    QSqlQuery query;
    query.prepare("SELECT solde FROM clients WHERE id = :id");
    query.bindValue(":id", sourceID);

    if (query.exec() && query.next()) {
        updateSolde();
        double solde = query.value(0).toDouble();

        if (solde < amount) {
            QMessageBox::warning(this, tr("Erreur"), tr("Solde insuffisant."));
            return;
        }

        // Débiter le montant du compte source
        query.prepare("UPDATE clients SET solde = solde - :amount WHERE id = :id");
        query.bindValue(":amount", amount);
        query.bindValue(":id", sourceID);

        if (!query.exec()) {
            QMessageBox::critical(this, tr("Erreur"), tr("Échec du débit."));
            return;
        }

        // Créditer le montant au compte destinataire
        query.prepare("UPDATE clients SET solde = solde + :amount WHERE id = :destinationID");
        query.bindValue(":amount", amount);
        query.bindValue(":destinationID", destinationID);

        if (query.exec()) {
            QMessageBox::information(this, tr("Succès"), tr("Le transfert a été effectué avec succès."));
        } else {
            // Annuler le débit en cas d'échec du crédit
            query.prepare("UPDATE clients SET solde = solde + :amount WHERE id = :id");
            query.bindValue(":amount", amount);
            query.bindValue(":id", sourceID);
            query.exec();

            QMessageBox::critical(this, tr("Erreur"), tr("Échec du transfert."));
        }
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Échec de la récupération du solde."));
    }
}


void Window::openInfosPage()
{
    QWidget *infosWindow = new QWidget;
    infosWindow->setWindowTitle("INFORMATIONS");
    infosWindow->resize(400, 300);

    QString id = idLineEdit->text();

    // Créer une requête pour récupérer les informations du client
    QSqlQuery query;
    query.prepare("SELECT nom, prenom, telephone, solde FROM clients WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        QString nom = query.value(0).toString();
        QString prenom = query.value(1).toString();
        QString telephone = query.value(2).toString();
        double solde = query.value(3).toDouble();

        QLabel *infosLabel = new QLabel(tr("Nom: %1\nPrénom: %2\nTéléphone: %3\nSolde: %4")
                                        .arg(nom)
                                        .arg(prenom)
                                        .arg(telephone)
                                        .arg(solde));

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(infosLabel);
        infosWindow->setLayout(layout);
    } else {
        QLabel *errorLabel = new QLabel(tr("Erreur : Impossible de récupérer les informations du client."));
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(errorLabel);
        infosWindow->setLayout(layout);
    }

    infosWindow->show();
}
void Window::updateSolde()
{
    QString id = idLineEdit->text();

    QSqlQuery query;
    query.prepare("SELECT solde FROM clients WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        double solde = query.value(0).toDouble();
        soldeLabel->setText(tr("Solde : %1 €").arg(solde));
    } else {
        soldeLabel->setText(tr("Erreur lors de la récupération du solde"));
    }
}

