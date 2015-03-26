# Introduction #

The qt-google-books example uses Google Books API. To use API you need to register your own application on Google. Do not worry: the procedure is very simple.


# Details #

You need to login to Google, so first you need to create simple Google account. Then you can visit the page

**https://code.google.com/apis/console**

there you can create your application. You need to check access to **Books API** in tab "Services".

Check access to **Books API** (set Status On) as shown in the next figure.

![http://dl.dropbox.com/u/72326178/SetServiceBooks.jpg](http://dl.dropbox.com/u/72326178/SetServiceBooks.jpg)

Then you need create client ID (example page view for this in next figure).

![http://dl.dropbox.com/u/72326178/SetAPIAccess.jpg](http://dl.dropbox.com/u/72326178/SetAPIAccess.jpg)

Then  you can see credentials of your application. You need to copy and paste **Client\_ID** and **Redirect\_URI** to the file **form.cpp**.
```
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

```

After that you can compile and run qt-google-books.