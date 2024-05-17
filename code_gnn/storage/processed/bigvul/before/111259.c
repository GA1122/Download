void WebPagePrivate::notifyTransformedContentsSizeChanged()
{
    m_previousContentsSize = contentsSize();

    const IntSize size = transformedContentsSize();
    m_backingStore->d->contentsSizeChanged(size);
    m_client->contentsSizeChanged();
}
