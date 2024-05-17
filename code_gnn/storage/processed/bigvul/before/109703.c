void Document::implicitClose()
{
    ASSERT(!inStyleRecalc());

    bool wasLocationChangePending = frame() && frame()->navigationScheduler()->locationChangePending();
    bool doload = !parsing() && m_parser && !processingLoadEvent() && !wasLocationChangePending;

    m_loadEventProgress = LoadEventTried;

    if (!doload)
        return;

    RefPtr<DOMWindow> protect(this->domWindow());

    m_loadEventProgress = LoadEventInProgress;

    ScriptableDocumentParser* parser = scriptableDocumentParser();
    m_wellFormed = parser && parser->wellFormed();

    detachParser();

    Frame* f = frame();
    if (f && !RuntimeEnabledFeatures::webAnimationsCSSEnabled())
        f->animation()->resumeAnimationsForDocument(this);

    if (f && f->script()->canExecuteScripts(NotAboutToExecuteScript)) {
        ImageLoader::dispatchPendingBeforeLoadEvents();
        ImageLoader::dispatchPendingLoadEvents();
        ImageLoader::dispatchPendingErrorEvents();

        HTMLLinkElement::dispatchPendingLoadEvents();
        HTMLStyleElement::dispatchPendingLoadEvents();
    }

    if (svgExtensions())
        accessSVGExtensions()->dispatchSVGLoadEventToOutermostSVGElements();

    dispatchWindowLoadEvent();
    enqueuePageshowEvent(PageshowEventNotPersisted);
    enqueuePopstateEvent(m_pendingStateObject ? m_pendingStateObject.release() : SerializedScriptValue::nullValue());

    if (frame()) {
        frame()->loader()->client()->dispatchDidHandleOnloadEvents();
        loader()->applicationCacheHost()->stopDeferringEvents();
    }

    if (!frame()) {
        m_loadEventProgress = LoadEventCompleted;
        return;
    }


    if (frame()->navigationScheduler()->locationChangePending() && elapsedTime() < cLayoutScheduleThreshold) {
        m_loadEventProgress = LoadEventCompleted;
        view()->unscheduleRelayout();
        return;
    }

    RenderObject* renderObject = renderer();

    m_overMinimumLayoutThreshold = true;
    if (!ownerElement() || (ownerElement()->renderer() && !ownerElement()->renderer()->needsLayout())) {
        updateStyleIfNeeded();

        if (view() && renderObject && (!renderObject->firstChild() || renderObject->needsLayout()))
            view()->layout();
    }

    m_loadEventProgress = LoadEventCompleted;

    if (f && renderObject && AXObjectCache::accessibilityEnabled()) {
        if (AXObjectCache* cache = axObjectCache()) {
            cache->getOrCreate(renderObject);
            if (this == topDocument()) {
                cache->postNotification(renderObject, AXObjectCache::AXLoadComplete, true);
            } else {
                cache->postNotification(renderObject, AXObjectCache::AXLayoutComplete, true);
            }
        }
    }

    if (svgExtensions())
        accessSVGExtensions()->startAnimations();
}
