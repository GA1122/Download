void WebPage::setDocumentCaretPosition(const Platform::IntPoint& documentCaretPosition)
{
    if (d->m_page->defersLoading())
        return;

    d->m_selectionHandler->setCaretPosition(documentCaretPosition);
}
