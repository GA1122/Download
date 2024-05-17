double WebPagePrivate::initialScale() const
{

    if (m_initialScale > 0.0 && respectViewport())
        return m_initialScale;

    if (m_webSettings->isZoomToFitOnLoad())
        return zoomToFitScale();

    return 1.0;
}
