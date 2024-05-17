    explicit DeferredTaskSetFocused(WebPagePrivate* webPagePrivate, bool focused)
        : DeferredTaskType(webPagePrivate)
    {
        webPagePrivate->m_cachedFocused = focused;
    }
