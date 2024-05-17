void CanvasSurfaceLayerBridgeClientImpl::asyncSatisfy(const cc::SurfaceSequence& sequence)
{
    m_service->Satisfy(sequence);
}
