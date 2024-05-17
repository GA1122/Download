    virtual void drawLayersOnCCThread(CCLayerTreeHostImpl* impl)
    {
        if (!impl->sourceFrameNumber())
            postSetNeedsCommitThenRedrawToMainThread();
        else if (impl->sourceFrameNumber() == 1)
            endTest();
    }
