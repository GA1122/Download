    virtual ~WebFrameTest()
    {
        Platform::current()->unitTestSupport()->unregisterAllMockedURLs();
    }
