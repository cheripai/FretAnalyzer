#include "mainwindow.h"
#include <QApplication>
#include "dialog.h"
#include "select.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Dialog dialog;
    Select selectdialog;
    selectdialog.show();

    int nRep = 0, nSet = 0, nRow = 0;

    // first, shows the select diaglog ->
    // second, shows the table setting dialog->
    // last, shows the table grid
    if(selectdialog.exec() == QDialog::Accepted && selectdialog.selectBox->currentIndex()==0)
    {
        dialog.show();
        if(dialog.exec() == QDialog::Accepted)
        {
            nRow = dialog.rowValueSpinBox->value();
            nRep = dialog.yvalueSpinBox->value();
            nSet = dialog.numSetsSpinBox->value();
            QString units = dialog.unitBox->currentText();

            MainWindow w(nRow,nSet,nRep,units);
            w.show();
            return app.exec();
        }
    }
    else
    {
        return 0;
    }
}
