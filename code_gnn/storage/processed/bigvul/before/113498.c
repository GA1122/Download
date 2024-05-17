double WebPagePrivate::maximumScale() const
{
    double zoomToFitScale = this->zoomToFitScale();
    if (m_maximumScale >= m_minimumScale && respectViewport())
        return std::max(zoomToFitScale, m_maximumScale);

    return hasVirtualViewport() ? std::max<double>(zoomToFitScale, 4.0) : 4.0;
}
