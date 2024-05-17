void LayerTreeHostQt::performScheduledLayerFlush()
{
#if USE(TILED_BACKING_STORE)
    if (m_isSuspended || m_waitingForUIProcess)
        return;
#endif

    m_webPage->layoutIfNeeded();

    if (!m_isValid)
        return;

    m_shouldSyncFrame = false;
    flushPendingLayerChanges();
    if (!m_shouldSyncFrame)
        return;

    if (m_shouldSyncRootLayer) {
        m_webPage->send(Messages::LayerTreeHostProxy::SetRootCompositingLayer(toWebGraphicsLayer(m_rootLayer.get())->id()));
        m_shouldSyncRootLayer = false;
    }

    m_webPage->send(Messages::LayerTreeHostProxy::DidRenderFrame());
    m_waitingForUIProcess = true;

    if (!m_notifyAfterScheduledLayerFlush)
        return;

    static_cast<DrawingAreaImpl*>(m_webPage->drawingArea())->layerHostDidFlushLayers();
    m_notifyAfterScheduledLayerFlush = false;
}
