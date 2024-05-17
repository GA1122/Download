void WebPagePrivate::setRootLayerWebKitThread(Frame* frame, LayerWebKitThread* layer)
{
    if (!layer && !m_frameLayers)
        return;

    if (!layer) {
         ASSERT(m_frameLayers);
         m_frameLayers->removeLayerByFrame(frame);
         if (!m_frameLayers->hasLayer())
             m_frameLayers.clear();
    } else {
        if (!m_frameLayers)
            m_frameLayers = adoptPtr(new FrameLayers(this));

        if (!m_frameLayers->containsLayerForFrame(frame))
            m_frameLayers->addLayer(frame, layer);

        ASSERT(m_frameLayers);
    }

    LayerCompositingThread* rootLayerCompositingThread = 0;
    if (m_frameLayers && m_frameLayers->rootLayer())
        rootLayerCompositingThread = m_frameLayers->rootLayer()->layerCompositingThread();

    setRootLayerCompositingThread(rootLayerCompositingThread);
}
