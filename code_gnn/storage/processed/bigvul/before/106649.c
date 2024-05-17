void WebPageProxy::showPopupMenu(const IntRect& rect, uint64_t textDirection, const Vector<WebPopupItem>& items, int32_t selectedIndex, const PlatformPopupMenuData& data)
{
    if (m_activePopupMenu) {
        m_activePopupMenu->hidePopupMenu();
        m_activePopupMenu->invalidate();
        m_activePopupMenu = 0;
    }

    m_activePopupMenu = m_pageClient->createPopupMenuProxy(this);

    process()->responsivenessTimer()->stop();

    RefPtr<WebPopupMenuProxy> protectedActivePopupMenu = m_activePopupMenu;

    protectedActivePopupMenu->showPopupMenu(rect, static_cast<TextDirection>(textDirection), m_viewScaleFactor, items, data, selectedIndex);
    protectedActivePopupMenu->invalidate();
    protectedActivePopupMenu = 0;
}
