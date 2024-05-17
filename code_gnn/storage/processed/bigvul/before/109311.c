void InspectorPageAgent::navigate(ErrorString*, const String& url)
{
    UserGestureIndicator indicator(DefinitelyProcessingNewUserGesture);
    Frame* frame = m_page->mainFrame();
    FrameLoadRequest request(frame->document()->securityOrigin(), ResourceRequest(frame->document()->completeURL(url)));
    frame->loader().load(request);
}
