    virtual void beginTest()
    {
        m_layerTreeHost->rootLayer()->setMaxScrollPosition(IntSize(100, 100));
        m_layerTreeHost->rootLayer()->setScrollPosition(m_initialScroll);
        postSetNeedsCommitThenRedrawToMainThread();
    }
