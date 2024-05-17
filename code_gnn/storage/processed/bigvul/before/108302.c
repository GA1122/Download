void MainResourceLoader::didFinishLoading()
{
#if !PLATFORM(CF)
    ASSERT(shouldLoadAsEmptyDocument(frameLoader()->activeDocumentLoader()->url()) || !defersLoading());
#endif
    
    RefPtr<MainResourceLoader> protect(this);

#if ENABLE(OFFLINE_WEB_APPLICATIONS)
    RefPtr<DocumentLoader> dl = documentLoader();
#endif

    ASSERT(!documentLoader()->timing()->responseEnd);
    documentLoader()->timing()->responseEnd = m_timeOfLastDataReceived;
    frameLoader()->finishedLoading();
    ResourceLoader::didFinishLoading();
    
#if ENABLE(OFFLINE_WEB_APPLICATIONS)
    dl->applicationCacheHost()->finishedLoadingMainResource();
#endif
}
