void WebPage::initializeIconDataBase()
{
    IconDatabaseClientBlackBerry::instance()->initIconDatabase(d->m_webSettings);
}
