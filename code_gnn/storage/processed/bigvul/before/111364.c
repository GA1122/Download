void WebPagePrivate::setRootLayerCompositingThread(LayerCompositingThread* layer)
{
    if (!Platform::userInterfaceThreadMessageClient()->isCurrentThread()) {
        Platform::userInterfaceThreadMessageClient()->dispatchSyncMessage(
            Platform::createMethodCallMessage(&WebPagePrivate::setRootLayerCompositingThread, this, layer));
        return;
    }

    if (layer && !m_compositor)
        createCompositor();

    if (m_compositor)
        m_compositor->setRootLayer(layer);
}
