#include "form.h"
#include "ui_form.h"

#include <QDebug>
#include <QMessageBox>
#include "finddialog.h"
#include <QDesktopServices>
#include "reading_dialog.h"



Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);


    m_pOAuth2 = new OAuth2(this);
    m_pOAuth2->setScope("https://www.googleapis.com/auth/books");
    m_pOAuth2->setClientID("YOUR_CLIENT_ID_HERE");
    m_pOAuth2->setRedirectURI("YOUR_REDIRECT_URI_HERE");
    m_pOAuth2->setCompanyName("YOUR_COMPANY_NAME_HERE");
    m_pOAuth2->setAppName("QtBooks");

    connect(m_pOAuth2, SIGNAL(loginDone()), this, SLOT(loginDone()));


    connect(ui->bookshelvesListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(loadVolumes()));
    connect(ui->volumesListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(showVolumeInfo()));
    connect(ui->removeVolumeButton, SIGNAL(clicked()), this, SLOT(removeVolume()));
    connect(ui->removeAllVolumesButton, SIGNAL(clicked()), this, SLOT(clearVolumes()));
    connect(ui->readOnlineButton, SIGNAL(clicked()), this, SLOT(readBookOnline()));
    connect(ui->readPdfButton, SIGNAL(clicked()), this, SLOT(readBookPdf()));

    connect(&m_booksDataManager, SIGNAL(bookshelvesReady()), this, SLOT(getBookshelvesFromManager()));
    connect(&m_booksDataManager, SIGNAL(volumesReady()),     this, SLOT(getVolumesFromManager()));
    connect(&m_booksDataManager, SIGNAL(errorOccured(QString)), this, SLOT(errorOccured(QString)));
    connect(&m_booksDataManager, SIGNAL(volumeRemoved()), this, SLOT(loadVolumes()));

    m_pFindDialog = new FindDialog(this);
    m_pReadingDialog = new ReadingDialog(this);
}

Form::~Form()
{
    delete ui;
}


void Form::startLogin(bool bForce)
{
    //Now we allow to start logging in when m_oauth2.isAuthorized(). User can log in using another Google account.
    //if(!m_oauth2.isAuthorized())
    {
        m_pOAuth2->startLogin(bForce); //this is a parent widget for a login dialog.
    }
}

void Form::loginDone()
{
    qDebug() << "Form" <<  __FUNCTION__;
    //Now trying to get JSON string for bookshelves of the user.

    //Clearing all the lists - to avoid getting volumes while updating the bookshelves (it might be another user's login).
    ui->volumeInfoWebView->setHtml("");
    ui->volumesListWidget->clear();
    ui->bookshelvesListWidget->clear();
    m_booksDataManager.getMyLibraryBookshelves(m_pOAuth2->accessToken());
}



void Form::loadVolumes()
{
    ui->volumesListWidget->clear();
    int shelf_index = ui->bookshelvesListWidget->currentRow();
    if(shelf_index < 0)
    {
        return;
    }
    int shelf_id = m_bookshelves[shelf_index].toMap()["id"].toInt();
    qDebug() << "shelf_id" << shelf_id;
    m_booksDataManager.getMyLibraryVolumes(m_pOAuth2->accessToken(), shelf_id);
}

void Form::getBookshelvesFromManager()
{
    qDebug() << "Form::getBookshelvesFromManager";
    m_bookshelves = m_booksDataManager.getBookshelves();
    QStringList lst;
    for(int i = 0; i < m_bookshelves.count(); ++i)
    {
        lst << m_bookshelves[i].toMap()["title"].toString();
    }
    ui->bookshelvesListWidget->clear();
    qDebug() << "Form::getBookshelvesFromManager, after clearBookshelves";
    ui->bookshelvesListWidget->addItems(lst);
}

void Form::getVolumesFromManager()
{
    m_volumes = m_booksDataManager.getVolumes();
    QStringList lst;
    for(int i = 0; i < m_volumes.count(); ++i)
    {
        lst << m_volumes[i].toMap()["volumeInfo"].toMap()["title"].toString();
    }
    ui->volumesListWidget->clear();
    ui->volumesListWidget->addItems(lst);

}


void Form::showVolumeInfo()
{
    ui->volumeInfoWebView->setHtml("");
    int volume_index = ui->volumesListWidget->currentRow();
    if(volume_index == -1)
    {
        return;
    }

    QString str;
    str += "<html><body>\n";

    QString strImage = m_volumes[volume_index].toMap()["volumeInfo"].toMap()["imageLinks"].toMap()["thumbnail"].toString();
    str += QString("<img src=\"%1\" width=128 height=192 />").arg(strImage);
    str += "<p><u>Title</u>: <b>" + m_volumes[volume_index].toMap()["volumeInfo"].toMap()["title"].toString() + "</b>\n\n";

    QString authors;
    QVariantList lst = m_volumes[volume_index].toMap()["volumeInfo"].toMap()["authors"].toList();
    for(int i = 0; i < lst.count(); ++i)
    {
        authors += lst[i].toString();
        if(i < lst.count()-1)
            authors += ", ";
    }
    str += "<P><u>Authors</u>: <b>" + authors + "</b>\n\n";

    str += "<P><u>Viewability</u>: <b>" + m_volumes[volume_index].toMap()["accessInfo"].toMap()["viewability"].toString() + "</b>\n\n";


    str += "<P><u>SelfLink</u>: <i>" + m_volumes[volume_index].toMap()["selfLink"].toString() + "</i>\n\n";
    str += "<P><u>Publisher</u>: " + m_volumes[volume_index].toMap()["volumeInfo"].toMap()["publisher"].toString() + "\n\n";
    str += "<P><u>Published Date</u>: " + m_volumes[volume_index].toMap()["volumeInfo"].toMap()["publishedDate"].toString() + "\n\n";
    str += "<P><u>Description</u>: " + m_volumes[volume_index].toMap()["volumeInfo"].toMap()["description"].toString() + "\n\n";

    str += "</body></html>\n";


    ui->volumeInfoWebView->setHtml(str);

    QString pdfUrl = m_volumes[volume_index].toMap()["accessInfo"].toMap()["pdf"].toMap()["downloadLink"].toString();
    if(pdfUrl.isEmpty())
    {
        ui->readPdfButton->setEnabled(false);
    }
    else
    {
        ui->readPdfButton->setEnabled(true);
    }


}

void Form::removeVolume()
{
    qDebug() << "removeVolume";
    int volume_index = ui->volumesListWidget->currentRow();
    if(volume_index == -1)
    {
        ui->volumeInfoWebView->setHtml("");
        QMessageBox::warning(this, tr("Warning"), tr("No selected volume."));
        return;
    }
    int shelf_index = ui->bookshelvesListWidget->currentRow();
    if(shelf_index == -1)
    {
        QMessageBox::warning(this, tr("Warning"), tr("No selected bookshelf."));
        return;
    }
    int shelf_id = m_bookshelves[shelf_index].toMap()["id"].toInt();
    QString volume_id = m_volumes[volume_index].toMap()["id"].toString();

    m_booksDataManager.removeVolumeFromMyLibrary(m_pOAuth2->accessToken(), shelf_id, volume_id);

}

void Form::clearVolumes()
{
    qDebug() << "clearVolumes";
    int shelf_index = ui->bookshelvesListWidget->currentRow();
    if(shelf_index == -1)
    {
        QMessageBox::warning(this, tr("Warning"), tr("No selected bookshelf."));
        return;
    }
    int shelf_id = m_bookshelves[shelf_index].toMap()["id"].toInt();

    m_booksDataManager.clearVolumesFromMyLibrary(m_pOAuth2->accessToken(), shelf_id);

}


void Form::errorOccured(const QString& error)
{
    if(error.indexOf("Token invalid") != -1)
    {
        startLogin(true);
    }
    else
    {
        QMessageBox::warning(this, tr("Error occured"), error);
    }
}

void Form::findBook()
{
    qDebug() << "findBook";
    m_pFindDialog->setBookshelves(m_bookshelves);
    m_pFindDialog->setBooksDataManager(&m_booksDataManager);
    m_pFindDialog->setAccessToken(m_pOAuth2->accessToken());

    m_pFindDialog->exec();
}

void Form::readBookOnline()
{
    qDebug() << "readBook";
    int volume_index = ui->volumesListWidget->currentRow();
    if(volume_index == -1)
    {
        ui->volumeInfoWebView->setHtml("");
        QMessageBox::warning(this, tr("Warning"), tr("No selected volume."));
        return;
    }
    QString id = m_volumes[volume_index].toMap()["id"].toString();
    QString s = QString("http://books.google.com/books?id=%1").arg(id);
    qDebug() << "Before openUrl" << s;
    //QDesktopServices::openUrl(QUrl(s));
    m_pReadingDialog->openUrl(s);
    m_pReadingDialog->show();
}

void Form::readBookPdf()
{
    qDebug() << "readBookPdf";
    int volume_index = ui->volumesListWidget->currentRow();
    if(volume_index == -1)
    {
        ui->volumeInfoWebView->setHtml("");
        QMessageBox::warning(this, tr("Warning"), tr("No selected volume."));
        return;
    }
    QString pdfUrl = m_volumes[volume_index].toMap()["accessInfo"].toMap()["pdf"].toMap()["downloadLink"].toString();
    if(pdfUrl.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot find PDF link."));
        return;
    }
    QDesktopServices::openUrl(QUrl(pdfUrl));
    //m_pReadingDialog->openUrl(pdfUrl);
    //m_pReadingDialog->show();

}
