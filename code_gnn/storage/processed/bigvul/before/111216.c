IntPoint WebPagePrivate::mapFromTransformedViewportToTransformedContents(const IntPoint& point) const
{
    return m_backingStoreClient->mapFromTransformedViewportToTransformedContents(point);
}
