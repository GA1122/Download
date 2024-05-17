CanvasSurfaceLayerBridgeClientImpl::CanvasSurfaceLayerBridgeClientImpl()
{
    DCHECK(!m_service.is_bound());
    Platform::current()->interfaceProvider()->getInterface(mojo::GetProxy(&m_service));
}
