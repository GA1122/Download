bool FrameLoader::shouldInterruptLoadForXFrameOptions(const String& content, const KURL& url, unsigned long requestIdentifier)
{
    UseCounter::count(m_frame->domWindow()->document(), UseCounter::XFrameOptions);

    Frame* topFrame = m_frame->tree().top();
    if (m_frame == topFrame)
        return false;

    XFrameOptionsDisposition disposition = parseXFrameOptionsHeader(content);

    switch (disposition) {
    case XFrameOptionsSameOrigin: {
        UseCounter::count(m_frame->domWindow()->document(), UseCounter::XFrameOptionsSameOrigin);
        RefPtr<SecurityOrigin> origin = SecurityOrigin::create(url);
        if (!topFrame->isLocalFrame() || !origin->isSameSchemeHostPort(toLocalFrame(topFrame)->document()->getSecurityOrigin()))
            return true;
        for (Frame* frame = m_frame->tree().parent(); frame; frame = frame->tree().parent()) {
            if (!frame->isLocalFrame() || !origin->isSameSchemeHostPort(toLocalFrame(frame)->document()->getSecurityOrigin())) {
                UseCounter::count(m_frame->domWindow()->document(), UseCounter::XFrameOptionsSameOriginWithBadAncestorChain);
                break;
            }
        }
        return false;
    }
    case XFrameOptionsDeny:
        return true;
    case XFrameOptionsAllowAll:
        return false;
    case XFrameOptionsConflict: {
        ConsoleMessage* consoleMessage = ConsoleMessage::createForRequest(JSMessageSource, ErrorMessageLevel, "Multiple 'X-Frame-Options' headers with conflicting values ('" + content + "') encountered when loading '" + url.elidedString() + "'. Falling back to 'DENY'.", url, requestIdentifier);
        m_frame->document()->addConsoleMessage(consoleMessage);
        return true;
    }
    case XFrameOptionsInvalid: {
        ConsoleMessage* consoleMessage = ConsoleMessage::createForRequest(JSMessageSource, ErrorMessageLevel, "Invalid 'X-Frame-Options' header encountered when loading '" + url.elidedString() + "': '" + content + "' is not a recognized directive. The header will be ignored.", url, requestIdentifier);
        m_frame->document()->addConsoleMessage(consoleMessage);
        return false;
    }
    default:
        NOTREACHED();
        return false;
    }
}
