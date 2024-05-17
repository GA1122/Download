void Document::updateLayoutTree(StyleRecalcChange change)
{
    ASSERT(isMainThread());

    ScriptForbiddenScope forbidScript;

    if (!view() || !isActive())
        return;

    if (view()->shouldThrottleRendering())
        return;

    if (change != Force && !needsLayoutTreeUpdate()) {
        if (lifecycle().state() < DocumentLifecycle::StyleClean) {
            lifecycle().advanceTo(DocumentLifecycle::InStyleRecalc);
            lifecycle().advanceTo(DocumentLifecycle::StyleClean);
        }
        return;
    }

    if (inStyleRecalc())
        return;

    RELEASE_ASSERT(!view()->isInPerformLayout());
    RELEASE_ASSERT(!view()->isPainting());

    RefPtrWillBeRawPtr<LocalFrame> protect(m_frame.get());

    TRACE_EVENT_BEGIN1("blink,devtools.timeline", "UpdateLayoutTree", "beginData", InspectorRecalculateStylesEvent::data(frame()));
    TRACE_EVENT_SCOPED_SAMPLING_STATE("blink", "UpdateLayoutTree");

    unsigned startElementCount = styleEngine().styleForElementCount();

    InspectorInstrumentationCookie cookie = InspectorInstrumentation::willRecalculateStyle(this);

    DocumentAnimations::updateAnimationTimingIfNeeded(*this);
    evaluateMediaQueryListIfNeeded();
    updateUseShadowTreesIfNeeded();
    updateDistribution();
    updateStyleInvalidationIfNeeded();


    updateStyle(change);

    notifyLayoutTreeOfSubtreeChanges();

    if (hoverNode() && !hoverNode()->layoutObject() && frame())
        frame()->eventHandler().dispatchFakeMouseMoveEventSoon();

    if (m_focusedElement && !m_focusedElement->isFocusable())
        clearFocusedElementSoon();
    layoutView()->clearHitTestCache();

    ASSERT(!DocumentAnimations::needsAnimationTimingUpdate(*this));

    unsigned elementCount = styleEngine().styleForElementCount() - startElementCount;

    TRACE_EVENT_END1("blink,devtools.timeline", "UpdateLayoutTree", "elementCount", elementCount);
    InspectorInstrumentation::didRecalculateStyle(cookie, elementCount);

#if ENABLE(ASSERT)
    assertLayoutTreeUpdated(*this);
#endif
}
