    virtual PassOwnPtr<CCLayerTreeHostImpl> createLayerTreeHostImpl()
    {
        return MockLayerTreeHostImpl::create(m_testHooks, settings());
    }
