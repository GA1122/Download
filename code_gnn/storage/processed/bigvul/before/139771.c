void CanvasSurfaceLayerBridge::requireCallback(const cc::SurfaceId& surfaceId, const cc::SurfaceSequence& sequence)
{
    m_client->asyncRequire(surfaceId, sequence);
}
