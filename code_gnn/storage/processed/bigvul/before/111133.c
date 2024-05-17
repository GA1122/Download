WebDOMDocument WebPage::document() const
{
    if (!d->m_mainFrame)
        return WebDOMDocument();
    return WebDOMDocument(d->m_mainFrame->document());
}
