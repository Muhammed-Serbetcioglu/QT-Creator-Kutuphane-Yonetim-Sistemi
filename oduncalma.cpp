#include "oduncalma.h"
#include "ui_oduncalma.h"

OduncAlma::OduncAlma(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OduncAlma)
{
    ui->setupUi(this);

    ui->leUyeNo->setReadOnly(true);
    ui->leKitapNo->setReadOnly(true);

    ui->deAlmaTarihi->setDate(QDate::currentDate());

    listele();
}

OduncAlma::~OduncAlma()
{
    delete ui;
}

void OduncAlma::on_tableWidget_cellClicked(int row, int column)
{
    if(ui->tableWidget->item(row, 0)) {
        ui->leUyeNo->setText(ui->tableWidget->item(row, 0)->text());
    }
}

void OduncAlma::on_tableWidget_2_cellClicked(int row, int column)
{
    if(ui->tableWidget_2->item(row, 0)) {
        ui->leKitapNo->setText(ui->tableWidget_2->item(row, 0)->text());
    }
}

void OduncAlma::on_pushButton_clicked()
{
    QString uyeNo = ui->leUyeNo->text();
    QString kitapNo = ui->leKitapNo->text();
    QString tarih = ui->deAlmaTarihi->date().toString("dd/MM/yyyy");

    if (uyeNo.isEmpty() || kitapNo.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen tablolardan bir üye ve bir kitap seçiniz!");
        return;
    }

    QSqlQuery kontrolAyniKitap;
    kontrolAyniKitap.prepare("SELECT count(*) FROM odunc_alinan WHERE uye_no = ? AND kitap_no = ?");
    kontrolAyniKitap.addBindValue(uyeNo);
    kontrolAyniKitap.addBindValue(kitapNo);
    kontrolAyniKitap.exec();
    kontrolAyniKitap.next();

    if (kontrolAyniKitap.value(0).toInt() > 0) {
        QMessageBox::information(this, "Hata", "Bu üye bu kitabın bir tanesini şu an ödünç almış tekrar ödünç verilemez!");
        return;
    }

    QSqlQuery stokSorgusu;
    stokSorgusu.prepare("SELECT kitap_sayisi FROM kitap WHERE kitap_no = ?");
    stokSorgusu.addBindValue(kitapNo);
    stokSorgusu.exec();
    int toplamStok = 0;
    if (stokSorgusu.next()) {
        toplamStok = stokSorgusu.value(0).toInt();
    }

    QSqlQuery verilmisSorgusu;
    verilmisSorgusu.prepare("SELECT count(*) FROM odunc_alinan WHERE kitap_no = ?");
    verilmisSorgusu.addBindValue(kitapNo);
    verilmisSorgusu.exec();
    int verilmisAdet = 0;
    if (verilmisSorgusu.next()) {
        verilmisAdet = verilmisSorgusu.value(0).toInt();
    }

    if (verilmisAdet >= toplamStok) {
        QMessageBox::critical(this, "Hata!", "Kütüphanede bu kitaptan kalmamıştır, hepsi üyelere verilmiştir!");
        return;
    }

    QSqlQuery oduncVer;
    oduncVer.prepare("INSERT INTO odunc_alinan (uye_no, kitap_no, odunc_alma_tarihi) VALUES (?, ?, ?)");
    oduncVer.addBindValue(uyeNo);
    oduncVer.addBindValue(kitapNo);
    oduncVer.addBindValue(tarih);

    if (oduncVer.exec()) {
        QMessageBox::information(this, "Başarılı", "Kitap üyeye başarıyla ödünç verildi.");
        ui->leUyeNo->clear();
        ui->leKitapNo->clear();
        listele();
    } else {
        QMessageBox::critical(this, "Hata!", "Kayıt eklenemedi: " + oduncVer.lastError().text());
    }
}

void OduncAlma::listele()
{
    QSqlQuery sorguUye("SELECT uye_no, uye_ad, uye_soyad FROM uye");
    ui->tableWidget->setRowCount(0);
    int satirUye = 0;
    while (sorguUye.next()) {
        ui->tableWidget->insertRow(satirUye);
        ui->tableWidget->setItem(satirUye, 0, new QTableWidgetItem(sorguUye.value(0).toString()));
        ui->tableWidget->setItem(satirUye, 1, new QTableWidgetItem(sorguUye.value(1).toString()));
        ui->tableWidget->setItem(satirUye, 2, new QTableWidgetItem(sorguUye.value(2).toString()));
        satirUye++;
    }

    QSqlQuery sorguKitap("SELECT kitap_no, kitap_ad, kitap_sayisi FROM kitap");
    ui->tableWidget_2->setRowCount(0);
    int satirKitap = 0;
    while (sorguKitap.next()) {
        ui->tableWidget_2->insertRow(satirKitap);
        ui->tableWidget_2->setItem(satirKitap, 0, new QTableWidgetItem(sorguKitap.value(0).toString()));
        ui->tableWidget_2->setItem(satirKitap, 1, new QTableWidgetItem(sorguKitap.value(1).toString()));
        ui->tableWidget_2->setItem(satirKitap, 2, new QTableWidgetItem(sorguKitap.value(2).toString()));
        satirKitap++;
    }

    QSqlQuery sorguOdunc("SELECT uye_no, kitap_no, odunc_alma_tarihi FROM odunc_alinan");
    ui->tableWidget_3->setRowCount(0);
    int satirOdunc = 0;
    while (sorguOdunc.next()) {
        ui->tableWidget_3->insertRow(satirOdunc);
        ui->tableWidget_3->setItem(satirOdunc, 0, new QTableWidgetItem(sorguOdunc.value(0).toString()));
        ui->tableWidget_3->setItem(satirOdunc, 1, new QTableWidgetItem(sorguOdunc.value(1).toString()));
        ui->tableWidget_3->setItem(satirOdunc, 2, new QTableWidgetItem(sorguOdunc.value(2).toString()));
        satirOdunc++;
    }
}
