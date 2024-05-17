void WebPage::selectAtDocumentPoint(const Platform::IntPoint& documentPoint)
{
    if (d->m_page->defersLoading())
        return;

    d->m_selectionHandler->selectAtPoint(documentPoint);
}
