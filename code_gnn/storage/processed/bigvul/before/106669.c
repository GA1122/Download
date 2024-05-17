void WebPageProxy::viewStateDidChange(ViewStateFlags flags)
{
    if (!isValid())
        return;

    if (flags & ViewIsFocused)
        process()->send(Messages::WebPage::SetFocused(m_pageClient->isViewFocused()), m_pageID);

    if (flags & ViewWindowIsActive)
        process()->send(Messages::WebPage::SetActive(m_pageClient->isViewWindowActive()), m_pageID);

    if (flags & ViewIsVisible) {
        bool isVisible = m_pageClient->isViewVisible();
        if (isVisible != m_isVisible) {
            m_isVisible = isVisible;
            m_drawingArea->visibilityDidChange();
            m_drawingArea->setPageIsVisible(isVisible);
        }
    }

    if (flags & ViewIsInWindow) {
        bool isInWindow = m_pageClient->isViewInWindow();
        if (m_isInWindow != isInWindow) {
            m_isInWindow = isInWindow;
            process()->send(Messages::WebPage::SetIsInWindow(isInWindow), m_pageID);
        }
    }

    if (flags & (ViewWindowIsActive | ViewIsVisible))
        m_drawingArea->setBackingStoreIsDiscardable(!m_pageClient->isViewWindowActive() || !isViewVisible());
}
