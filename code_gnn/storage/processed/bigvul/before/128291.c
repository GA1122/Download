void FrameView::paintContents(GraphicsContext* p, const IntRect& rect)
{
    Document* document = m_frame->document();

#ifndef NDEBUG
    bool fillWithRed;
    if (document->printing())
        fillWithRed = false;  
    else if (m_frame->owner())
        fillWithRed = false;  
    else if (isTransparent())
        fillWithRed = false;  
    else if (m_paintBehavior & PaintBehaviorSelectionOnly)
        fillWithRed = false;  
    else if (m_nodeToDraw)
        fillWithRed = false;  
    else
        fillWithRed = true;

    if (fillWithRed)
        p->fillRect(rect, Color(0xFF, 0, 0));
#endif

    RenderView* renderView = this->renderView();
    if (!renderView) {
        WTF_LOG_ERROR("called FrameView::paint with nil renderer");
        return;
    }

    RELEASE_ASSERT(!needsLayout());
    ASSERT(document->lifecycle().state() >= DocumentLifecycle::CompositingClean);

    TRACE_EVENT1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline"), "Paint", "data", InspectorPaintEvent::data(renderView, rect, 0));
    TRACE_EVENT_INSTANT1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline.stack"), "CallStack", "stack", InspectorCallStackEvent::currentCallStack());
    InspectorInstrumentation::willPaint(renderView, 0);

    bool isTopLevelPainter = !s_inPaintContents;
    s_inPaintContents = true;

    FontCachePurgePreventer fontCachePurgePreventer;

    PaintBehavior oldPaintBehavior = m_paintBehavior;

    if (FrameView* parentView = parentFrameView()) {
        if (parentView->paintBehavior() & PaintBehaviorFlattenCompositingLayers)
            m_paintBehavior |= PaintBehaviorFlattenCompositingLayers;
    }

    if (m_paintBehavior == PaintBehaviorNormal)
        document->markers().invalidateRenderedRectsForMarkersInRect(rect);

    if (document->printing())
        m_paintBehavior |= PaintBehaviorFlattenCompositingLayers;

    ASSERT(!m_isPainting);
    m_isPainting = true;

    RenderObject* renderer = m_nodeToDraw ? m_nodeToDraw->renderer() : 0;
    RenderLayer* rootLayer = renderView->layer();

#if ENABLE(ASSERT)
    renderView->assertSubtreeIsLaidOut();
    RenderObject::SetLayoutNeededForbiddenScope forbidSetNeedsLayout(*rootLayer->renderer());
#endif

    rootLayer->paint(p, rect, m_paintBehavior, renderer);

    if (rootLayer->containsDirtyOverlayScrollbars())
        rootLayer->paintOverlayScrollbars(p, rect, m_paintBehavior, renderer);

    m_isPainting = false;

    m_paintBehavior = oldPaintBehavior;
    m_lastPaintTime = currentTime();

    if (document->annotatedRegionsDirty())
        updateAnnotatedRegions();

    if (isTopLevelPainter) {
        s_currentFrameTimeStamp = currentTime();
        s_inPaintContents = false;
    }

    InspectorInstrumentation::didPaint(renderView, 0, p, rect);
}
