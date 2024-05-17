Frame* InspectorPageAgent::findFrameWithSecurityOrigin(const String& originRawString)
{
    for (Frame* frame = m_page->mainFrame(); frame; frame = frame->tree().traverseNext()) {
        RefPtr<SecurityOrigin> documentOrigin = frame->document()->securityOrigin();
        if (documentOrigin->toRawString() == originRawString)
            return frame;
    }
    return 0;
}
