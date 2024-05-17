void Document::setViewportDescription(const ViewportDescription& viewportDescription)
{
    if (viewportDescription.isLegacyViewportType()) {
        if (viewportDescription == m_legacyViewportDescription)
            return;
        m_legacyViewportDescription = viewportDescription;
    } else {
        if (viewportDescription == m_viewportDescription)
            return;
        m_viewportDescription = viewportDescription;

        if (!viewportDescription.isSpecifiedByAuthor())
            m_viewportDefaultMinWidth = viewportDescription.minWidth;
    }

    updateViewportDescription();
}
