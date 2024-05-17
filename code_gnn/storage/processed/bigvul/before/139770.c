CanvasSurfaceLayerBridge::CanvasSurfaceLayerBridge(std::unique_ptr<CanvasSurfaceLayerBridgeClient> client)
{
    m_client = std::move(client);
}
