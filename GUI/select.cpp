#include "Select.h"

Select::Select()
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

void Select::createFormGroupBox()
{
    selectBox = new QComboBox(this); // drop down menu, index wiht 0, 1, 2..
    selectBox->addItem(tr("Protein interaction dissociation constant"));



    formGroupBox = new QGroupBox(tr("Welcome"));
    QFormLayout *layout = new QFormLayout;

    layout->addRow(new QLabel(tr("Select a choice:")), selectBox);
    formGroupBox->setLayout(layout);
}
