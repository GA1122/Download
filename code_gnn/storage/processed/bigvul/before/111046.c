    explicit DeferredTaskSetDateTimeInput(WebPagePrivate* webPagePrivate, BlackBerry::Platform::String value)
        : DeferredTaskType(webPagePrivate)
    {
        webPagePrivate->m_cachedDateTimeInput = value;
    }
