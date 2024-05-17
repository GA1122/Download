void Document::recalcStyle(StyleRecalcChange change)
{
    RELEASE_ASSERT(!view() || !view()->isPainting());

    if (!renderer() || !view())
        return;

    if (m_inStyleRecalc)
        return;

    TRACE_EVENT0("webkit", "Document::recalcStyle");
    TRACE_EVENT_SCOPED_SAMPLING_STATE("Blink", "RecalcStyle");

    updateDistributionIfNeeded();

    InspectorInstrumentationCookie cookie = InspectorInstrumentation::willRecalculateStyle(this);

    if (m_evaluateMediaQueriesOnStyleRecalc) {
        m_evaluateMediaQueriesOnStyleRecalc = false;
        evaluateMediaQueryList();
    }


    if (m_styleEngine->needsUpdateActiveStylesheetsOnStyleRecalc())
        m_styleEngine->updateActiveStyleSheets(FullStyleUpdate);

    if (m_elemSheet && m_elemSheet->contents()->usesRemUnits())
        m_styleEngine->setUsesRemUnit(true);

    {
        PostAttachCallbacks::SuspendScope suspendPostAttachCallbacks;
        RenderWidget::UpdateSuspendScope suspendWidgetHierarchyUpdates;
        FrameView::DeferredRepaintScope deferRepaints(*view());
        TemporaryChange<bool> changeInStyleRecalc(m_inStyleRecalc, true);

        if (styleChangeType() >= SubtreeStyleChange)
            change = Force;

        if (change == Force || (change >= Inherit && shouldDisplaySeamlesslyWithParent())) {
            m_hasNodesWithPlaceholderStyle = false;
            RefPtr<RenderStyle> documentStyle = StyleResolver::styleForDocument(*this, m_styleResolver ? m_styleResolver->fontSelector() : 0);
            StyleRecalcChange localChange = RenderStyle::compare(documentStyle.get(), renderer()->style());
            if (localChange != NoChange)
                renderer()->setStyle(documentStyle.release());
        }

        inheritHtmlAndBodyElementStyles(change);

        if (Element* documentElement = this->documentElement()) {
            if (shouldRecalcStyle(change, documentElement))
                documentElement->recalcStyle(change);
        }

        view()->updateCompositingLayersAfterStyleChange();

        clearNeedsStyleRecalc();
        clearChildNeedsStyleRecalc();
        unscheduleStyleRecalc();

        if (m_styleEngine->needsUpdateActiveStylesheetsOnStyleRecalc())
            setNeedsStyleRecalc();

        if (m_styleResolver) {
            m_styleEngine->resetCSSFeatureFlags(m_styleResolver->ruleFeatureSet());
            m_styleResolver->clearStyleSharingList();
        }
    }

    STYLE_STATS_PRINT();
    STYLE_STATS_CLEAR();

    InspectorInstrumentation::didRecalculateStyle(cookie);

    if (hoverNode() && !hoverNode()->renderer() && frame())
        frame()->eventHandler()->dispatchFakeMouseMoveEventSoon();
}
