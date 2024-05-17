void FrameLoader::saveScrollState()
{
    if (!m_currentItem || !m_frame->view())
        return;

    if (needsHistoryItemRestore(m_loadType) && m_documentLoader && !m_documentLoader->initialScrollState().wasScrolledByUser)
        return;

    if (ScrollableArea* layoutScrollableArea = m_frame->view()->layoutViewportScrollableArea())
        m_currentItem->setScrollPoint(layoutScrollableArea->scrollPosition());
    m_currentItem->setVisualViewportScrollPoint(m_frame->host()->visualViewport().visibleRect().location());

    if (m_frame->isMainFrame())
        m_currentItem->setPageScaleFactor(m_frame->page()->pageScaleFactor());

    client()->didUpdateCurrentHistoryItem();
}
