void DocumentLoader::reportMemoryUsage(MemoryObjectInfo* memoryObjectInfo) const
{
    MemoryClassInfo info(memoryObjectInfo, this, WebCoreMemoryTypes::Loader);
    info.addMember(m_frame, "frame");
    info.addMember(m_cachedResourceLoader, "cachedResourceLoader");
    info.addMember(m_mainResource, "mainResource");
    info.addMember(m_subresourceLoaders, "subresourceLoaders");
    info.addMember(m_multipartSubresourceLoaders, "multipartSubresourceLoaders");
    info.addMember(m_plugInStreamLoaders, "plugInStreamLoaders");
    info.addMember(m_substituteData, "substituteData");
    info.addMember(m_pageTitle.string(), "pageTitle.string()");
    info.addMember(m_overrideEncoding, "overrideEncoding");
    info.addMember(m_responses, "responses");
    info.addMember(m_originalRequest, "originalRequest");
    info.addMember(m_originalRequestCopy, "originalRequestCopy");
    info.addMember(m_request, "request");
    info.addMember(m_response, "response");
    info.addMember(m_lastCheckedRequest, "lastCheckedRequest");
    info.addMember(m_responses, "responses");
    info.addMember(m_pendingSubstituteResources, "pendingSubstituteResources");
    info.addMember(m_substituteResourceDeliveryTimer, "substituteResourceDeliveryTimer");
    info.addMember(m_archiveResourceCollection, "archiveResourceCollection");
#if ENABLE(WEB_ARCHIVE) || ENABLE(MHTML)
    info.addMember(m_archive, "archive");
    info.addMember(m_parsedArchiveData, "parsedArchiveData");
#endif
    info.addMember(m_resourcesClientKnowsAbout, "resourcesClientKnowsAbout");
    info.addMember(m_resourcesLoadedFromMemoryCacheForClientNotification, "resourcesLoadedFromMemoryCacheForClientNotification");
    info.addMember(m_clientRedirectSourceForHistory, "clientRedirectSourceForHistory");
    info.addMember(m_iconLoadDecisionCallback, "iconLoadDecisionCallback");
    info.addMember(m_iconDataCallback, "iconDataCallback");
    info.addMember(m_applicationCacheHost, "applicationCacheHost");
}
