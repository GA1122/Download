void Document::dispatchUnloadEvents()
{
    RefPtr<Document> protect(this);
    if (m_parser)
        m_parser->stopParsing();

    if (m_loadEventProgress >= LoadEventTried && m_loadEventProgress <= UnloadEventInProgress) {
        Element* currentFocusedElement = focusedElement();
        if (currentFocusedElement && currentFocusedElement->hasTagName(inputTag))
            toHTMLInputElement(currentFocusedElement)->endEditing();
        if (m_loadEventProgress < PageHideInProgress) {
            m_loadEventProgress = PageHideInProgress;
            dispatchWindowEvent(PageTransitionEvent::create(EventTypeNames::pagehide, false), this);
            if (!m_frame)
                return;

            RefPtr<DocumentLoader> documentLoader =  m_frame->loader()->provisionalDocumentLoader();
            m_loadEventProgress = UnloadEventInProgress;
            RefPtr<Event> unloadEvent(Event::create(EventTypeNames::unload));
            if (documentLoader && !documentLoader->timing()->unloadEventStart() && !documentLoader->timing()->unloadEventEnd()) {
                DocumentLoadTiming* timing = documentLoader->timing();
                ASSERT(timing->navigationStart());
                timing->markUnloadEventStart();
                dispatchWindowEvent(unloadEvent, this);
                timing->markUnloadEventEnd();
            } else {
                m_frame->domWindow()->dispatchEvent(unloadEvent, m_frame->document());
            }
        }
        updateStyleIfNeeded();
        m_loadEventProgress = UnloadEventHandled;
    }

    if (!m_frame)
        return;

    bool keepEventListeners = m_frame->loader()->stateMachine()->isDisplayingInitialEmptyDocument() && m_frame->loader()->provisionalDocumentLoader()
        && isSecureTransitionTo(m_frame->loader()->provisionalDocumentLoader()->url());
    if (!keepEventListeners)
        removeAllEventListeners();
}
