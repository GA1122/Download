void WebPagePrivate::resumeBackingStore()
{
    ASSERT(m_webPage->isVisible());

    if (!m_backingStore->d->isActive() || shouldResetTilesWhenShown()) {
        BackingStorePrivate::setCurrentBackingStoreOwner(m_webPage);

        if (m_backingStore->d->isOpenGLCompositing())
            setCompositorDrawsRootLayer(!m_backingStore->d->isActive());

        m_backingStore->d->orientationChanged();  
        m_backingStore->d->resetTiles();
        m_backingStore->d->updateTiles(false  , false  );

#if USE(ACCELERATED_COMPOSITING)
        setNeedsOneShotDrawingSynchronization();
#endif

        m_backingStore->d->renderAndBlitVisibleContentsImmediately();
    } else {
        if (m_backingStore->d->isOpenGLCompositing())
           setCompositorDrawsRootLayer(false);

        m_backingStore->d->updateTiles(true  , false  );
#if USE(ACCELERATED_COMPOSITING)
        setNeedsOneShotDrawingSynchronization();
#endif
    }

    setShouldResetTilesWhenShown(false);
}
