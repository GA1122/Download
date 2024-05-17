void WebPagePrivate::commitRootLayer(const IntRect& layoutRect, const IntRect& documentRect, bool drawsRootLayer)
{
#if DEBUG_AC_COMMIT
    Platform::logAlways(Platform::LogLevelCritical,
        "%s: m_compositor = 0x%p",
        WTF_PRETTY_FUNCTION, m_compositor.get());
#endif

    if (!m_compositor)
        return;

    LayerWebKitThread* rootLayer = 0;
    if (m_frameLayers)
        rootLayer = m_frameLayers->rootLayer();

    if (rootLayer && rootLayer->layerCompositingThread() != m_compositor->rootLayer())
        m_compositor->setRootLayer(rootLayer->layerCompositingThread());

    LayerWebKitThread* overlayLayer = 0;
    if (m_overlayLayer)
        overlayLayer = m_overlayLayer->platformLayer();

    if (overlayLayer && overlayLayer->layerCompositingThread() != m_compositor->overlayLayer())
        m_compositor->setOverlayLayer(overlayLayer->layerCompositingThread());

    m_compositor->setLayoutRect(layoutRect);
    m_compositor->setDocumentRect(documentRect);
    m_compositor->setDrawsRootLayer(drawsRootLayer);

    if (rootLayer)
        rootLayer->commitOnCompositingThread();

    if (overlayLayer)
        overlayLayer->commitOnCompositingThread();

    scheduleCompositingRun();
}
