void FrameLoader::continueFragmentScrollAfterNavigationPolicy(const ResourceRequest& request, bool shouldContinue)
{
    m_quickRedirectComing = false;

    if (!shouldContinue)
        return;

    bool isRedirect = m_quickRedirectComing || policyChecker()->loadType() == FrameLoadTypeRedirectWithLockedBackForwardList;    
    loadInSameDocument(request.url(), 0, !isRedirect);
}
