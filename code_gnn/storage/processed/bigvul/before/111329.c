bool WebPage::selectionContainsDocumentPoint(const Platform::IntPoint& point)
{
    return d->m_selectionHandler->selectionContains(point);
}
