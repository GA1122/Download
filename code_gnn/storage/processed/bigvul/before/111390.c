void WebPagePrivate::suspendBackingStore()
{
#if USE(ACCELERATED_COMPOSITING)
    if (m_backingStore->d->isOpenGLCompositing()) {
        if (m_visible)
            setCompositorDrawsRootLayer(true);

        return;
    }
#endif
}
