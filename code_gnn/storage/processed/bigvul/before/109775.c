void Document::recalcStyleForLayoutIgnoringPendingStylesheets()
{
    TemporaryChange<bool> ignorePendingStylesheets(m_ignorePendingStylesheets, m_ignorePendingStylesheets);
    if (!haveStylesheetsLoaded()) {
        m_ignorePendingStylesheets = true;
        HTMLElement* bodyElement = body();
        if (bodyElement && !bodyElement->renderer() && m_pendingSheetLayout == NoLayoutWithPendingSheets) {
            m_pendingSheetLayout = DidLayoutWithPendingSheets;
            styleResolverChanged(RecalcStyleImmediately);
        } else if (m_hasNodesWithPlaceholderStyle) {
            recalcStyle(Force);
        }
    }
}
