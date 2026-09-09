#ifndef ODUNCTESLIM_H
#define ODUNCTESLIM_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>

namespace Ui {
class OduncTeslim;
}

class OduncTeslim : public QWidget
{
    Q_OBJECT

public:
    explicit OduncTeslim(QWidget *parent = nullptr);
    ~OduncTeslim();

private slots:
    void on_tableWidget_cellClicked(int row, int column);
    void on_btnOduncuVer_clicked();

private:
    Ui::OduncTeslim *ui;
    void listele();
    QString seciliAlmaTarihi;
};

#endif // ODUNCTESLIM_H
