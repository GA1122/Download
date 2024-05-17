void LayerTreeHostQt::detachLayer(WebGraphicsLayer* layer)
{
    m_registeredLayers.remove(layer);
    m_shouldSyncFrame = true;
    m_webPage->send(Messages::LayerTreeHostProxy::DeleteCompositingLayer(layer->id()));
}
