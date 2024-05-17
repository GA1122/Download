bool DocumentLoader::isLoadingInAPISense() const
{
    if (frameLoader()->state() != FrameStateComplete) {
        if (m_frame->settings()->needsIsLoadingInAPISenseQuirk() && !m_subresourceLoaders.isEmpty())
            return true;
    
        Document* doc = m_frame->document();
        if ((isLoadingMainResource() || !m_frame->document()->loadEventFinished()) && isLoading())
            return true;
        if (m_cachedResourceLoader->requestCount())
            return true;
        if (doc->processingLoadEvent())
            return true;
        if (doc->hasActiveParser())
            return true;
    }
    return frameLoader()->subframeIsLoading();
}
