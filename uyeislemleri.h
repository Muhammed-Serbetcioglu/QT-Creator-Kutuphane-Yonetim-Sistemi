#ifndef UYEISLEMLERI_H
#define UYEISLEMLERI_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class UyeIslemleri;
}

class UyeIslemleri : public QWidget
{
    Q_OBJECT

public:
    explicit UyeIslemleri(QWidget *parent = nullptr);
    ~UyeIslemleri();

private slots:
    void on_btnYeniKayit_clicked();
    void on_btnGuncelle_clicked();
    void on_btnSil_clicked();
    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::UyeIslemleri *ui;
    void listele();
};

#endif // UYEISLEMLERI_H
