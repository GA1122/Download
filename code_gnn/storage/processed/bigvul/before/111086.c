void WebPage::clearCurrentInputField()
{
    if (d->m_page->defersLoading())
        return;
    d->m_inputHandler->clearField();
}
