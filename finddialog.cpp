#include "finddialog.h"
#include "ui_finddialog.h"
#include "books_data_manager.h"
#include <QMessageBox>

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    connect(ui->findButton, SIGNAL(clicked()), this, SLOT(findBooks()));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addBook()));

}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::setBookshelves(const QVariantList& bookshelves)
{
    m_bookshelves = bookshelves;
    QStringList lst;
    for(int i = 0; i < m_bookshelves.count(); ++i)
    {
        lst << m_bookshelves[i].toMap()["title"].toString();
    }
    ui->bookshelvesListWidget->clear();
    ui->bookshelvesListWidget->addItems(lst);
}

void FindDialog::setBooksDataManager(BooksDataManager* pManager)
{
    m_pBooksDataManager = pManager;
    connect(m_pBooksDataManager, SIGNAL(searchVolumesReady()), this, SLOT(searchVolumesReady()));
}

void FindDialog::setAccessToken(const QString& access_token)
{
    m_strAccessToken = access_token;
}

void FindDialog::findBooks()
{
    QString search_string;
    if(!ui->authorLineEdit->text().isEmpty())
    {
        search_string += "inauthor:" + ui->authorLineEdit->text() + "+";
    }
    if(!ui->titleLineEdit->text().isEmpty())
    {
        search_string += "intitle:" + ui->titleLineEdit->text();
    }
    if(search_string.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Empty both strings for search."));
        return;
    }
    search_string.replace(" ", "+");
    m_pBooksDataManager->searchVolumes(search_string);
}

void FindDialog::searchVolumesReady()
{
    m_searchVolumes = m_pBooksDataManager->getSearchVolumes();
    QStringList items;
    for(int k = 0; k < m_searchVolumes.count(); ++k)
    {
        QString authors;
        QVariantList lst = m_searchVolumes[k].toMap()["volumeInfo"].toMap()["authors"].toList();
        for(int i = 0; i < lst.count(); ++i)
        {
            authors += lst[i].toString();
            if(i < lst.count()-1)
                authors += ", ";
        }
        QString title = m_searchVolumes[k].toMap()["volumeInfo"].toMap()["title"].toString();

        items << title + " by " + authors;
    }

    ui->resultsListWidget->clear();
    ui->resultsListWidget->addItems(items);
}

void FindDialog::addBook()
{
    int volume_index = ui->resultsListWidget->currentRow();
    if(volume_index == -1)
    {
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
    QString volume_id = m_searchVolumes[volume_index].toMap()["id"].toString();

    m_pBooksDataManager->addVolumeToMyLibrary(m_strAccessToken, shelf_id, volume_id);

}

