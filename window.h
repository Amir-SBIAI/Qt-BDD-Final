#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QLabel>
#include <QStackedLayout>

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QCheckBox;
class QPushButton;
QT_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);

private slots:
    void toggleIDField(bool checked);
    void togglePasswordField(bool checked);
    void onConnectButtonClicked();
    void openCrediterPage();
    void openDebiterPage();
    void openTransfertPage();
    void openInfosPage();


    void creditAmount();
    void debitAmount();
    void transferAmount();
    void updateSolde();




private:
    QStackedLayout *stackedLayout;
    QLabel *soldeLabel;
    QLineEdit *idLineEdit;
    QLineEdit *passwordLineEdit;
    QCheckBox *showIDCheckBox;
    QCheckBox *showPasswordCheckBox;
    QPushButton *connectButton;

    QLineEdit *creditAmountLineEdit;
    QPushButton *confirmCreditButton;
    QLineEdit *debitAmountLineEdit;
    QPushButton *confirmDebitButton;
    QLineEdit *transferDestinationLineEdit;
    QLineEdit *transferAmountLineEdit;
    QPushButton *confirmTransferButton;




};

#endif
