void WebPage::reload()
{
    d->m_mainFrame->loader()->reload(  true);
}
