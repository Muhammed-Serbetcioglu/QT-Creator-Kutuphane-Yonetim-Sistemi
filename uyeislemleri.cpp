#include "uyeislemleri.h"
#include "ui_uyeislemleri.h"

UyeIslemleri::UyeIslemleri(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UyeIslemleri)
{
    ui->setupUi(this);

    listele();
}

UyeIslemleri::~UyeIslemleri()
{
    delete ui;
}

void UyeIslemleri::on_btnYeniKayit_clicked()
{
    if (ui->leUyeAd->text().isEmpty() || ui->leUyeSoyad->text().isEmpty()) {
        QMessageBox::critical(this, "Hata!", "Gerekli alanları doldurunuz!");
        return;
    }

    QSqlQuery sorgu;
    sorgu.prepare("INSERT INTO uye (uye_ad, uye_soyad) VALUES (?, ?)");

    sorgu.addBindValue(ui->leUyeAd->text());
    sorgu.addBindValue(ui->leUyeSoyad->text());

    if (sorgu.exec()) {
        QMessageBox::information(this, "Başarılı", "Yeni üye kaydı tamamlandı.");
        ui->leUyeAd->clear();
        ui->leUyeSoyad->clear();
        listele();
    } else {
        QMessageBox::critical(this, "Hata!", "Kayıt hatası: " + sorgu.lastError().text());
    }
}

void UyeIslemleri::on_btnGuncelle_clicked()
{
    QString uyeNo = ui->leUyeNo->text();

    if (uyeNo.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen listeden güncellenecek üyeyi seçin!");
        return;
    }

    QSqlQuery sorgu;
    sorgu.prepare("UPDATE uye SET uye_ad = ?, uye_soyad = ? WHERE uye_no = ?");
    sorgu.addBindValue(ui->leUyeAd->text());
    sorgu.addBindValue(ui->leUyeSoyad->text());
    sorgu.addBindValue(uyeNo);

    if (sorgu.exec()) {
        QMessageBox::information(this, "Başarılı", "Üye bilgileri güncellendi.");
        listele();
    } else {
        QMessageBox::critical(this, "Hata", "Güncelleme hatası: " + sorgu.lastError().text());
    }
}

void UyeIslemleri::on_btnSil_clicked()
{
    QString uyeNo = ui->leUyeNo->text();

    if (uyeNo.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen listeden silinecek üyeyi seçin!");
        return;
    }

    QSqlQuery kontrol;
    kontrol.prepare("SELECT count(*) FROM odunc_alinan WHERE uye_no = ?");
    kontrol.addBindValue(uyeNo);
    kontrol.exec();
    kontrol.next();

    if (kontrol.value(0).toInt() > 0) {
        QMessageBox::critical(this, "Hata!", "Bu üye silinemez. Üyenin henüz teslim etmediği kitaplar vardır.");
    } else {
        QMessageBox::StandardButton onay;
        onay = QMessageBox::question(this, "Onay", "Seçili üyeyi silmek istediğinize emin misiniz?",
                                     QMessageBox::Yes | QMessageBox::No);

        if (onay == QMessageBox::Yes) {
            QSqlQuery silSorgu;
            silSorgu.prepare("DELETE FROM uye WHERE uye_no = ?");
            silSorgu.addBindValue(uyeNo);

            if (silSorgu.exec()) {
                QMessageBox::information(this, "Başarılı", "Üye kaydı silindi.");
                listele();

                ui->leUyeNo->clear();
                ui->leUyeAd->clear();
                ui->leUyeSoyad->clear();
            }
        }
    }
}

void UyeIslemleri::on_tableWidget_cellClicked(int row, int column)
{
    if (ui->tableWidget->item(row, 0)) ui->leUyeNo->setText(ui->tableWidget->item(row, 0)->text());
    if (ui->tableWidget->item(row, 1)) ui->leUyeAd->setText(ui->tableWidget->item(row, 1)->text());
    if (ui->tableWidget->item(row, 2)) ui->leUyeSoyad->setText(ui->tableWidget->item(row, 2)->text());
}

void UyeIslemleri::listele() {
    QSqlQuery sorgu;
    sorgu.prepare("SELECT * FROM uye");

    if (!sorgu.exec()) {
        QMessageBox::critical(this, "Hata", "Veriler çekilemedi: " + sorgu.lastError().text());
        return;
    }

    ui->tableWidget->setRowCount(0);
    int satir = 0;
    while (sorgu.next()) {
        ui->tableWidget->insertRow(satir);
        ui->tableWidget->setItem(satir, 0, new QTableWidgetItem(sorgu.value("uye_no").toString()));
        ui->tableWidget->setItem(satir, 1, new QTableWidgetItem(sorgu.value("uye_ad").toString()));
        ui->tableWidget->setItem(satir, 2, new QTableWidgetItem(sorgu.value("uye_soyad").toString()));
        satir++;
    }
}
