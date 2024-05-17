IntRect WebPagePrivate::mapFromTransformedViewportToTransformedContents(const IntRect& rect) const
{
    return m_backingStoreClient->mapFromTransformedViewportToTransformedContents(rect);
}
