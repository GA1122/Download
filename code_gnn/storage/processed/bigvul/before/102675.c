void CCLayerTreeHost::composite()
{
    ASSERT(!m_settings.enableCompositorThread);
    static_cast<CCSingleThreadProxy*>(m_proxy.get())->compositeImmediately();
}
