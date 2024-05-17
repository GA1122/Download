void LayerWebKitThread::removeFromSuperlayer()
{
    if (m_superlayer)
        m_superlayer->removeSublayerOrOverlay(this);
}
