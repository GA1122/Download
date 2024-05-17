void Document::implicitClose()
{
    if (m_inStyleRecalc) {
        m_closeAfterStyleRecalc = true;
        return;
    }

    bool wasLocationChangePending = frame() && frame()->navigationScheduler()->locationChangePending();
    bool doload = !parsing() && m_parser && !m_processingLoadEvent && !wasLocationChangePending;
    
    if (!doload)
        return;

    RefPtr<Document> protect(this);

    m_processingLoadEvent = true;

    ScriptableDocumentParser* parser = scriptableDocumentParser();
    m_wellFormed = parser && parser->wellFormed();

    detachParser();

    Frame* f = frame();
    if (f) {
        f->loader()->icon()->startLoader();
        f->animation()->resumeAnimationsForDocument(this);
    }

    ImageLoader::dispatchPendingBeforeLoadEvents();
    ImageLoader::dispatchPendingLoadEvents();
    ImageLoader::dispatchPendingErrorEvents();

    HTMLLinkElement::dispatchPendingLoadEvents();
    HTMLStyleElement::dispatchPendingLoadEvents();

#if ENABLE(SVG)
    if (svgExtensions())
        accessSVGExtensions()->dispatchSVGLoadEventToOutermostSVGElements();
#endif

    dispatchWindowLoadEvent();
    enqueuePageshowEvent(PageshowEventNotPersisted);
    enqueuePopstateEvent(m_pendingStateObject ? m_pendingStateObject.release() : SerializedScriptValue::nullValue());
    
    if (f)
        f->loader()->handledOnloadEvents();
#ifdef INSTRUMENT_LAYOUT_SCHEDULING
    if (!ownerElement())
        printf("onload fired at %d\n", elapsedTime());
#endif

    if (!frame()) {
        m_processingLoadEvent = false;
        return;
    }


    if (frame()->navigationScheduler()->locationChangePending() && elapsedTime() < cLayoutScheduleThreshold) {
        m_processingLoadEvent = false;
        view()->unscheduleRelayout();
        return;
    }

    frame()->loader()->checkCallImplicitClose();
    RenderObject* renderObject = renderer();
    
    m_overMinimumLayoutThreshold = true;
    if (!ownerElement() || (ownerElement()->renderer() && !ownerElement()->renderer()->needsLayout())) {
        updateStyleIfNeeded();
        
        if (view() && renderObject && (!renderObject->firstChild() || renderObject->needsLayout()))
            view()->layout();
    }

    m_processingLoadEvent = false;

#if PLATFORM(MAC) || PLATFORM(CHROMIUM)
    if (f && renderObject && AXObjectCache::accessibilityEnabled()) {
        axObjectCache()->getOrCreate(renderObject);
        if (this == topDocument())
            axObjectCache()->postNotification(renderObject, AXObjectCache::AXLoadComplete, true);
        else {
            axObjectCache()->postNotification(renderObject, AXObjectCache::AXLayoutComplete, true);
        }
    }
#endif

#if ENABLE(SVG)
    if (svgExtensions())
        accessSVGExtensions()->startAnimations();
#endif
}
