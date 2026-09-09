#include "oduncteslim.h"
#include "ui_oduncteslim.h"

OduncTeslim::OduncTeslim(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OduncTeslim)
{
    ui->setupUi(this);

    ui->leUyeNo->setReadOnly(true);
    ui->leKitapNo->setReadOnly(true);
    ui->leUyeNo->setStyleSheet("background-color: #f0f0f0;");
    ui->leKitapNo->setStyleSheet("background-color: #f0f0f0;");

    ui->dateEdit->setDate(QDate::currentDate());

    listele();
}

OduncTeslim::~OduncTeslim()
{
    delete ui;
}

void OduncTeslim::on_tableWidget_cellClicked(int row, int column)
{
    if(ui->tableWidget->item(row, 0)) {
        ui->leUyeNo->setText(ui->tableWidget->item(row, 0)->text());
        ui->leKitapNo->setText(ui->tableWidget->item(row, 1)->text());

        seciliAlmaTarihi = ui->tableWidget->item(row, 2)->text();
    }
}

void OduncTeslim::on_btnOduncuVer_clicked()
{
    QString uyeNo = ui->leUyeNo->text();
    QString kitapNo = ui->leKitapNo->text();

    if (uyeNo.isEmpty() || kitapNo.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen sol tablodan teslim edilecek kaydı seçiniz!");
        return;
    }

    QDate almaTarihi = QDate::fromString(seciliAlmaTarihi, "dd/MM/yyyy");
    QDate vermeTarihi = ui->dateEdit->date();

    int gunFarki = almaTarihi.daysTo(vermeTarihi);
    int borc = 0;

    if (gunFarki > 15) {
        borc = (gunFarki - 15) * 4;
    } else if (gunFarki < 0) {
        QMessageBox::critical(this, "Hata", "Teslim tarihi, alma tarihinden önce olamaz!");
        return;
    }

    QSqlQuery teslimSorgu;
    teslimSorgu.prepare("INSERT INTO odunc_teslim_edilen (uye_no, kitap_no, alma_tarihi, verme_tarihi, borc) VALUES (?, ?, ?, ?, ?)");
    teslimSorgu.addBindValue(uyeNo);
    teslimSorgu.addBindValue(kitapNo);
    teslimSorgu.addBindValue(seciliAlmaTarihi);
    teslimSorgu.addBindValue(vermeTarihi.toString("dd/MM/yyyy"));
    teslimSorgu.addBindValue(borc);

    if (teslimSorgu.exec()) {
        QSqlQuery silSorgu;
        silSorgu.prepare("DELETE FROM odunc_alinan WHERE uye_no = ? AND kitap_no = ?");
        silSorgu.addBindValue(uyeNo);
        silSorgu.addBindValue(kitapNo);
        silSorgu.exec();

        if (borc > 0) {
            QMessageBox::warning(this, "Teslim Alındı", "Kitap teslim alındı!\nÜyenin " + QString::number(gunFarki) + " gün gecikmesi vardır.\nCezası: " + QString::number(borc) + " TL");
        } else {
            QMessageBox::information(this, "Başarılı", "Kitap zamanında teslim alındı. Borç: 0 TL");
        }

        ui->leUyeNo->clear();
        ui->leKitapNo->clear();
        listele();

    } else {
        QMessageBox::critical(this, "Hata!", "Kayıt işlemi başarısız: " + teslimSorgu.lastError().text());
    }
}

void OduncTeslim::listele()
{
    QSqlQuery sorguAlinan("SELECT uye_no, kitap_no, odunc_alma_tarihi FROM odunc_alinan");
    ui->tableWidget->setRowCount(0);
    int satir1 = 0;
    while (sorguAlinan.next()) {
        ui->tableWidget->insertRow(satir1);
        ui->tableWidget->setItem(satir1, 0, new QTableWidgetItem(sorguAlinan.value(0).toString()));
        ui->tableWidget->setItem(satir1, 1, new QTableWidgetItem(sorguAlinan.value(1).toString()));
        ui->tableWidget->setItem(satir1, 2, new QTableWidgetItem(sorguAlinan.value(2).toString()));
        satir1++;
    }

    QSqlQuery sorguTeslim("SELECT uye_no, kitap_no, alma_tarihi, verme_tarihi, borc FROM odunc_teslim_edilen");
    ui->tableWidget_2->setRowCount(0);
    int satir2 = 0;
    while (sorguTeslim.next()) {
        ui->tableWidget_2->insertRow(satir2);
        ui->tableWidget_2->setItem(satir2, 0, new QTableWidgetItem(sorguTeslim.value(0).toString()));
        ui->tableWidget_2->setItem(satir2, 1, new QTableWidgetItem(sorguTeslim.value(1).toString()));
        ui->tableWidget_2->setItem(satir2, 2, new QTableWidgetItem(sorguTeslim.value(2).toString()));
        ui->tableWidget_2->setItem(satir2, 3, new QTableWidgetItem(sorguTeslim.value(3).toString()));
        ui->tableWidget_2->setItem(satir2, 4, new QTableWidgetItem(sorguTeslim.value(4).toString()));
        satir2++;
    }
}
