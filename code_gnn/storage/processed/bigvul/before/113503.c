void WebPagePrivate::notifyTransformedScrollChanged()
{
    const IntPoint pos = transformedScrollPosition();
    m_backingStore->d->scrollChanged(pos);
    m_client->scrollChanged();

#if ENABLE(FULLSCREEN_API)
    adjustFullScreenElementDimensionsIfNeeded();
#endif
}
