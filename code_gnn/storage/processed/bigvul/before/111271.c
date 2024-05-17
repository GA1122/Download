    virtual void performInternal(WebPagePrivate* webPagePrivate)
    {
        webPagePrivate->m_mainFrame->script()->executeIfJavaScriptURL(webPagePrivate->m_cachedManualScript, DoNotReplaceDocumentIfJavaScriptURL);
    }
