void Document::updateLayoutTree(StyleRecalcChange change)
{
    ASSERT(isMainThread());

    ScriptForbiddenScope forbidScript;

    if (!view() || !isActive())
        return;

    if (change != Force && !needsLayoutTreeUpdate())
        return;

    if (inStyleRecalc())
        return;

    RELEASE_ASSERT(!view()->isInPerformLayout());
    RELEASE_ASSERT(!view()->isPainting());

    RefPtrWillBeRawPtr<LocalFrame> protect(m_frame.get());

    TRACE_EVENT_BEGIN0("blink", "Document::updateLayoutTree");
    TRACE_EVENT_SCOPED_SAMPLING_STATE("blink", "UpdateLayoutTree");

    m_styleRecalcElementCounter = 0;
    TRACE_EVENT_BEGIN1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline"), "RecalculateStyles", "beginData", InspectorRecalculateStylesEvent::data(frame()));
    InspectorInstrumentationCookie cookie = InspectorInstrumentation::willRecalculateStyle(this);

    DocumentAnimations::updateAnimationTimingIfNeeded(*this);
    evaluateMediaQueryListIfNeeded();
    updateUseShadowTreesIfNeeded();
    updateDistribution();
    updateStyleInvalidationIfNeeded();


    if (m_elemSheet && m_elemSheet->contents()->usesRemUnits())
        styleEngine().setUsesRemUnit(true);

    updateStyle(change);

    notifyLayoutTreeOfSubtreeChanges();

    if (hoverNode() && !hoverNode()->layoutObject() && frame())
        frame()->eventHandler().dispatchFakeMouseMoveEventSoon();

    if (m_focusedElement && !m_focusedElement->isFocusable())
        clearFocusedElementSoon();

    ASSERT(!m_timeline->hasOutdatedAnimation());

    TRACE_EVENT_END1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline"), "RecalculateStyles", "elementCount", m_styleRecalcElementCounter);
    TRACE_EVENT_END1("blink", "Document::updateLayoutTree", "elementCount", m_styleRecalcElementCounter);
    InspectorInstrumentation::didRecalculateStyle(cookie, m_styleRecalcElementCounter);
}
