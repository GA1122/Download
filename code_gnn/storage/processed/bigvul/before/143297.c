void Document::updateStyleAndLayoutTreeIgnorePendingStylesheets()
{
    StyleEngine::IgnoringPendingStylesheet ignoring(styleEngine());

    if (styleEngine().hasPendingScriptBlockingSheets()) {
        HTMLElement* bodyElement = body();
        if (bodyElement && !bodyElement->layoutObject() && m_pendingSheetLayout == NoLayoutWithPendingSheets) {
            m_pendingSheetLayout = DidLayoutWithPendingSheets;
            styleEngine().resolverChanged(FullStyleUpdate);
        } else if (m_hasNodesWithPlaceholderStyle) {
            setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::CleanupPlaceholderStyles));
        }
    }
    updateStyleAndLayoutTree();
}
