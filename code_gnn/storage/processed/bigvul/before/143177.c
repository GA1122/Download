void Document::dispatchUnloadEvents()
{
    PluginScriptForbiddenScope forbidPluginDestructorScripting;
    if (m_parser)
        m_parser->stopParsing();

    if (m_loadEventProgress == LoadEventNotRun)
        return;

    if (m_loadEventProgress <= UnloadEventInProgress) {
        Element* currentFocusedElement = focusedElement();
        if (isHTMLInputElement(currentFocusedElement))
            toHTMLInputElement(*currentFocusedElement).endEditing();
        if (m_loadEventProgress < PageHideInProgress) {
            m_loadEventProgress = PageHideInProgress;
            if (LocalDOMWindow* window = domWindow())
                window->dispatchEvent(PageTransitionEvent::create(EventTypeNames::pagehide, false), this);
            if (!m_frame)
                return;

            PageVisibilityState visibilityState = pageVisibilityState();
            m_loadEventProgress = UnloadVisibilityChangeInProgress;
            if (visibilityState != PageVisibilityStateHidden && RuntimeEnabledFeatures::visibilityChangeOnUnloadEnabled()) {
                dispatchEvent(Event::createBubble(EventTypeNames::visibilitychange));
                dispatchEvent(Event::createBubble(EventTypeNames::webkitvisibilitychange));
            }
            if (!m_frame)
                return;

            DocumentLoader* documentLoader = m_frame->loader().provisionalDocumentLoader();
            m_loadEventProgress = UnloadEventInProgress;
            Event* unloadEvent(Event::create(EventTypeNames::unload));
            if (documentLoader && !documentLoader->timing().unloadEventStart() && !documentLoader->timing().unloadEventEnd()) {
                DocumentLoadTiming& timing = documentLoader->timing();
                DCHECK(timing.navigationStart());
                timing.markUnloadEventStart();
                m_frame->localDOMWindow()->dispatchEvent(unloadEvent, this);
                timing.markUnloadEventEnd();
            } else {
                m_frame->localDOMWindow()->dispatchEvent(unloadEvent, m_frame->document());
            }
        }
        m_loadEventProgress = UnloadEventHandled;
    }

    if (!m_frame)
        return;

    bool keepEventListeners = m_frame->loader().provisionalDocumentLoader()
        && m_frame->shouldReuseDefaultView(m_frame->loader().provisionalDocumentLoader()->url());
    if (!keepEventListeners)
        removeAllEventListenersRecursively();
}
