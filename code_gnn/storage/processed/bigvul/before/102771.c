    static PassRefPtr<MockLayerTreeHost> create(TestHooks* testHooks, CCLayerTreeHostClient* client, PassRefPtr<LayerChromium> rootLayer, const CCSettings& settings)
    {
        return adoptRef(new MockLayerTreeHost(testHooks, client, rootLayer, settings));
    }
