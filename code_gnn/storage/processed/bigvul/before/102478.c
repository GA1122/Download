void LayerTreeCoordinator::setLayerAnimatedOpacity(WebLayerID id, float opacity)
{
    m_shouldSyncFrame = true;
    m_webPage->send(Messages::LayerTreeCoordinatorProxy::SetLayerAnimatedOpacity(id, opacity));
}
