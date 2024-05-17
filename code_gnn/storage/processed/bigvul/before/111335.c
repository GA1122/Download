void WebPagePrivate::setCompositor(PassRefPtr<WebPageCompositorPrivate> compositor)
{
    using namespace BlackBerry::Platform;

    ASSERT(webKitThreadMessageClient()->isCurrentThread());

    if (m_compositor || m_client->window())
        m_backingStore->d->suspendScreenUpdates();

    userInterfaceThreadMessageClient()->dispatchSyncMessage(createMethodCallMessage(&WebPagePrivate::setCompositorHelper, this, compositor));

    if (m_compositor || m_client->window())  
        m_backingStore->d->resumeScreenUpdates(BackingStore::RenderAndBlit);
}
