void WebPagePrivate::overflowExceedsContentsSize()
{
    m_overflowExceedsContentsSize = true;
    if (absoluteVisibleOverflowSize().width() < DEFAULT_MAX_LAYOUT_WIDTH && !hasVirtualViewport()) {
        if (setViewMode(viewMode())) {
            setNeedsLayout();
            requestLayoutIfNeeded();
        }
    }
}
