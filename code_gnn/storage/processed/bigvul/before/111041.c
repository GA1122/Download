    explicit DeferredTaskLoadManualScript(WebPagePrivate* webPagePrivate, const KURL& url)
        : DeferredTaskType(webPagePrivate)
    {
        webPagePrivate->m_cachedManualScript = url;
    }
