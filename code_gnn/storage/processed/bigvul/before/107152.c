void LayerTreeHostQt::updateTile(WebLayerID layerID, int tileID, const UpdateInfo& updateInfo)
{
    m_webPage->send(Messages::LayerTreeHostProxy::UpdateTileForLayer(layerID, tileID, updateInfo));
}
