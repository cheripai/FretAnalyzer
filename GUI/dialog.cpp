

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
    setWindowTitle(tr("qFRET Assay Setup"));
}

void Dialog::createFormGroupBox()
{
    unitBox = new QComboBox(this); // drop down menu, index wiht 0, 1, 2..
    unitBox->addItem(tr("μM"));
    unitBox->addItem(tr("nM"));
    unitBox->addItem(tr("pM"));


    rowValueSpinBox = new QSpinBox(this);
    yvalueSpinBox = new QSpinBox(this);
    numSetsSpinBox =  new QSpinBox(this);
    formGroupBox = new QGroupBox(tr("Data Input Format"));
    QFormLayout *layout = new QFormLayout;

    layout->addRow(new QLabel(tr("Donor and acceptor concentration:")), unitBox);
    layout->addRow(new QLabel(tr("How many concentrations of donor:")), numSetsSpinBox);
    layout->addRow(new QLabel(tr("How many concentrations of acceptor:")), rowValueSpinBox);
    layout->addRow(new QLabel(tr("Enter acceptor replicates:")), yvalueSpinBox);
    formGroupBox->setLayout(layout);
}
