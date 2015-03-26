# Introduction #

Short guide on compilation


# Details #

This project depends on qt-oauth-lib library (here in the Google code) and QJson library.

You need to get all three projects by:

  * **_git clone https://code.google.com/p/qt-google-books/_**
  * **_git clone https://code.google.com/p/qt-oauth-lib/_**
  * **_git clone git://gitorious.org/qjson/qjson.git_**

All three folders: **_qt-google-books, qt-google-books, qjson_** must be in the same directory.

Then you need to compile libraries first. The **_qt-oauth-lib_** should be in the its directory (no shadow build).
We count that **qjson** library is in the **_qjson/build/lib_** directory.


Then the **qt-google-books** project will be compiled OK. To run it you need to adjust **LD\_LIBRARY\_PATH** on Linux (or **PATH** on Windows) so that variable points to the **_qjson/build/lib_** directory.