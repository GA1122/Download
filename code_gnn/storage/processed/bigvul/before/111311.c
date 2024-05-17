void WebPagePrivate::restoreHistoryViewState(Platform::IntSize contentsSize, Platform::IntPoint scrollPosition, double scale, bool shouldReflowBlock)
{
    if (!m_mainFrame) {
        m_backingStore->d->resumeBackingStoreUpdates();
        m_backingStore->d->resumeScreenUpdates(BackingStore::RenderAndBlit);
        return;
    }

    m_mainFrame->view()->setContentsSizeFromHistory(contentsSize);

    bool oldConstrainsScrollingToContentEdge = m_mainFrame->view()->constrainsScrollingToContentEdge();
    m_mainFrame->view()->setConstrainsScrollingToContentEdge(false);
    setScrollPosition(scrollPosition);
    m_mainFrame->view()->setConstrainsScrollingToContentEdge(oldConstrainsScrollingToContentEdge);

    m_shouldReflowBlock = shouldReflowBlock;

    bool didZoom = zoomAboutPoint(scale, m_mainFrame->view()->scrollPosition(), true  , true  , true  );
    m_backingStore->d->resumeBackingStoreUpdates();
    m_backingStore->d->resumeScreenUpdates(BackingStore::RenderAndBlit);

    if (!didZoom) {
        notifyTransformedContentsSizeChanged();
        notifyTransformedScrollChanged();
    }
}
