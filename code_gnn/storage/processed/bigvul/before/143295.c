void Document::updateStyleAndLayoutTree()
{
    DCHECK(isMainThread());

    ScriptForbiddenScope forbidScript;
    PluginScriptForbiddenScope pluginForbidScript;

    if (!view() || !isActive())
        return;

    if (view()->shouldThrottleRendering())
        return;

    if (!needsLayoutTreeUpdate()) {
        if (lifecycle().state() < DocumentLifecycle::StyleClean) {
            lifecycle().advanceTo(DocumentLifecycle::InStyleRecalc);
            lifecycle().advanceTo(DocumentLifecycle::StyleClean);
        }
        return;
    }

    if (inStyleRecalc())
        return;

    RELEASE_ASSERT(lifecycle().stateAllowsTreeMutations());

    TRACE_EVENT_BEGIN1("blink,devtools.timeline", "UpdateLayoutTree", "beginData", InspectorRecalculateStylesEvent::data(frame()));
    TRACE_EVENT_SCOPED_SAMPLING_STATE("blink", "UpdateLayoutTree");

    unsigned startElementCount = styleEngine().styleForElementCount();

    InspectorInstrumentation::StyleRecalc instrumentation(this);

    DocumentAnimations::updateAnimationTimingIfNeeded(*this);
    evaluateMediaQueryListIfNeeded();
    updateUseShadowTreesIfNeeded();
    updateDistribution();
    updateStyleInvalidationIfNeeded();


    updateStyle();

    notifyLayoutTreeOfSubtreeChanges();

    if (hoverNode() && !hoverNode()->layoutObject() && frame())
        frame()->eventHandler().dispatchFakeMouseMoveEventSoon();

    if (m_focusedElement && !m_focusedElement->isFocusable())
        clearFocusedElementSoon();
    layoutView()->clearHitTestCache();

    DCHECK(!DocumentAnimations::needsAnimationTimingUpdate(*this));

    unsigned elementCount = styleEngine().styleForElementCount() - startElementCount;

    TRACE_EVENT_END1("blink,devtools.timeline", "UpdateLayoutTree", "elementCount", elementCount);

#if DCHECK_IS_ON()
    assertLayoutTreeUpdated(*this);
#endif
}
