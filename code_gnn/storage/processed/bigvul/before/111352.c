bool WebPage::setInputSelection(unsigned start, unsigned end)
{
    if (d->m_page->defersLoading())
        return false;
    return d->m_inputHandler->setSelection(start, end);
}
