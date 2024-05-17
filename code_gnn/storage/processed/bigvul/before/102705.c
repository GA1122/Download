    static PassOwnPtr<CCThreadProxySchedulerClient> create(CCThreadProxy* proxy)
    {
        return adoptPtr(new CCThreadProxySchedulerClient(proxy));
    }
