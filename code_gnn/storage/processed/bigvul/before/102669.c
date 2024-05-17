CCLayerTreeHost::CCLayerTreeHost(CCLayerTreeHostClient* client, PassRefPtr<LayerChromium> rootLayer, const CCSettings& settings)
    : m_compositorIdentifier(-1)
    , m_animating(false)
    , m_client(client)
    , m_frameNumber(0)
    , m_rootLayer(rootLayer)
    , m_settings(settings)
    , m_visible(true)
{
    CCMainThread::initialize();
    ASSERT(CCProxy::isMainThread());
}
