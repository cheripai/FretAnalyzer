#ifndef SELECT_H
#define SELECT_H

#include <QDialog>
#include <QtWidgets>

class Select: public QDialog
{
    Q_OBJECT
public:
    Select();
    QComboBox *selectBox;
    QLabel *label;
private:
    void createFormGroupBox();
    QGroupBox *formGroupBox;
    QDialogButtonBox *buttonBox;
    QAction *exitAction;

};

#endif // SELECT_H
