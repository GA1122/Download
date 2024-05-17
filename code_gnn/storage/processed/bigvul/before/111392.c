void WebPagePrivate::syncDestroyCompositorOnCompositingThread()
{
    if (!m_compositor)
        return;

    Platform::userInterfaceThreadMessageClient()->dispatchSyncMessage(
        Platform::createMethodCallMessage(
            &WebPagePrivate::destroyCompositor, this));
}
