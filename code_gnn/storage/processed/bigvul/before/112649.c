void DocumentLoader::continueAfterNavigationPolicy(const ResourceRequest&, bool shouldContinue)
{
    if (!shouldContinue)
        stopLoadingForPolicyChange();
    else if (m_substituteData.isValid()) {
        ASSERT(timing()->redirectCount());

        RefPtr<ResourceLoader> resourceLoader = mainResourceLoader();
        ASSERT(resourceLoader->shouldSendResourceLoadCallbacks());
        resourceLoader->setSendCallbackPolicy(DoNotSendCallbacks);
        if (m_mainResource) {
            m_mainResource->removeClient(this);
            m_mainResource = 0;
        }
        resourceLoader->setSendCallbackPolicy(SendCallbacks);
        handleSubstituteDataLoadSoon();
    }
}
