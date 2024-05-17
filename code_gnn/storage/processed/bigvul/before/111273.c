    virtual void performInternal(WebPagePrivate* webPagePrivate)
    {
        webPagePrivate->m_webPage->popupListClosed(webPagePrivate->m_cachedPopupListSelecteds.size(), webPagePrivate->m_cachedPopupListSelecteds.data());
    }
