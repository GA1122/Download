void Document::setViewportDescription(const ViewportDescription& viewportDescription)
{
    if (viewportDescription.isLegacyViewportType()) {
        m_legacyViewportDescription = viewportDescription;

        if (m_viewportDescription.type == ViewportDescription::AuthorStyleSheet)
            return;
        m_viewportDescription = viewportDescription;
    } else {
        if (!shouldOverrideLegacyViewport(viewportDescription.type))
            m_viewportDescription = m_legacyViewportDescription;
        else
            m_viewportDescription = viewportDescription;
    }

    updateViewportDescription();
}
