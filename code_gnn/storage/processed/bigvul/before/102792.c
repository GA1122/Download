    void postSetNeedsCommitThenRedrawToMainThread()
    {
        callOnMainThread(CCLayerTreeHostTest::dispatchSetNeedsCommitThenRedraw, this);
    }
