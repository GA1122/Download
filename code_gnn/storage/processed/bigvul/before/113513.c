void WebPagePrivate::setLoadState(LoadState state)
{
    if (m_loadState == state)
        return;

    bool isFirstLoad = m_loadState == None;

    if (state == Finished && m_mainFrame && m_mainFrame->document())
        m_mainFrame->document()->updateStyleIfNeeded();

    m_backingStore->d->setWebPageBackgroundColor(m_mainFrame && m_mainFrame->view()
        ? m_mainFrame->view()->documentBackgroundColor()
        : m_webSettings->backgroundColor());

    m_loadState = state;

#if DEBUG_WEBPAGE_LOAD
    Platform::logAlways(Platform::LogLevelInfo, "WebPagePrivate::setLoadState %d", state);
#endif

    switch (m_loadState) {
    case Provisional:
        if (isFirstLoad) {
            m_backingStore->d->renderAndBlitVisibleContentsImmediately();
        }
        break;
    case Committed:
        {
#if ENABLE(ACCELERATED_2D_CANVAS)
            if (m_page->settings()->canvasUsesAcceleratedDrawing()) {
                SharedGraphicsContext3D::get()->makeContextCurrent();
                GrContext* grContext = Platform::Graphics::getGrContext();
                grContext->freeGpuResources();
            }
#endif

#if USE(ACCELERATED_COMPOSITING)
            releaseLayerResources();
#endif

            m_backingStore->d->suspendBackingStoreUpdates();
            m_backingStore->d->suspendScreenUpdates();

            m_previousContentsSize = IntSize();
            m_backingStore->d->resetRenderQueue();
            m_backingStore->d->resetTiles();
            m_backingStore->d->setScrollingOrZooming(false, false  );
            m_shouldZoomToInitialScaleAfterLoadFinished = false;
            m_userPerformedManualZoom = false;
            m_userPerformedManualScroll = false;
            m_shouldUseFixedDesktopMode = false;
            m_forceRespectViewportArguments = false;
            if (m_resetVirtualViewportOnCommitted)  
                m_virtualViewportSize = IntSize();
            if (m_webSettings->viewportWidth() > 0)
                m_virtualViewportSize = IntSize(m_webSettings->viewportWidth(), m_defaultLayoutSize.height());

            static ViewportArguments defaultViewportArguments;
            bool documentHasViewportArguments = false;
            if (m_mainFrame && m_mainFrame->document() && m_mainFrame->document()->viewportArguments() != defaultViewportArguments)
                documentHasViewportArguments = true;
            if (!(m_didRestoreFromPageCache && documentHasViewportArguments)) {
                m_viewportArguments = ViewportArguments();
                m_userScalable = m_webSettings->isUserScalable();
                resetScales();

                dispatchViewportPropertiesDidChange(m_userViewportArguments);
                if (m_userViewportArguments != defaultViewportArguments)
                    m_forceRespectViewportArguments = true;
            } else {
                Platform::IntSize virtualViewport = recomputeVirtualViewportFromViewportArguments();
                m_webPage->setVirtualViewportSize(virtualViewport);
                if (m_shouldUseFixedDesktopMode)
                    setViewMode(FixedDesktop);
                else
                    setViewMode(Desktop);
            }

#if ENABLE(EVENT_MODE_METATAGS)
            didReceiveCursorEventMode(ProcessedCursorEvents);
            didReceiveTouchEventMode(ProcessedTouchEvents);
#endif

            resetBlockZoom();
#if ENABLE(VIEWPORT_REFLOW)
            toggleTextReflowIfEnabledForBlockZoomOnly();
#endif

            m_inputHandler->setInputModeEnabled(false);

            setScrollPosition(IntPoint::zero());
            notifyTransformedScrollChanged();

            m_backingStore->d->resumeBackingStoreUpdates();
            m_backingStore->d->resumeScreenUpdates(BackingStore::RenderAndBlit);

            updateCursor();

            break;
        }
    case Finished:
    case Failed:
        m_client->scaleChanged();
        m_backingStore->d->updateTiles(true  , false  );
        break;
    default:
        break;
    }
}
