void FrameLoaderClient::dispatchDecidePolicyForNavigationAction(FramePolicyFunction policyFunction, const NavigationAction& action, const ResourceRequest& resourceRequest, PassRefPtr<FormState>)
{
    ASSERT(policyFunction);
    if (!policyFunction)
        return;

    if (resourceRequest.isNull()) {
        (core(m_frame)->loader()->policyChecker()->*policyFunction)(PolicyIgnore);
        return;
    }

    WebKitWebView* webView = getViewFromFrame(m_frame);
    WebKitNetworkRequest* request = webkit_network_request_new_with_core_request(resourceRequest);
    WebKitNavigationResponse response;
     
    g_signal_emit_by_name(webView, "navigation-requested", m_frame, request, &response);

    if (response == WEBKIT_NAVIGATION_RESPONSE_IGNORE) {
        (core(m_frame)->loader()->policyChecker()->*policyFunction)(PolicyIgnore);
        g_object_unref(request);
        return;
    }

    WebKitWebPolicyDecision* policyDecision = webkit_web_policy_decision_new(m_frame, policyFunction);
    if (m_policyDecision)
        g_object_unref(m_policyDecision);
    m_policyDecision = policyDecision;

    WebKitWebNavigationAction* navigationAction = getNavigationAction(action, NULL);
    gboolean isHandled = false;
    g_signal_emit_by_name(webView, "navigation-policy-decision-requested", m_frame, request, navigationAction, policyDecision, &isHandled);

    g_object_unref(navigationAction);
    g_object_unref(request);

    if (!isHandled)
        webkit_web_policy_decision_use(m_policyDecision);
}
