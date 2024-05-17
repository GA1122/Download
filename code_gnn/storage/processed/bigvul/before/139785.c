bool MockCanvasSurfaceLayerBridgeClient::syncGetSurfaceId(cc::SurfaceId* surfaceIdPtr)
{
     return m_service->GetSurfaceId(surfaceIdPtr);
 }
