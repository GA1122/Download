    explicit DeferredTaskPopupListSelectSingle(WebPagePrivate* webPagePrivate, int index)
        : DeferredTaskType(webPagePrivate)
    {
        webPagePrivate->m_cachedPopupListSelectedIndex = index;
    }
