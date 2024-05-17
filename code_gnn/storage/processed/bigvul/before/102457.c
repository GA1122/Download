void LayerTreeCoordinator::detachLayer(CoordinatedGraphicsLayer* layer)
{
    m_registeredLayers.remove(layer);
    m_shouldSyncFrame = true;
    m_webPage->send(Messages::LayerTreeCoordinatorProxy::DeleteCompositingLayer(layer->id()));
}
