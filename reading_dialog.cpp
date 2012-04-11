#include "reading_dialog.h"
#include "ui_reading_dialog.h"
#include <QDebug>

ReadingDialog::ReadingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadingDialog)
{
    ui->setupUi(this);
    connect(ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(onUrlChanged(QUrl)));
    connect(ui->webView->page(), SIGNAL(downloadRequested(const QNetworkRequest &)),
            this, SLOT(downloadRequested(const QNetworkRequest &)));

}

ReadingDialog::~ReadingDialog()
{
    delete ui;
}

void ReadingDialog::openUrl(const QString& s)
{
    ui->webView->setUrl(QUrl(s));
}

void ReadingDialog::onUrlChanged(const QUrl& url)
{
    qDebug() << "onUrlChanged" << url;
}

void ReadingDialog::downloadRequested(const QNetworkRequest & request)
{
    qDebug() << "downloadRequested" << request.url();
}
