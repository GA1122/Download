void LayerTreeHostQt::cancelPendingLayerFlush()
{
    m_layerFlushTimer.stop();
}
