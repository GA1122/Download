void FrameView::handleLoadCompleted()
{
    if (m_autoSizeInfo)
        m_autoSizeInfo->autoSizeIfNeeded();
}
