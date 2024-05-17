void LayerTreeCoordinator::syncCanvas(WebLayerID id, const IntSize& canvasSize, uint64_t graphicsSurfaceToken, uint32_t frontBuffer)
{
    m_shouldSyncFrame = true;
    m_webPage->send(Messages::LayerTreeCoordinatorProxy::SyncCanvas(id, canvasSize, graphicsSurfaceToken, frontBuffer));
}
