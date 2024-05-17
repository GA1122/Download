void FrameLoaderClient::dispatchDecidePolicyForMIMEType(FramePolicyFunction policyFunction, const String& mimeType, const ResourceRequest& resourceRequest)
{
    ASSERT(policyFunction);
    if (!policyFunction)
        return;

    if (resourceRequest.isNull()) {
        (core(m_frame)->loader()->policyChecker()->*policyFunction)(PolicyIgnore);
        return;
    }

    WebKitWebView* page = getViewFromFrame(m_frame);
    WebKitNetworkRequest* request = webkit_network_request_new_with_core_request(resourceRequest);

    WebKitWebPolicyDecision* policyDecision = webkit_web_policy_decision_new(m_frame, policyFunction);
    if (m_policyDecision)
        g_object_unref(m_policyDecision);
    m_policyDecision = policyDecision;

    gboolean isHandled = false;
    g_signal_emit_by_name(page, "mime-type-policy-decision-requested", m_frame, request, mimeType.utf8().data(), policyDecision, &isHandled);

    g_object_unref(request);

    if (isHandled)
        return;

    if (canShowMIMEType(mimeType))
        webkit_web_policy_decision_use (policyDecision);
    else
        webkit_web_policy_decision_ignore (policyDecision);
}
