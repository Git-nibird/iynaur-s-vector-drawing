#ifndef CODEEDITDLG_H
#define CODEEDITDLG_H

//#include <QDialog>
#include<QTextEdit>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include "dialog_global.h"

class DIALOG_EXPORT CodeEditDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CodeEditDlg(QWidget *parent = 0);
    ~CodeEditDlg();
    QTextEdit* textEdit;

private:
    //Ui::CodeEditDlg *ui;
    QDialogButtonBox *buttonBox;
};

#endif // CODEEDITDLG_H
