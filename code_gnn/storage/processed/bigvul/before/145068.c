    virtual void TearDown()
    {
        memoryCache()->evictResources();

        replaceMemoryCacheForTesting(m_globalMemoryCache.release());
    }
