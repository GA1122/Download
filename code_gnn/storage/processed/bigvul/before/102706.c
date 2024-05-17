    static PassOwnPtr<CCThreadProxyScrollControllerAdapter> create(CCThreadProxy* proxy)
    {
        return adoptPtr(new CCThreadProxyScrollControllerAdapter(proxy));
    }
