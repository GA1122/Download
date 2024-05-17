    static PassOwnPtr<MockLayerTreeHostImpl> create(TestHooks* testHooks, const CCSettings& settings)
    {
        return adoptPtr(new MockLayerTreeHostImpl(testHooks, settings));
    }
