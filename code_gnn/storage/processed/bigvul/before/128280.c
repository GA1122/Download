void FrameView::layout(bool allowSubtree)
{
    ASSERT(m_frame);
    ASSERT(m_frame->view() == this);
    ASSERT(m_frame->page());

    ScriptForbiddenScope forbidScript;

    if (isInPerformLayout() || !m_frame->document()->isActive())
        return;

    TRACE_EVENT0("blink", "FrameView::layout");
    TRACE_EVENT_SCOPED_SAMPLING_STATE("blink", "Layout");

    RefPtr<FrameView> protector(this);

    TemporaryChange<bool> changeInProgrammaticScroll(m_inProgrammaticScroll, true);

    m_hasPendingLayout = false;
    DocumentLifecycle::Scope lifecycleScope(lifecycle(), DocumentLifecycle::LayoutClean);

    RELEASE_ASSERT(!isPainting());

    TRACE_EVENT_BEGIN1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline"), "Layout", "beginData", InspectorLayoutEvent::beginData(this));
    TRACE_EVENT_INSTANT1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline.stack"), "CallStack", "stack", InspectorCallStackEvent::currentCallStack());
    InspectorInstrumentationCookie cookie = InspectorInstrumentation::willLayout(m_frame.get());

    if (!allowSubtree && isSubtreeLayout()) {
        m_layoutSubtreeRoot->markContainingBlocksForLayout(false);
        m_layoutSubtreeRoot = 0;
    }

    performPreLayoutTasks();

    if (protector->hasOneRef())
        return;

    Document* document = m_frame->document();
    bool inSubtreeLayout = isSubtreeLayout();
    RenderObject* rootForThisLayout = inSubtreeLayout ? m_layoutSubtreeRoot : document->renderView();
    if (!rootForThisLayout) {
        ASSERT_NOT_REACHED();
        return;
    }

    FontCachePurgePreventer fontCachePurgePreventer;
    RenderLayer* layer;
    {
        TemporaryChange<bool> changeSchedulingEnabled(m_layoutSchedulingEnabled, false);

        m_nestedLayoutCount++;
        if (!inSubtreeLayout) {
            Document* document = m_frame->document();
            Node* body = document->body();
            if (body && body->renderer()) {
                if (isHTMLFrameSetElement(*body)) {
                    body->renderer()->setChildNeedsLayout();
                } else if (isHTMLBodyElement(*body)) {
                    if (!m_firstLayout && m_size.height() != layoutSize().height() && body->renderer()->enclosingBox()->stretchesToViewport())
                        body->renderer()->setChildNeedsLayout();
                }
            }
        }
        updateCounters();
        if (m_autoSizeInfo)
            m_autoSizeInfo->autoSizeIfNeeded();

        ScrollbarMode hMode;
        ScrollbarMode vMode;
        calculateScrollbarModesForLayoutAndSetViewportRenderer(hMode, vMode);

        if (!inSubtreeLayout) {
            ScrollbarMode currentHMode = horizontalScrollbarMode();
            ScrollbarMode currentVMode = verticalScrollbarMode();

            if (m_firstLayout) {
                setScrollbarsSuppressed(true);

                m_doFullPaintInvalidation = true;
                m_firstLayout = false;
                m_firstLayoutCallbackPending = true;
                m_lastViewportSize = layoutSize(IncludeScrollbars);
                m_lastZoomFactor = rootForThisLayout->style()->zoom();

                if (vMode == ScrollbarAuto)
                    setVerticalScrollbarMode(ScrollbarAlwaysOn);  
                if (hMode == ScrollbarAuto)
                    setHorizontalScrollbarMode(ScrollbarAlwaysOff);  

                setScrollbarModes(hMode, vMode);
                setScrollbarsSuppressed(false, true);
            } else if (hMode != currentHMode || vMode != currentVMode) {
                setScrollbarModes(hMode, vMode);
            }

            LayoutSize oldSize = m_size;

            m_size = LayoutSize(layoutSize().width(), layoutSize().height());

            if (oldSize != m_size && !m_firstLayout) {
                RenderBox* rootRenderer = document->documentElement() ? document->documentElement()->renderBox() : 0;
                RenderBox* bodyRenderer = rootRenderer && document->body() ? document->body()->renderBox() : 0;
                if (bodyRenderer && bodyRenderer->stretchesToViewport())
                    bodyRenderer->setChildNeedsLayout();
                else if (rootRenderer && rootRenderer->stretchesToViewport())
                    rootRenderer->setChildNeedsLayout();
            }

            m_doFullPaintInvalidation |= renderView()->shouldDoFullPaintInvalidationForNextLayout();
        }

        layer = rootForThisLayout->enclosingLayer();

        performLayout(rootForThisLayout, inSubtreeLayout);

        m_layoutSubtreeRoot = 0;
        if (RenderObject* container = rootForThisLayout->container())
            container->setMayNeedPaintInvalidation(true);
    }  

    if (!inSubtreeLayout && !toRenderView(rootForThisLayout)->document().printing())
        adjustViewSize();

    layer->updateLayerPositionsAfterLayout();

    if (m_doFullPaintInvalidation)
        renderView()->compositor()->fullyInvalidatePaint();
    renderView()->compositor()->didLayout();

    m_layoutCount++;

    if (AXObjectCache* cache = rootForThisLayout->document().axObjectCache()) {
        const KURL& url = rootForThisLayout->document().url();
        if (url.isValid() && !url.isAboutBlankURL())
            cache->handleLayoutComplete(rootForThisLayout);
    }
    updateAnnotatedRegions();

    ASSERT(!rootForThisLayout->needsLayout());

    if (document->hasListenerType(Document::OVERFLOWCHANGED_LISTENER))
        updateOverflowStatus(layoutSize().width() < contentsWidth(), layoutSize().height() < contentsHeight());

    scheduleOrPerformPostLayoutTasks();

    TRACE_EVENT_END1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline"), "Layout", "endData", InspectorLayoutEvent::endData(rootForThisLayout));
    InspectorInstrumentation::didLayout(cookie, rootForThisLayout);

    m_nestedLayoutCount--;
    if (m_nestedLayoutCount)
        return;

#if ENABLE(ASSERT)
    document->renderView()->assertSubtreeIsLaidOut();
#endif

    if (frame().page())
        frame().page()->chrome().client().layoutUpdated(m_frame.get());
}
