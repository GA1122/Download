String WebPage::cachedResponseMIMETypeForURL(const WebCore::KURL& url)
{
#if USE(CFNETWORK)
    RetainPtr<CFURLRef> cfURL(AdoptCF, url.createCFURL());
    RetainPtr<CFMutableURLRequestRef> request(AdoptCF, CFURLRequestCreateMutable(0, cfURL.get(), kCFURLRequestCachePolicyReloadIgnoringCache, 60, 0));
    
    RetainPtr<CFStringRef> userAgent(AdoptCF, userAgent().createCFString());
    CFURLRequestSetHTTPHeaderFieldValue(request.get(), CFSTR("User-Agent"), userAgent.get());

    RetainPtr<CFURLCacheRef> cache;
#if USE(CFURLSTORAGESESSIONS)
    if (CFURLStorageSessionRef storageSession = ResourceHandle::privateBrowsingStorageSession())
        cache.adoptCF(wkCopyURLCache(storageSession));
    else
#endif
        cache.adoptCF(CFURLCacheCopySharedURLCache());

    RetainPtr<CFCachedURLResponseRef> cachedResponse(AdoptCF, CFURLCacheCopyResponseForRequest(cache.get(), request.get()));
    
    CFURLResponseRef response = CFCachedURLResponseGetWrappedResponse(cachedResponse.get());
    
    return response ? CFURLResponseGetMIMEType(response) : String();
#else
    return String();
#endif
}
