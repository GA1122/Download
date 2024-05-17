void Document::processHttpEquivXFrameOptions(const AtomicString& content)
{
    if (!m_frame)
        return;

    unsigned long requestIdentifier = loader()->mainResourceIdentifier();
    if (m_frame->loader().shouldInterruptLoadForXFrameOptions(content, url(), requestIdentifier)) {
        String message = "Refused to display '" + url().elidedString() + "' in a frame because it set 'X-Frame-Options' to '" + content + "'.";
        m_frame->loader().stopAllLoaders();
        if (!m_frame)
            return;
        m_frame->navigate(*this, SecurityOrigin::urlWithUniqueSecurityOrigin(), true, UserGestureStatus::None);
        RefPtrWillBeRawPtr<ConsoleMessage> consoleMessage = ConsoleMessage::create(SecurityMessageSource, ErrorMessageLevel, message);
        consoleMessage->setRequestIdentifier(requestIdentifier);
        addConsoleMessage(consoleMessage.release());
    }
}
