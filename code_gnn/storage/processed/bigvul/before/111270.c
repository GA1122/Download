void WebPage::paste()
{
    if (d->m_page->defersLoading())
        return;
    d->m_inputHandler->paste();
}
