    void preload(Document* document, const KURL& baseURL)
    {
        if (m_urlToLoad.isEmpty())
            return;

        CachedResourceLoader* cachedResourceLoader = document->cachedResourceLoader();
        CachedResourceRequest request(ResourceRequest(document->completeURL(m_urlToLoad, baseURL)));
        request.setInitiator(tagName());
        if (m_tagName == scriptTag) {
            request.mutableResourceRequest().setAllowCookies(crossOriginModeAllowsCookies());
            cachedResourceLoader->preload(CachedResource::Script, request, m_charset);
        }
        else if (m_tagName == imgTag || (m_tagName == inputTag && m_inputIsImage))
            cachedResourceLoader->preload(CachedResource::ImageResource, request, String());
        else if (m_tagName == linkTag && m_linkIsStyleSheet && m_linkMediaAttributeIsScreen) 
            cachedResourceLoader->preload(CachedResource::CSSStyleSheet, request, m_charset);
    }
