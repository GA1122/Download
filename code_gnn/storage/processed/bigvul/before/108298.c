void MainResourceLoader::continueAfterNavigationPolicy(const ResourceRequest&, bool shouldContinue)
{
    if (!shouldContinue)
        stopLoadingForPolicyChange();
    deref();  
}
