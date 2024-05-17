void Document::didRemoveAllPendingStylesheet()
{
    m_needsNotifyRemoveAllPendingStylesheet = false;

    styleResolverChanged(RecalcStyleDeferred, AnalyzedStyleUpdate);
    executeScriptsWaitingForResourcesIfNeeded();

    if (m_gotoAnchorNeededAfterStylesheetsLoad && view())
        view()->scrollToFragment(m_url);
}
