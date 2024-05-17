void LayerWebKitThread::removeSublayerOrOverlay(LayerWebKitThread* sublayer)
{
    remove(m_sublayers, sublayer);
    remove(m_overlays, sublayer);
}
