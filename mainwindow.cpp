#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QDate>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Ensure selection behavior and mode are set
    ui->expensesTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->expensesTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    qDebug() << "Selection Behavior:" << ui->
                expensesTableWidget->selectionBehavior();
                    qDebug() << "Selection Mode:" << ui->expensesTableWidget->selectionMode();

                    // Connect buttons to their respective slots
                    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddExpense);
                    connect(ui->editButton, &QPushButton::clicked, this, &MainWindow::onEditExpense);
                    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteExpense);
                    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::onClearForm);

                    // Connect the table widget selection to a slot to handle row selection changes
                    connect(ui->expensesTableWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onRowSelectionChanged);

                    // Initialize database and load expenses
                    initializeDatabase();
                    loadExpenses();

                    // Populate category combo box
                    populateCategoryComboBox();
                }

                MainWindow::~MainWindow()
                {
                    delete ui;
                }

                void MainWindow::initializeDatabase()
                {
                    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                    db.setDatabaseName("expenses.db");
                    if (!db.open()) {
                        qDebug() << "Error: Unable to connect to database!";
                        return;
                    }
                    QSqlQuery query;
                    query.exec("CREATE TABLE IF NOT EXISTS expenses (id INTEGER PRIMARY KEY AUTOINCREMENT, description TEXT, amount REAL, category TEXT, date TEXT)");
                }

                void MainWindow::loadExpenses()
                {
                    ui->expensesTableWidget->setRowCount(0);
                    QSqlQuery query("SELECT id, description, amount, category, date FROM expenses");

                    while (query.next()) {
                        int row = ui->expensesTableWidget->rowCount();
                        ui->expensesTableWidget->insertRow(row);

                        QTableWidgetItem *descriptionItem = new QTableWidgetItem(query.value("description").toString());
                        descriptionItem->setData(Qt::UserRole, query.value("id").toInt());  // Store ID in UserRole
                        ui->expensesTableWidget->setItem(row, 0, descriptionItem);
                        ui->expensesTableWidget->setItem(row, 1, new QTableWidgetItem(query.value("amount").toString()));
                        ui->expensesTableWidget->setItem(row, 2, new QTableWidgetItem(query.value("category").toString()));
                        ui->expensesTableWidget->setItem(row, 3, new QTableWidgetItem(query.value("date").toString()));
                    }
                }

                void MainWindow::onAddExpense()
                {
                    QString description = ui->descriptionLineEdit->text();
                    QString amount = ui->amountLineEdit->text();
                    QString category = ui->categoryComboBox->currentText();
                    QString date = QDate::currentDate().toString("yyyy-MM-dd");

                    if (description.isEmpty() || amount.isEmpty()) {
                        QMessageBox::warning(this, "Input Error", "Description and amount cannot be empty.");
                        return;
                    }

                    QSqlQuery query;
                    query.prepare("INSERT INTO expenses (description, amount, category, date) VALUES (?, ?, ?, ?)");
                    query.addBindValue(description);
                    query.addBindValue(amount.toDouble());
                    query.addBindValue(category);
                    query.addBindValue(date);

                    if (query.exec()) {
                        qDebug() << "Expense added!";
                        loadExpenses();
                    } else {
                        qDebug() << "Error: Failed to add expense - " << query.lastError();
                    }
                }

                void MainWindow::onEditExpense()
                {
                    int row = ui->expensesTableWidget->currentRow();
                    if (row < 0) {
                        qDebug() << "No row selected for editing.";
                        return;
                    }

                    int id = ui->expensesTableWidget->item(row, 0)->data(Qt::UserRole).toInt();
                    qDebug() << "Editing expense ID:" << id;

                    QString description = ui->descriptionLineEdit->text();
                    QString amount = ui->amountLineEdit->text();
                    QString category = ui->categoryComboBox->currentText();
                    QString date = QDate::currentDate().toString("yyyy-MM-dd");

                    QSqlQuery query;
                    query.prepare("UPDATE expenses SET description = ?, amount = ?, category = ?, date = ? WHERE id = ?");
                    query.addBindValue(description);
                    query.addBindValue(amount.toDouble());
                    query.addBindValue(category);
                    query.addBindValue(date);
                    query.addBindValue(id);

                    if (query.exec()) {
                        qDebug() << "Expense updated!";
                        loadExpenses();
                    } else {
                        qDebug() << "Error: Failed to update expense - " << query.lastError();
                    }
                }

                void MainWindow::onDeleteExpense()
                {
                    int row = ui->expensesTableWidget->currentRow();
                    if (row < 0) {
                        qDebug() << "No row selected for deletion.";
                        return;
                    }

                    int id = ui->expensesTableWidget->item(row, 0)->data(Qt::UserRole).toInt();
                    qDebug() << "Deleting expense ID:" << id;

                    QSqlQuery query;
                    query.prepare("DELETE FROM expenses WHERE id = ?");
                    query.addBindValue(id);

                    if (query.exec()) {
                        qDebug() << "Expense deleted!";
                        loadExpenses();
                    } else {
                        qDebug() << "Error: Failed to delete expense - " << query.lastError();
                    }
                }

                void MainWindow::onClearForm()
                {
                    ui->descriptionLineEdit->clear();
                    ui->amountLineEdit->clear();
                    ui->categoryComboBox->setCurrentIndex(0);
                }

                void MainWindow::populateCategoryComboBox()
                {
                    ui->categoryComboBox->addItem("Food");
                    ui->categoryComboBox->addItem("Transportation");
                    ui->categoryComboBox->addItem("Utilities");
                    ui->categoryComboBox->addItem("Entertainment");
                    ui->categoryComboBox->addItem("Other");
                }

                void MainWindow::onRowSelectionChanged()
                {
                    int row = ui->expensesTableWidget->currentRow();
                    qDebug() << "Row selection changed, current row:" << row;

                    if (row >= 0) {
                        QString description = ui->expensesTableWidget->item(row, 0)->text();
                        QString amount = ui->expensesTableWidget->item(row, 1)->text();
                        QString category = ui->expensesTableWidget->item(row, 2)->text();

                        ui->descriptionLineEdit->setText(description);
                        ui->amountLineEdit->setText(amount);
                        ui->categoryComboBox->setCurrentText(category);
                    }
                }
