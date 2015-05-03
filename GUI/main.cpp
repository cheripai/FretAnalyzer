#include "mainwindow.h"
#include <QApplication>
#include "dialog.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Dialog dialog;
    dialog.show();

    int nRep=0, nSet = 0, nRow = 0;

    if(dialog.exec() == QDialog::Accepted)
    {
        nRow = dialog.rowValueSpinBox->value();
        nRep = dialog.yvalueSpinBox->value();
        nSet = dialog.numSetsSpinBox->value();

        MainWindow w(nRow,nSet,nRep);
        w.show();
        return app.exec();
    }
    else
    {
        return 0;
    }
}
