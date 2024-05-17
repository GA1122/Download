    explicit DeferredTaskSetColorInput(WebPagePrivate* webPagePrivate, BlackBerry::Platform::String value)
        : DeferredTaskType(webPagePrivate)
    {
        webPagePrivate->m_cachedColorInput = value;
    }
