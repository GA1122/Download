void WebPage::destroy()
{
    disableWebInspector();

    d->m_backingStore->d->suspendBackingStoreUpdates();
    d->m_backingStore->d->suspendScreenUpdates();

    d->m_page->backForward()->close();
    pageCache()->releaseAutoreleasedPagesNow();

    FrameLoader* loader = d->m_mainFrame->loader();

    d->m_mainFrame = 0;
    if (loader)
        loader->detachFromParent();

    deleteGuardedObject(this);
}
