void Document::updateStyle(StyleRecalcChange change)
{
    TRACE_EVENT_BEGIN0("blink,blink_style", "Document::updateStyle");
    unsigned initialResolverAccessCount = styleEngine().resolverAccessCount();

    HTMLFrameOwnerElement::UpdateSuspendScope suspendWidgetHierarchyUpdates;
    m_lifecycle.advanceTo(DocumentLifecycle::InStyleRecalc);

    NthIndexCache nthIndexCache(*this);

    if (styleChangeType() >= SubtreeStyleChange)
        change = Force;


    if (change == Force) {
        m_hasNodesWithPlaceholderStyle = false;
        RefPtr<ComputedStyle> documentStyle = StyleResolver::styleForDocument(*this);
        StyleRecalcChange localChange = ComputedStyle::stylePropagationDiff(documentStyle.get(), layoutView()->style());
        if (localChange != NoChange)
            layoutView()->setStyle(documentStyle.release());
    }

    clearNeedsStyleRecalc();


    if (StyleResolverStats* stats = ensureStyleResolver().stats())
        stats->reset();

    if (Element* documentElement = this->documentElement()) {
        inheritHtmlAndBodyElementStyles(change);
        dirtyElementsForLayerUpdate();
        if (documentElement->shouldCallRecalcStyle(change))
            documentElement->recalcStyle(change);
        while (dirtyElementsForLayerUpdate())
            documentElement->recalcStyle(NoChange);
    }

    ensureStyleResolver().printStats();

    view()->recalcOverflowAfterStyleChange();

    clearChildNeedsStyleRecalc();

    if (styleEngine().hasResolver()) {
        StyleResolver& resolver = styleEngine().ensureResolver();
        styleEngine().resetCSSFeatureFlags(resolver.ensureUpdatedRuleFeatureSet());
        resolver.clearStyleSharingList();
    }

    ASSERT(!needsStyleRecalc());
    ASSERT(!childNeedsStyleRecalc());
    ASSERT(inStyleRecalc());
    m_lifecycle.advanceTo(DocumentLifecycle::StyleClean);
    TRACE_EVENT_END1("blink,blink_style", "Document::updateStyle", "resolverAccessCount", styleEngine().resolverAccessCount() - initialResolverAccessCount);
}
