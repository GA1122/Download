void LayerWebKitThread::addOverlay(PassRefPtr<LayerWebKitThread> overlay)
{
    insert(m_overlays, overlay, m_overlays.size());
}
