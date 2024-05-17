    virtual void beginTest()
    {
        postSetNeedsCommitThenRedrawToMainThread();
        endTest();
    }
