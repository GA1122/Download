void Document::implicitClose()
{
    ASSERT(!inStyleRecalc());
    if (processingLoadEvent() || !m_parser)
        return;
    if (frame() && frame()->navigationScheduler().locationChangePending()) {
        suppressLoadEvent();
        return;
    }

    RefPtrWillBeRawPtr<LocalDOMWindow> protectedWindow(this->domWindow());

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

    if (protectedWindow)
        protectedWindow->documentWasClosed();

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

    if (!ownerElement() || (ownerElement()->layoutObject() && !ownerElement()->layoutObject()->needsLayout())) {
        updateLayoutTreeIfNeeded();

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
