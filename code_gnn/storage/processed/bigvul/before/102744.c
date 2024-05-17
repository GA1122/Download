    MockLayerTreeHost(TestHooks* testHooks, CCLayerTreeHostClient* client, PassRefPtr<LayerChromium> rootLayer, const CCSettings& settings)
        : CCLayerTreeHost(client, rootLayer, settings)
        , m_testHooks(testHooks)
    {
        bool success = initialize();
        ASSERT(success);
        UNUSED_PARAM(success);
    }
