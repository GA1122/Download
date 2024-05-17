LayerWebKitThread::LayerWebKitThread(LayerType type, GraphicsLayerBlackBerry* owner)
    : LayerData(type)
    , m_owner(owner)
    , m_superlayer(0)
    , m_contents(0)
    , m_isDrawable(false)
    , m_isMask(false)
    , m_animationsChanged(false)
    , m_clearOverrideOnCommit(false)
#if ENABLE(CSS_FILTERS)
    , m_filtersChanged(false)
#endif
    , m_didStartAnimations(false)
{
    if (type == Layer)
        m_tiler = LayerTiler::create(this);
    m_layerCompositingThread = LayerCompositingThread::create(type, m_tiler.get());
}
