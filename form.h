#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "oauth2.h"
#include "books_data_manager.h"

class FindDialog;
class ReadingDialog;

namespace Ui {
    class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();


    void startLogin(bool bForce); //Show login dialog even if there is gotten already access_token.

public slots:
    void findBook();

private slots:
    void loginDone();
    void loadVolumes();
    void showVolumeInfo();
    void removeVolume();
    void clearVolumes();

    void readBookOnline();
    void readBookPdf();

    void getBookshelvesFromManager();
    void getVolumesFromManager();
    void errorOccured(const QString& error);


private:
    Ui::Form *ui;

    OAuth2* m_pOAuth2;
    QVariantList m_bookshelves;
    QVariantList m_volumes;

    BooksDataManager m_booksDataManager;
    FindDialog* m_pFindDialog;
    ReadingDialog* m_pReadingDialog;


};

#endif // FORM_H
