#include "expensedetailsdialog.h"
#include "ui_expensedetailsdialog.h"

ExpenseDetailsDialog::ExpenseDetailsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExpenseDetailsDialog)
{
    ui->setupUi(this);
}

ExpenseDetailsDialog::~ExpenseDetailsDialog()
{
    delete ui;
}

void ExpenseDetailsDialog::setExpenseDetails(const QString &description, double amount, const QString &category, const QString &date)
{
    ui->descriptionLabel->setText("Description: " + description);
    ui->amountLabel->setText("Amount: " + QString::number(amount));
    ui->categoryLabel->setText("Category: " + category);
    ui->dateLabel->setText("Date: " + date);
}
