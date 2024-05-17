void Document::processHttpEquivXFrameOptions(const String& content)
{
    Frame* frame = this->frame();
    if (!frame)
        return;

    FrameLoader* frameLoader = frame->loader();
    unsigned long requestIdentifier = loader()->mainResourceIdentifier();
    if (frameLoader->shouldInterruptLoadForXFrameOptions(content, url(), requestIdentifier)) {
        String message = "Refused to display '" + url().elidedString() + "' in a frame because it set 'X-Frame-Options' to '" + content + "'.";
        frameLoader->stopAllLoaders();
        frame->navigationScheduler()->scheduleLocationChange(securityOrigin(), SecurityOrigin::urlWithUniqueSecurityOrigin(), String());
        addConsoleMessageWithRequestIdentifier(SecurityMessageSource, ErrorMessageLevel, message, requestIdentifier);
    }
}
