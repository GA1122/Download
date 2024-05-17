void WebPagePrivate::releaseLayerResources()
{
    if (!isAcceleratedCompositingActive())
        return;

    if (m_frameLayers)
        m_frameLayers->releaseLayerResources();

    Platform::userInterfaceThreadMessageClient()->dispatchSyncMessage(
        Platform::createMethodCallMessage(&WebPagePrivate::releaseLayerResourcesCompositingThread, this));
}
