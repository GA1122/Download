void WebPagePrivate::resumeBackingStore()
{
    ASSERT(m_webPage->isVisible());

    bool directRendering = m_backingStore->d->shouldDirectRenderingToWindow();
    if (!m_backingStore->d->isActive()
        || shouldResetTilesWhenShown()
        || directRendering) {
        BackingStorePrivate::setCurrentBackingStoreOwner(m_webPage);

        if (m_backingStore->d->isOpenGLCompositing())
            setCompositorDrawsRootLayer(!m_backingStore->d->isActive());

        m_backingStore->d->orientationChanged();  
        m_backingStore->d->resetTiles();
        m_backingStore->d->updateTiles(false  , false  );

        directRendering = m_backingStore->d->shouldDirectRenderingToWindow();
        if (m_backingStore->d->renderVisibleContents()) {
            if (!m_backingStore->d->isSuspended() && !directRendering)
                m_backingStore->d->blitVisibleContents();
            m_client->notifyPixelContentRendered(m_backingStore->d->visibleContentsRect());
        }
    } else {
        if (m_backingStore->d->isOpenGLCompositing())
           setCompositorDrawsRootLayer(false);

        m_backingStore->d->updateTiles(true  , false  );
    }

#if USE(ACCELERATED_COMPOSITING)
    setNeedsOneShotDrawingSynchronization();
#endif

    setShouldResetTilesWhenShown(false);
}
