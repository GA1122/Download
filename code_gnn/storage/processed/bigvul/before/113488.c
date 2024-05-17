IntSize WebPagePrivate::fixedLayoutSize(bool snapToIncrement) const
{
    if (hasVirtualViewport())
        return m_virtualViewportSize;

    const int defaultLayoutWidth = m_defaultLayoutSize.width();
    const int defaultLayoutHeight = m_defaultLayoutSize.height();

    int minWidth = defaultLayoutWidth;
    int maxWidth = DEFAULT_MAX_LAYOUT_WIDTH;
    int maxHeight = DEFAULT_MAX_LAYOUT_HEIGHT;

    if (m_loadState == None)
        return IntSize(defaultLayoutWidth, defaultLayoutHeight);

    if (m_viewMode == FixedDesktop) {
        int width  = maxWidth;
        int height;
        if (defaultLayoutHeight <= minimumLayoutSize.height())
            height = maxHeight;
        else
            height = ceilf(static_cast<float>(width) / static_cast<float>(defaultLayoutWidth) * static_cast<float>(defaultLayoutHeight));
        return IntSize(width, height);
    }

    if (m_viewMode == Desktop) {
        int width = std::max(absoluteVisibleOverflowSize().width(), contentsSize().width());
        if (m_pendingOrientation != -1 && !m_nestedLayoutFinishedCount && !m_overflowExceedsContentsSize)
            width = 0;

        if (snapToIncrement) {
            float factor = static_cast<float>(width) / (defaultLayoutWidth / 2.0);
            factor = ceilf(factor);
            width = (defaultLayoutWidth / 2.0) * factor;
        }

        if (width < minWidth)
            width = minWidth;
        if (width > maxWidth)
            width = maxWidth;
        int height = ceilf(static_cast<float>(width) / static_cast<float>(defaultLayoutWidth) * static_cast<float>(defaultLayoutHeight));
        return IntSize(width, height);
    }

    ASSERT_NOT_REACHED();
    return IntSize(defaultLayoutWidth, defaultLayoutHeight);
}
