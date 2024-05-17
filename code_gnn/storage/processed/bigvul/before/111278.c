    virtual void performInternal(WebPagePrivate* webPagePrivate)
    {
        webPagePrivate->m_webPage->setFocused(webPagePrivate->m_cachedFocused);
    }
