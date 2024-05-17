IntPoint WebPagePrivate::mapFromContentsToViewport(const IntPoint& point) const
{
    return m_backingStoreClient->mapFromContentsToViewport(point);
}
