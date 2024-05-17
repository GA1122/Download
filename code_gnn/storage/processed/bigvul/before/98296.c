void FrameLoaderClient::dispatchDecidePolicyForNewWindowAction(FramePolicyFunction policyFunction, const NavigationAction& action, const ResourceRequest& resourceRequest, PassRefPtr<FormState>, const String& frameName)
{
    ASSERT(policyFunction);
    if (!policyFunction)
        return;

    if (resourceRequest.isNull()) {
        (core(m_frame)->loader()->policyChecker()->*policyFunction)(PolicyIgnore);
        return;
    }

    WebKitWebPolicyDecision* policyDecision = webkit_web_policy_decision_new(m_frame, policyFunction);

    if (m_policyDecision)
        g_object_unref(m_policyDecision);
    m_policyDecision = policyDecision;

    WebKitWebView* webView = getViewFromFrame(m_frame);
    WebKitNetworkRequest* request = webkit_network_request_new(resourceRequest.url().string().utf8().data());
    WebKitWebNavigationAction* navigationAction = getNavigationAction(action, frameName.utf8().data());
    gboolean isHandled = false;

    g_signal_emit_by_name(webView, "new-window-policy-decision-requested", m_frame, request, navigationAction, policyDecision, &isHandled);

    g_object_unref(navigationAction);
    g_object_unref(request);

    if (!isHandled)
        (core(m_frame)->loader()->policyChecker()->*policyFunction)(PolicyUse);
}
