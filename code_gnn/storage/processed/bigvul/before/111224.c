double WebPagePrivate::maximumScale() const
{
    if (m_maximumScale >= zoomToFitScale() && m_maximumScale >= m_minimumScale && respectViewport())
        return m_maximumScale;

    return hasVirtualViewport() ? std::max<double>(zoomToFitScale(), 4.0) : 4.0;
}
