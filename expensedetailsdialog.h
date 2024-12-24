#ifndef EXPENSEDETAILSDIALOG_H
#define EXPENSEDETAILSDIALOG_H

#include <QDialog>

namespace Ui {
class ExpenseDetailsDialog;
}

class ExpenseDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExpenseDetailsDialog(QWidget *parent = nullptr);
    ~ExpenseDetailsDialog();
    void setExpenseDetails(const QString &description, double amount, const QString &category, const QString &date);

private:
    Ui::ExpenseDetailsDialog *ui;
};

#endif // EXPENSEDETAILSDIALOG_H
