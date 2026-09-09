#ifndef KITAPISLEMLERI_H
#define KITAPISLEMLERI_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class KitapIslemleri;
}

class KitapIslemleri : public QWidget
{
    Q_OBJECT

public:
    explicit KitapIslemleri(QWidget *parent = nullptr);
    ~KitapIslemleri();

private slots:
    void on_btnYeniKayt_clicked();
    void on_btnGncelle_clicked();
    void on_btnSil_clicked();
    void on_tblTumKitaplar_cellClicked(int row, int column);

private:
    Ui::KitapIslemleri *ui;
    void listele();
    void altTablolariDoldur(QString kitapNo);
};

#endif // KITAPISLEMLERI_H
