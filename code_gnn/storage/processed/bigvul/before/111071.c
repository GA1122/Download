void WebPagePrivate::blitVisibleContents()
{
    if (m_backingStore->d->shouldDirectRenderingToWindow())
        return;

    m_backingStore->d->blitVisibleContents();
}
