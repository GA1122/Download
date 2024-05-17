    MockLayerTreeHostImpl(TestHooks* testHooks, const CCSettings& settings)
        : CCLayerTreeHostImpl(settings)
        , m_testHooks(testHooks)
    {
    }
