void FrameLoader::restoreScrollPositionAndViewState()
{
    FrameView* view = m_frame->view();
    if (!m_frame->page()
        || !view
        || !view->layoutViewportScrollableArea()
        || !m_currentItem
        || !m_stateMachine.committedFirstRealDocumentLoad()
        || !documentLoader()) {
        return;
    }

    if (!needsHistoryItemRestore(m_loadType))
        return;

    bool shouldRestoreScroll = m_currentItem->scrollRestorationType() != ScrollRestorationManual;
    bool shouldRestoreScale = m_currentItem->pageScaleFactor();

    bool canRestoreWithoutClamping = view->layoutViewportScrollableArea()->clampScrollPosition(m_currentItem->scrollPoint()) == m_currentItem->scrollPoint();
    bool canRestoreWithoutAnnoyingUser = !documentLoader()->initialScrollState().wasScrolledByUser
        && (canRestoreWithoutClamping || !m_frame->isLoading() || !shouldRestoreScroll);
    if (!canRestoreWithoutAnnoyingUser)
        return;

    if (shouldRestoreScroll)
        view->layoutViewportScrollableArea()->setScrollPosition(m_currentItem->scrollPoint(), ProgrammaticScroll);

    if (m_frame->isMainFrame()) {
        FloatPoint visualViewportOffset(m_currentItem->visualViewportScrollPoint());

        if (visualViewportOffset.x() == -1 && visualViewportOffset.y() == -1)
            visualViewportOffset = FloatPoint(m_currentItem->scrollPoint() - view->scrollPosition());

        VisualViewport& visualViewport = m_frame->host()->visualViewport();
        if (shouldRestoreScale && shouldRestoreScroll)
            visualViewport.setScaleAndLocation(m_currentItem->pageScaleFactor(), visualViewportOffset);
        else if (shouldRestoreScale)
            visualViewport.setScale(m_currentItem->pageScaleFactor());
        else if (shouldRestoreScroll)
            visualViewport.setLocation(visualViewportOffset);

        if (ScrollingCoordinator* scrollingCoordinator = m_frame->page()->scrollingCoordinator())
            scrollingCoordinator->frameViewRootLayerDidChange(view);
    }

    documentLoader()->initialScrollState().didRestoreFromHistory = true;
}
