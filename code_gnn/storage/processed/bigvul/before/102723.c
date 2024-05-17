    virtual void scrollRootLayer(const IntSize& offset)
    {
        m_proxy->m_layerTreeHostImpl->scrollRootLayer(offset);
        m_proxy->setNeedsRedrawOnCCThread();
        m_proxy->setNeedsCommitOnCCThread();
    }
