void Document::styleResolverChanged(StyleResolverUpdateMode updateMode)
{
    styleEngine().resolverChanged(updateMode);

    if (didLayoutWithPendingStylesheets() && !styleEngine().hasPendingSheets()) {
        m_pendingSheetLayout = IgnoreLayoutWithPendingSheets;

        ASSERT(layoutView() || importsController());
        if (layoutView())
            layoutView()->invalidatePaintForViewAndCompositedLayers();
    }
}
