
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtWidgets>
QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog();
    QSpinBox *rowValueSpinBox;
     QSpinBox *yvalueSpinBox;
     QSpinBox *numSetsSpinBox;
private:

    void createFormGroupBox();



    QGroupBox *formGroupBox;

    QDialogButtonBox *buttonBox;


    QAction *exitAction;
};


#endif // DIALOG_H
