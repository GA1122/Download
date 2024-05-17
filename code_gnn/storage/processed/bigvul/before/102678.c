GraphicsContext3D* CCLayerTreeHost::context()
{
    ASSERT(!m_settings.enableCompositorThread);
    return m_proxy->context();
}
