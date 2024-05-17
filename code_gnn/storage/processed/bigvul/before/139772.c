void CanvasSurfaceLayerBridge::satisfyCallback(const cc::SurfaceSequence& sequence)
{
    m_client->asyncSatisfy(sequence);
}
