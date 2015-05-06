

#include <QtWidgets>

#include "dialog.h"

Dialog::Dialog()
{
    createFormGroupBox();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(formGroupBox);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(tr("Basic Layouts"));
}





void Dialog::createFormGroupBox()
{
    rowValueSpinBox = new QSpinBox(this);
    yvalueSpinBox = new QSpinBox(this);
    numSetsSpinBox =  new QSpinBox(this);
    formGroupBox = new QGroupBox(tr("Table setting"));
    QFormLayout *layout = new QFormLayout;

    layout->addRow(new QLabel(tr("Enter x rows:")), rowValueSpinBox);
    layout->addRow(new QLabel(tr("Enter y sets:")), numSetsSpinBox);
    layout->addRow(new QLabel(tr("Enter y replicates:")), yvalueSpinBox);
    formGroupBox->setLayout(layout);
}
