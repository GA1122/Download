IntRect WebPagePrivate::mapFromViewportToContents(const IntRect& rect) const
{
    return m_backingStoreClient->mapFromViewportToContents(rect);
}
