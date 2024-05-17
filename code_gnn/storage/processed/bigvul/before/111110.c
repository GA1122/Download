void WebPage::cut()
{
    if (d->m_page->defersLoading())
        return;
    d->m_inputHandler->cut();
}
