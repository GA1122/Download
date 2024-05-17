 void CanvasSurfaceLayerBridgeTest::SetUp()
 {
     m_surfaceService = wrapUnique(new FakeOffscreenCanvasSurfaceImpl());
    std::unique_ptr<CanvasSurfaceLayerBridgeClient> bridgeClient = wrapUnique(new MockCanvasSurfaceLayerBridgeClient(m_surfaceService.get()));
    m_surfaceLayerBridge = wrapUnique(new CanvasSurfaceLayerBridge(std::move(bridgeClient)));
}
