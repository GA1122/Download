void Document::processViewport(const String& features, ViewportArguments::Type origin)
{
    ASSERT(!features.isNull());

    if (origin < m_viewportArguments.type)
        return;

    m_viewportArguments = ViewportArguments(origin);
    processArguments(features, (void*)&m_viewportArguments, &setViewportFeature);

    updateViewportArguments();
}
