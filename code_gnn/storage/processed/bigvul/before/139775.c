 void CanvasSurfaceLayerBridgeClientImpl::asyncRequire(const cc::SurfaceId& surfaceId, const cc::SurfaceSequence& sequence)
 {
     m_service->Require(surfaceId, sequence);
}
