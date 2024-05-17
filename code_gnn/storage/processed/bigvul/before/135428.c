void Document::updateLayoutTreeIgnorePendingStylesheets()
{
    StyleEngine::IgnoringPendingStylesheet ignoring(styleEngine());

    if (styleEngine().hasPendingSheets()) {
        HTMLElement* bodyElement = body();
        if (bodyElement && !bodyElement->layoutObject() && m_pendingSheetLayout == NoLayoutWithPendingSheets) {
            m_pendingSheetLayout = DidLayoutWithPendingSheets;
            styleResolverChanged();
        } else if (m_hasNodesWithPlaceholderStyle) {
            updateLayoutTree(Force);
        }
    }
    updateLayoutTreeIfNeeded();
}
