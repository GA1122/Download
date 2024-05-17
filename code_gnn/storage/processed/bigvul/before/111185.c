void WebPage::insertText(const BlackBerry::Platform::String& string)
{
    if (d->m_page->defersLoading())
        return;
    d->m_inputHandler->insertText(string);
}
