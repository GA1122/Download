IntRect WebPagePrivate::mapFromTransformedContentsToTransformedViewport(const IntRect& rect) const
{
    return m_backingStoreClient->mapFromTransformedContentsToTransformedViewport(rect);
}
