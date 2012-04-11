#ifndef BOOKS_DATA_MANAGER_H
#define BOOKS_DATA_MANAGER_H

#include <QObject>
#include <QVariantList>

#include <QNetworkReply>
#include <QNetworkAccessManager>

class BooksDataManager : public QObject
{
    Q_OBJECT

public:
    BooksDataManager();

    //Requests to Google API service.
    void getMyLibraryBookshelves(const QString& access_token);
    void getMyLibraryVolumes(const QString& access_token, int shelf_id);
    void removeVolumeFromMyLibrary(const QString& access_token, int shelf_id, const QString& volume_id);
    void clearVolumesFromMyLibrary(const QString& access_token, int shelf_id);
    void addVolumeToMyLibrary(const QString& access_token, int shelf_id, const QString& volume_id);
    void searchVolumes(const QString& search_string);

    //Getting currently read bookshelves and volumes.
    QVariantList getBookshelves();
    QVariantList getVolumes();
    QVariantList getSearchVolumes();

signals:
    //Signals that are sent when reading is finished, and the results are ready.
    void bookshelvesReady();
    void volumesReady();
    void searchVolumesReady();
    void errorOccured(const QString& error);
    void volumeRemoved();

private slots:
    void replyFinished(QNetworkReply*);

private:
    QVariantList m_bookshelves;
    QVariantList m_volumes;
    QVariantList m_searchVolumes;
    QNetworkAccessManager* m_pNetworkAccessManager;

};

#endif // BOOKS_DATA_MANAGER_H
