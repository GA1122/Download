void FrameView::enableAutoSizeMode(const IntSize& minSize, const IntSize& maxSize)
{
    if (!m_autoSizeInfo)
        m_autoSizeInfo = adoptPtr(new FrameViewAutoSizeInfo(this));

    m_autoSizeInfo->configureAutoSizeMode(minSize, maxSize);
}
