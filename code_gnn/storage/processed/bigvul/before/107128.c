void LayerTreeHostQt::didSyncCompositingStateForLayer(const WebLayerInfo& info)
{
    m_shouldSyncFrame = true;
    m_webPage->send(Messages::LayerTreeHostProxy::SyncCompositingLayerState(info));
}
