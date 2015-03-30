#include "books_data_manager.h"
#include <QDebug>
#include <QJson/Parser>
#include <QApplication>
#include <QNetworkRequest>

BooksDataManager::BooksDataManager()
{
    m_pNetworkAccessManager = new QNetworkAccessManager(this);
    connect(m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void BooksDataManager::getMyLibraryBookshelves(const QString& access_token)
{
    qDebug() << "BooksDataManager::getMyLibraryBookshelves";
    QString s = QString("https://www.googleapis.com/books/v1/mylibrary/bookshelves?pp=1&access_token=%1").arg(access_token);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->get(QNetworkRequest(QUrl(s)));
}

void BooksDataManager::getMyLibraryVolumes(const QString& access_token, int shelf_id)
{
    qDebug() << "BooksDataManager::getMyLibraryVolumes";
    QString s = QString("https://www.googleapis.com/books/v1/mylibrary/bookshelves/%1/volumes?pp=1&access_token=%2").arg(shelf_id).arg(access_token);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->get(QNetworkRequest(QUrl(s)));
}

void BooksDataManager::searchVolumes(const QString& search_string)
{
    QString s = QString("https://www.googleapis.com/books/v1/volumes?q=%1").arg(search_string);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->get(QNetworkRequest(QUrl(s)));
}


void BooksDataManager::removeVolumeFromMyLibrary(const QString& access_token, int shelf_id, const QString& volume_id)
{
    QString s = QString("https://www.googleapis.com/books/v1/mylibrary/bookshelves/%1/removeVolume?volumeId=%2&access_token=%3").
            arg(shelf_id).arg(volume_id).arg(access_token);

    QByteArray params;
    //params.append("Authorization: ");
    //params.append(access_token.toLatin1());
    //params.append("\nContent-Type: application/json\n");
    //params.append("Content-Length: ");
    //int content_length = params.count() + 3;
    //params.append(QString::number(content_length));

    QNetworkRequest request;
    request.setUrl(QUrl(s));
    request.setRawHeader("Content-Type", "application/json");


    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->post(request, params);
}

void BooksDataManager::addVolumeToMyLibrary(const QString& access_token, int shelf_id, const QString& volume_id)
{
    QString s = QString("https://www.googleapis.com/books/v1/mylibrary/bookshelves/%1/addVolume?volumeId=%2&access_token=%3").
            arg(shelf_id).arg(volume_id).arg(access_token);

    QByteArray params;
    //params.append("Authorization: ");
    //params.append(access_token.toLatin1());
    //params.append("\nContent-Type: application/json\n");
    //params.append("Content-Length: ");
    //int content_length = params.count() + 3;
    //params.append(QString::number(content_length));

    QNetworkRequest request;
    request.setUrl(QUrl(s));
    request.setRawHeader("Content-Type", "application/json");


    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->post(request, params);
}


void BooksDataManager::clearVolumesFromMyLibrary(const QString& access_token, int shelf_id)
{
    QString s = QString("https://www.googleapis.com/books/v1/mylibrary/bookshelves/%1/clearVolumes?access_token=%2").
            arg(shelf_id).arg(access_token);

    qDebug() << "BooksDataManager::clearVolumesFromMyLibrary" << s;

    QByteArray params;
    //params.append("Authorization: ");
    //params.append(access_token.toLatin1());
    //params.append("\nContent-Type: application/json\n");
    //params.append("Content-Length: ");
    //int content_length = params.count() + 3;
    //params.append(QString::number(content_length));

    QNetworkRequest request;
    request.setUrl(QUrl(s));
//#if QT_VERSION >= 0x040800
    request.setRawHeader("Content-Type", "application/json");
//#endif

    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_pNetworkAccessManager->post(request, params);

}


QVariantList BooksDataManager::getBookshelves()
{
    return m_bookshelves;
}

QVariantList BooksDataManager::getVolumes()
{
    return m_volumes;
}

QVariantList BooksDataManager::getSearchVolumes()
{
    return m_searchVolumes;
}

void BooksDataManager::replyFinished(QNetworkReply *reply)
{
    QApplication::restoreOverrideCursor();

    QString json = reply->readAll();
    //qDebug() << "Reply = " << json;
    //qDebug() << "URL = " << reply->url();
    QString strUrl = reply->url().toString();

    if(json.length() == 0 && (strUrl.indexOf("removeVolume") || strUrl.indexOf("clearVolumes")))
    {
        //Empty answer after removeVolume!
        //loadVolumes();
        emit volumeRemoved();
        return;
    }

    QJson::Parser parser;

    bool ok;

    // json is a QString containing the data to convert
#if QT_VERSION >= 0x050000
    // Qt5 code
    QVariant result = parser.parse (json.toUtf8(), &ok);
#else
    // Qt4 code
    QVariant result = parser.parse (json.toLatin1(), &ok);
#endif
    if(!ok)
    {
        emit errorOccured(QString("Cannot convert to QJson object: %1").arg(json));
        return;
    }

    if(result.toMap().contains("error"))
    {
        emit errorOccured(result.toMap()["error"].toMap()["message"].toString());
        return;
    }

    if(result.toMap()["kind"] == "books#bookshelves")
    {
        m_bookshelves = result.toMap()["items"].toList();
        emit bookshelvesReady();
    }
    else if(result.toMap()["kind"] == "books#volumes" && strUrl.indexOf("books/v1/volumes?q=") == -1) //Not search results
    {
        m_volumes = result.toMap()["items"].toList();
        emit volumesReady();
    }
    else if(result.toMap()["kind"] == "books#volumes" && strUrl.indexOf("books/v1/volumes?q=") != -1) //Search results
    {
        m_searchVolumes = result.toMap()["items"].toList();
        emit searchVolumesReady();
    }

}
