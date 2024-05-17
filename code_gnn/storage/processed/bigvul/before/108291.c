void FrameLoader::urlSelected(const ResourceRequest& request, const String& passedTarget, PassRefPtr<Event> triggeringEvent, bool lockHistory, bool lockBackForwardList, bool userGesture, ReferrerPolicy referrerPolicy, ShouldReplaceDocumentIfJavaScriptURL shouldReplaceDocumentIfJavaScriptURL)
{
    ASSERT(!m_suppressOpenerInNewFrame);

    if (m_frame->script()->executeIfJavaScriptURL(request.url(), userGesture, shouldReplaceDocumentIfJavaScriptURL))
        return;

    String target = passedTarget;
    if (target.isEmpty())
        target = m_frame->document()->baseTarget();

    FrameLoadRequest frameRequest(request, target);

    if (referrerPolicy == NoReferrer)
        m_suppressOpenerInNewFrame = true;
    else if (frameRequest.resourceRequest().httpReferrer().isEmpty())
        frameRequest.resourceRequest().setHTTPReferrer(m_outgoingReferrer);
    addHTTPOriginIfNeeded(frameRequest.resourceRequest(), outgoingOrigin());

    loadFrameRequest(frameRequest, lockHistory, lockBackForwardList, triggeringEvent, 0, referrerPolicy);

    m_suppressOpenerInNewFrame = false;
}
