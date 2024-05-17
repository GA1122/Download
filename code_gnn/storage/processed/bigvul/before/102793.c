    void postSetNeedsRedrawToMainThread()
    {
         callOnMainThread(CCLayerTreeHostTest::dispatchSetNeedsRedraw, this);
     }
