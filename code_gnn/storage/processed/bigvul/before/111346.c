void WebPage::setDocumentSelection(const Platform::IntPoint& documentStartPoint, const Platform::IntPoint& documentEndPoint)
{
    if (d->m_page->defersLoading())
        return;

    d->m_selectionHandler->setSelection(documentStartPoint, documentEndPoint);
}
