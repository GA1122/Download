bool WebPage::platformHasLocalDataForURL(const WebCore::KURL& url)
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

    RetainPtr<CFCachedURLResponseRef> response(AdoptCF, CFURLCacheCopyResponseForRequest(cache.get(), request.get()));    
    return response;
#else
    return false;
#endif
}
