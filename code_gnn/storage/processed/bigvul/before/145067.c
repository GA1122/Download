    virtual void SetUp()
    {
        blink::Platform::initialize(&m_proxyPlatform);

        m_globalMemoryCache = replaceMemoryCacheForTesting(MemoryCache::create());

        m_fetcher = ResourceFetcher::create(MockFetchContext::create());
    }
