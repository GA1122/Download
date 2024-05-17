void Document::styleResolverChanged(StyleResolverUpdateFlag updateFlag)
{
    if (!attached() || (!m_didCalculateStyleResolver && !haveStylesheetsLoaded())) {
        m_styleResolver.clear();
        return;
    }
    m_didCalculateStyleResolver = true;

#ifdef INSTRUMENT_LAYOUT_SCHEDULING
    if (!ownerElement())
        printf("Beginning update of style selector at time %d.\n", elapsedTime());
#endif

    DocumentStyleSheetCollection::UpdateFlag styleSheetUpdate = (updateFlag == RecalcStyleIfNeeded)
        ? DocumentStyleSheetCollection::OptimizedUpdate
        : DocumentStyleSheetCollection::FullUpdate;
    bool stylesheetChangeRequiresStyleRecalc = m_styleSheetCollection->updateActiveStyleSheets(styleSheetUpdate);

    if (updateFlag == DeferRecalcStyle) {
        scheduleForcedStyleRecalc();
        return;
    }

    if (didLayoutWithPendingStylesheets() && !m_styleSheetCollection->hasPendingSheets()) {
        m_pendingSheetLayout = IgnoreLayoutWithPendingSheets;
        if (renderer())
            renderView()->repaintViewAndCompositedLayers();
    }

    if (!stylesheetChangeRequiresStyleRecalc)
        return;

    {
        AnimationUpdateBlock animationUpdateBlock(m_frame ? m_frame->animation() : 0);
        recalcStyle(Force);
    }

#ifdef INSTRUMENT_LAYOUT_SCHEDULING
    if (!ownerElement())
        printf("Finished update of style selector at time %d\n", elapsedTime());
#endif

    if (renderer()) {
        renderer()->setNeedsLayoutAndPrefWidthsRecalc();
        if (view())
            view()->scheduleRelayout();
    }

    evaluateMediaQueryList();
}
