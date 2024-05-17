void DocumentLoader::callContinueAfterNavigationPolicy(void* argument, const ResourceRequest& request, PassRefPtr<FormState>, bool shouldContinue)
{
    static_cast<DocumentLoader*>(argument)->continueAfterNavigationPolicy(request, shouldContinue);
}
