void LayerTreeHostQt::createTile(WebLayerID layerID, int tileID, const UpdateInfo& updateInfo)
{
    m_webPage->send(Messages::LayerTreeHostProxy::CreateTileForLayer(layerID, tileID, updateInfo));
}
