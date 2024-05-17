void Document::styleResolverMayHaveChanged()
{
    styleEngine().resolverChanged(hasNodesWithPlaceholderStyle() ? FullStyleUpdate : AnalyzedStyleUpdate);

    if (didLayoutWithPendingStylesheets() && !styleEngine().hasPendingScriptBlockingSheets()) {
        m_pendingSheetLayout = IgnoreLayoutWithPendingSheets;

        DCHECK(layoutView() || importsController());
        if (layoutView())
            layoutView()->invalidatePaintForViewAndCompositedLayers();
    }
}
