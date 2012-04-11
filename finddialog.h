#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QVariantList>

class BooksDataManager;

namespace Ui {
    class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = 0);
    ~FindDialog();

    void setBookshelves(const QVariantList& lst);
    void setBooksDataManager(BooksDataManager* pManager);
    void setAccessToken(const QString& access_token);

private slots:
    void findBooks();
    void searchVolumesReady();
    void addBook();

private:
    Ui::FindDialog *ui;
    QVariantList m_bookshelves;
    QVariantList m_searchVolumes;
    BooksDataManager* m_pBooksDataManager;
    QString m_strAccessToken;
};

#endif // FINDDIALOG_H
