double WebPagePrivate::zoomToFitScale() const
{
    int contentWidth = contentsSize().width();

    Document* doc = m_page->mainFrame()->document();
    bool isImageDocument = doc && doc->isImageDocument();
    if (isImageDocument)
        contentWidth = static_cast<ImageDocument*>(doc)->imageSize().width();

    double zoomToFitScale = contentWidth > 0.0 ? static_cast<double>(m_actualVisibleWidth) / contentWidth : 1.0;
    int contentHeight = contentsSize().height();
    if (contentHeight * zoomToFitScale < static_cast<double>(m_defaultLayoutSize.height()))
        zoomToFitScale = contentHeight > 0 ? static_cast<double>(m_defaultLayoutSize.height()) / contentHeight : 1.0;
    zoomToFitScale = std::max(zoomToFitScale, minimumZoomToFitScale);

    if (!isImageDocument)
        return zoomToFitScale;

    return std::min(zoomToFitScale, maximumImageDocumentZoomToFitScale);
}
