void Document::updateStyle()
{
    DCHECK(!view()->shouldThrottleRendering());
    TRACE_EVENT_BEGIN0("blink,blink_style", "Document::updateStyle");
    unsigned initialElementCount = styleEngine().styleForElementCount();

    HTMLFrameOwnerElement::UpdateSuspendScope suspendWidgetHierarchyUpdates;
    m_lifecycle.advanceTo(DocumentLifecycle::InStyleRecalc);

    StyleRecalcChange change = NoChange;
    if (getStyleChangeType() >= SubtreeStyleChange)
        change = Force;

    NthIndexCache nthIndexCache(*this);


    if (change == Force) {
        m_hasNodesWithPlaceholderStyle = false;
        RefPtr<ComputedStyle> documentStyle = StyleResolver::styleForDocument(*this);
        StyleRecalcChange localChange = ComputedStyle::stylePropagationDiff(documentStyle.get(), layoutView()->style());
        if (localChange != NoChange)
            layoutView()->setStyle(documentStyle.release());
    }

    clearNeedsStyleRecalc();

    StyleResolver& resolver = ensureStyleResolver();

    bool shouldRecordStats;
    TRACE_EVENT_CATEGORY_GROUP_ENABLED("blink,blink_style", &shouldRecordStats);
    styleEngine().setStatsEnabled(shouldRecordStats);

    if (Element* documentElement = this->documentElement()) {
        inheritHtmlAndBodyElementStyles(change);
        dirtyElementsForLayerUpdate();
        if (documentElement->shouldCallRecalcStyle(change))
            documentElement->recalcStyle(change);
        while (dirtyElementsForLayerUpdate())
            documentElement->recalcStyle(NoChange);
    }

    view()->recalcOverflowAfterStyleChange();

    clearChildNeedsStyleRecalc();

    resolver.clearStyleSharingList();

    m_wasPrinting = m_printing;

    DCHECK(!needsStyleRecalc());
    DCHECK(!childNeedsStyleRecalc());
    DCHECK(inStyleRecalc());
    DCHECK_EQ(styleResolver(), &resolver);
    m_lifecycle.advanceTo(DocumentLifecycle::StyleClean);
    if (shouldRecordStats) {
        TRACE_EVENT_END2("blink,blink_style", "Document::updateStyle",
            "resolverAccessCount", styleEngine().styleForElementCount() - initialElementCount,
            "counters", styleEngine().stats()->toTracedValue());
    } else {
        TRACE_EVENT_END1("blink,blink_style", "Document::updateStyle",
            "resolverAccessCount", styleEngine().styleForElementCount() - initialElementCount);
    }
}
