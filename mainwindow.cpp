#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uyeislemleri.h"
#include "kitapislemleri.h"
#include "oduncalma.h"
#include "oduncteslim.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("kutuphane.db");
    if (db.open()) {
        ui->statusbar->showMessage("Veri tabanına bağlanıldı!");
    } else {
        ui->statusbar->showMessage("Veri tabanı bağlantı hatası!");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnUye_clicked()
{
    UyeIslemleri *uyeSayfasi = new UyeIslemleri();
    uyeSayfasi->show();
}

void MainWindow::on_btnKitap_clicked()
{
    KitapIslemleri *kitapSayfasi = new KitapIslemleri();
    kitapSayfasi->show();
}

void MainWindow::on_btnOduncAlma_clicked()
{
    OduncAlma *oduncAlmaSayfasi = new OduncAlma();
    oduncAlmaSayfasi->show();
}

void MainWindow::on_btnOduncTeslim_clicked()
{
    OduncTeslim *teslimSayfasi = new OduncTeslim();
    teslimSayfasi->show();
}
