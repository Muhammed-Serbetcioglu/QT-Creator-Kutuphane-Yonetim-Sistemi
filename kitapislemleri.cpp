#include "kitapislemleri.h"
#include "ui_kitapislemleri.h"

KitapIslemleri::KitapIslemleri(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::KitapIslemleri)
{
    ui->setupUi(this);

    ui->lineEdit->setReadOnly(true);
    ui->lineEdit->setStyleSheet("background-color: #f0f0f0;");

    listele();
}

KitapIslemleri::~KitapIslemleri()
{
    delete ui;
}

void KitapIslemleri::on_btnYeniKayt_clicked()
{
    if (ui->lineEdit_2->text().isEmpty() || ui->lineEdit_3->text().isEmpty()) {
        QMessageBox::critical(this, "Hata!", "Gerekli alanları doldurunuz!");
        return;
    }

    QSqlQuery sorgu;
    sorgu.prepare("INSERT INTO kitap (kitap_ad, kitap_sayisi) VALUES (?, ?)");
    sorgu.addBindValue(ui->lineEdit_2->text());
    sorgu.addBindValue(ui->lineEdit_3->text());

    if (sorgu.exec()) {
        QMessageBox::information(this, "Başarılı", "Yeni kitap eklendi.");
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        listele();
    } else {
        QMessageBox::critical(this, "Hata", sorgu.lastError().text());
    }
}

void KitapIslemleri::on_btnGncelle_clicked()
{
    QString kitapNo = ui->lineEdit->text();

    if (kitapNo.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Güncellenecek kitabı seçiniz!");
        return;
    }

    QSqlQuery sorgu;
    sorgu.prepare("UPDATE kitap SET kitap_ad = ?, kitap_sayisi = ? WHERE kitap_no = ?");
    sorgu.addBindValue(ui->lineEdit_2->text());
    sorgu.addBindValue(ui->lineEdit_3->text());
    sorgu.addBindValue(kitapNo);

    if (sorgu.exec()) {
        QMessageBox::information(this, "Başarılı", "Kitap güncellendi.");
        listele();
    }
}

void KitapIslemleri::on_btnSil_clicked()
{
    QString kitapNo = ui->lineEdit->text();

    if (kitapNo.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Silinecek kitabı seçiniz!");
        return;
    }

    QSqlQuery kontrol;
    kontrol.prepare("SELECT count(*) FROM odunc_alinan WHERE kitap_no = ?");
    kontrol.addBindValue(kitapNo);
    kontrol.exec();
    kontrol.next();

    if (kontrol.value(0).toInt() > 0) {
        QMessageBox::critical(this, "Hata!", "Bu kitap silinemez! Bu kitap bir üyeye ödünç verilmiştir.");
        return;
    }

    QMessageBox::StandardButton onay = QMessageBox::question(this, "Onay", "Kitabı silmek istediğinize emin misiniz?", QMessageBox::Yes | QMessageBox::No);
    if (onay == QMessageBox::Yes) {
        QSqlQuery silSorgu;
        silSorgu.prepare("DELETE FROM kitap WHERE kitap_no = ?");
        silSorgu.addBindValue(kitapNo);

        if (silSorgu.exec()) {
            QMessageBox::information(this, "Başarılı", "Kitap silindi.");
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
            ui->lineEdit_3->clear();
            listele();

            ui->tblSuAnkiler->setRowCount(0);
            ui->tblGecmis->setRowCount(0);
        }
    }
}

void KitapIslemleri::on_tblTumKitaplar_cellClicked(int row, int column)
{
    if (ui->tblTumKitaplar->item(row, 0)) ui->lineEdit->setText(ui->tblTumKitaplar->item(row, 0)->text());
    if (ui->tblTumKitaplar->item(row, 1)) ui->lineEdit_2->setText(ui->tblTumKitaplar->item(row, 1)->text());
    if (ui->tblTumKitaplar->item(row, 2)) ui->lineEdit_3->setText(ui->tblTumKitaplar->item(row, 2)->text());

    altTablolariDoldur(ui->lineEdit->text());
}

void KitapIslemleri::listele()
{
    QSqlQuery sorgu("SELECT * FROM kitap");
    ui->tblTumKitaplar->setRowCount(0);
    int satir = 0;
    while (sorgu.next()) {
        ui->tblTumKitaplar->insertRow(satir);
        ui->tblTumKitaplar->setItem(satir, 0, new QTableWidgetItem(sorgu.value("kitap_no").toString()));
        ui->tblTumKitaplar->setItem(satir, 1, new QTableWidgetItem(sorgu.value("kitap_ad").toString()));
        ui->tblTumKitaplar->setItem(satir, 2, new QTableWidgetItem(sorgu.value("kitap_sayisi").toString()));
        satir++;
    }
}

void KitapIslemleri::altTablolariDoldur(QString kitapNo)
{
    QSqlQuery suAnkiSorgu;
    suAnkiSorgu.prepare("SELECT uye_no, kitap_no, odunc_alma_tarihi FROM odunc_alinan WHERE kitap_no = ?");
    suAnkiSorgu.addBindValue(kitapNo);
    suAnkiSorgu.exec();

    ui->tblSuAnkiler->setRowCount(0);
    int satir1 = 0;
    while (suAnkiSorgu.next()) {
        ui->tblSuAnkiler->insertRow(satir1);
        ui->tblSuAnkiler->setItem(satir1, 0, new QTableWidgetItem(suAnkiSorgu.value(0).toString()));
        ui->tblSuAnkiler->setItem(satir1, 1, new QTableWidgetItem(suAnkiSorgu.value(1).toString()));
        ui->tblSuAnkiler->setItem(satir1, 2, new QTableWidgetItem(suAnkiSorgu.value(2).toString()));
        satir1++;
    }

    QSqlQuery gecmisSorgu;
    gecmisSorgu.prepare("SELECT uye_no, kitap_no, alma_tarihi FROM odunc_teslim_edilen WHERE kitap_no = ?");
    gecmisSorgu.addBindValue(kitapNo);
    gecmisSorgu.exec();

    ui->tblGecmis->setRowCount(0);
    int satir2 = 0;
    while (gecmisSorgu.next()) {
        ui->tblGecmis->insertRow(satir2);
        ui->tblGecmis->setItem(satir2, 0, new QTableWidgetItem(gecmisSorgu.value(0).toString()));
        ui->tblGecmis->setItem(satir2, 1, new QTableWidgetItem(gecmisSorgu.value(1).toString()));
        ui->tblGecmis->setItem(satir2, 2, new QTableWidgetItem(gecmisSorgu.value(2).toString()));
        satir2++;
    }
}
