#ifndef READING_DIALOG_H
#define READING_DIALOG_H

#include <QDialog>
#include <QUrl>
#include <QNetworkRequest>

namespace Ui {
    class ReadingDialog;
}

class ReadingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReadingDialog(QWidget *parent = 0);
    ~ReadingDialog();

    void openUrl(const QString&);

private slots:
    void onUrlChanged(const QUrl& url);
    void downloadRequested(const QNetworkRequest & request);

private:
    Ui::ReadingDialog *ui;
};

#endif // READING_DIALOG_H
