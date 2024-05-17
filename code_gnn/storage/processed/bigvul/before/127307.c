    TestCachePolicyWebFrameClient()
        : m_policy(WebURLRequest::UseProtocolCachePolicy)
        , m_client(0)
        , m_willSendRequestCallCount(0)
        , m_childFrameCreationCount(0)
    {
    }
