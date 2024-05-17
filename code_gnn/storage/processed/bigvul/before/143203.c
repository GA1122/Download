void Document::implicitClose()
{
    DCHECK(!inStyleRecalc());
    if (processingLoadEvent() || !m_parser)
        return;
    if (frame() && frame()->navigationScheduler().locationChangePending()) {
        suppressLoadEvent();
        return;
    }

    m_loadEventProgress = LoadEventInProgress;

    ScriptableDocumentParser* parser = scriptableDocumentParser();
    m_wellFormed = parser && parser->wellFormed();

    detachParser();

    if (frame() && frame()->script().canExecuteScripts(NotAboutToExecuteScript)) {
        ImageLoader::dispatchPendingLoadEvents();
        ImageLoader::dispatchPendingErrorEvents();

        HTMLLinkElement::dispatchPendingLoadEvents();
        HTMLStyleElement::dispatchPendingLoadEvents();
    }


    if (svgExtensions())
        accessSVGExtensions().dispatchSVGLoadEventToOutermostSVGElements();

    if (this->domWindow())
        this->domWindow()->documentWasClosed();

    if (frame()) {
        frame()->loader().client()->dispatchDidHandleOnloadEvents();
        loader()->applicationCacheHost()->stopDeferringEvents();
    }

    if (!frame()) {
        m_loadEventProgress = LoadEventCompleted;
        return;
    }


    if (frame()->navigationScheduler().locationChangePending() && elapsedTime() < cLayoutScheduleThreshold) {
        m_loadEventProgress = LoadEventCompleted;
        return;
    }

    if (!localOwner() || (localOwner()->layoutObject() && !localOwner()->layoutObject()->needsLayout())) {
        updateStyleAndLayoutTree();

        if (view() && layoutView() && (!layoutView()->firstChild() || layoutView()->needsLayout()))
            view()->layout();
    }

    m_loadEventProgress = LoadEventCompleted;

    if (frame() && layoutView() && settings()->accessibilityEnabled()) {
        if (AXObjectCache* cache = axObjectCache()) {
            if (this == &axObjectCacheOwner())
                cache->handleLoadComplete(this);
            else
                cache->handleLayoutComplete(this);
        }
    }

    if (svgExtensions())
        accessSVGExtensions().startAnimations();
}
