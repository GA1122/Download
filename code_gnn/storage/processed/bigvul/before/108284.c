void FrameLoader::stopLoading(UnloadEventPolicy unloadEventPolicy, DatabasePolicy databasePolicy)
{
    if (m_frame->document() && m_frame->document()->parser())
        m_frame->document()->parser()->stopParsing();

    if (unloadEventPolicy != UnloadEventPolicyNone) {
        if (m_frame->document()) {
            if (m_didCallImplicitClose && !m_wasUnloadEventEmitted) {
                Node* currentFocusedNode = m_frame->document()->focusedNode();
                if (currentFocusedNode)
                    currentFocusedNode->aboutToUnload();
                m_pageDismissalEventBeingDispatched = true;
                if (m_frame->domWindow()) {
                    if (unloadEventPolicy == UnloadEventPolicyUnloadAndPageHide)
                        m_frame->domWindow()->dispatchEvent(PageTransitionEvent::create(eventNames().pagehideEvent, m_frame->document()->inPageCache()), m_frame->document());
                    if (!m_frame->document()->inPageCache()) {
                        m_frame->domWindow()->dispatchEvent(Event::create(eventNames().unloadEvent, false, false), m_frame->domWindow()->document());

                        if (m_provisionalDocumentLoader) {
                            DocumentLoadTiming* timing = m_provisionalDocumentLoader->timing();
                            ASSERT(timing->navigationStart);
                            timing->unloadEventEnd = currentTime();
                            ASSERT(timing->unloadEventEnd >= timing->navigationStart);
                        }
                    }
                }
                m_pageDismissalEventBeingDispatched = false;
                if (m_frame->document())
                    m_frame->document()->updateStyleIfNeeded();
                m_wasUnloadEventEmitted = true;
            }
        }

        if (m_frame->document() && !m_frame->document()->inPageCache()) {
            bool keepEventListeners = m_stateMachine.isDisplayingInitialEmptyDocument() && m_provisionalDocumentLoader
                && m_frame->document()->securityOrigin()->isSecureTransitionTo(m_provisionalDocumentLoader->url());

            if (!keepEventListeners)
                m_frame->document()->removeAllEventListeners();
        }
    }

    m_isComplete = true;  
    m_isLoadingMainResource = false;
    m_didCallImplicitClose = true;  

    if (m_frame->document() && m_frame->document()->parsing()) {
        finishedParsing();
        m_frame->document()->setParsing(false);
    }
  
    m_workingURL = KURL();

    if (Document* doc = m_frame->document()) {
        if (DocLoader* docLoader = doc->docLoader())
            cache()->loader()->cancelRequests(docLoader);

#if ENABLE(DATABASE)
        if (databasePolicy == DatabasePolicyStop)
            doc->stopDatabases(0);
#else
    UNUSED_PARAM(databasePolicy);
#endif
    }

    m_frame->redirectScheduler()->cancel();
}
