#include "select.h"

Select::Select()
{
    resize(350, 250);
    createFormGroupBox();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(formGroupBox);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(tr("Welcome"));
}

void Select::createFormGroupBox()
{
    selectBox = new QComboBox(this); // drop down menu, index wiht 0, 1, 2..
    selectBox->addItem(tr("Protein interaction dissociation constant"));
    selectBox->addItem(tr("Enzyme Kinetics"));

    label = new QLabel(this);
    QPixmap pixmap(":/images/graph.png");
    label->setPixmap(pixmap);

    formGroupBox = new QGroupBox(tr("Kinetics Parameter Determinations"));
    QFormLayout *layout = new QFormLayout;

    layout->addRow(new QLabel(tr("Select:")), selectBox);
    layout->addWidget(label);
    formGroupBox->setLayout(layout);

}
