bool FrameLoader::shouldContinueForNavigationPolicy(const ResourceRequest& request, const SubstituteData& substituteData,
    DocumentLoader* loader, ContentSecurityPolicyDisposition shouldCheckMainWorldContentSecurityPolicy,
    NavigationType type, NavigationPolicy policy, bool replacesCurrentHistoryItem, bool isClientRedirect)
{
    if (request.url().isEmpty() || substituteData.isValid())
        return true;

    if (shouldCheckMainWorldContentSecurityPolicy == CheckContentSecurityPolicy) {
        Frame* parentFrame = m_frame->tree().parent();
        if (parentFrame) {
            ContentSecurityPolicy* parentPolicy = parentFrame->securityContext()->contentSecurityPolicy();
            ContentSecurityPolicy::RedirectStatus redirectStatus = request.followedRedirect()
                ? ContentSecurityPolicy::DidRedirect
                : ContentSecurityPolicy::DidNotRedirect;
            if (!parentPolicy->allowChildFrameFromSource(request.url(), redirectStatus)) {
                m_frame->document()->enforceSandboxFlags(SandboxOrigin);
                m_frame->owner()->dispatchLoad();
                return false;
            }
        }
    }

    bool isFormSubmission = type == NavigationTypeFormSubmitted || type == NavigationTypeFormResubmitted;
    if (isFormSubmission && !m_frame->document()->contentSecurityPolicy()->allowFormAction(request.url()))
        return false;

    policy = client()->decidePolicyForNavigation(request, loader, type, policy, replacesCurrentHistoryItem, isClientRedirect);
    if (policy == NavigationPolicyCurrentTab)
        return true;
    if (policy == NavigationPolicyIgnore)
        return false;
    if (policy == NavigationPolicyHandledByClient) {
        m_progressTracker->progressStarted();
        return false;
    }
    if (!LocalDOMWindow::allowPopUp(*m_frame) && !UserGestureIndicator::utilizeUserGesture())
        return false;
    client()->loadURLExternally(request, policy, String(), replacesCurrentHistoryItem);
    return false;
}
