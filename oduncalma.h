#ifndef ODUNCALMA_H
#define ODUNCALMA_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>

namespace Ui {
class OduncAlma;
}

class OduncAlma : public QWidget
{
    Q_OBJECT

public:
    explicit OduncAlma(QWidget *parent = nullptr);
    ~OduncAlma();

private slots:
    void on_pushButton_clicked();
    void on_tableWidget_cellClicked(int row, int column);
    void on_tableWidget_2_cellClicked(int row, int column);

private:
    Ui::OduncAlma *ui;
    void listele();
};

#endif // ODUNCALMA_H
