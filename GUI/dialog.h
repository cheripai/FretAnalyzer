
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtWidgets>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog();
    QSpinBox *rowValueSpinBox;
    QSpinBox *yvalueSpinBox;
    QSpinBox *numSetsSpinBox;
    QComboBox *unitBox;
private:
    void createFormGroupBox();
    QGroupBox *formGroupBox;
    QDialogButtonBox *buttonBox;
    QAction *exitAction;

};

#endif // DIALOG_H
