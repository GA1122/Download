void Document::updateViewportDescription()
{
    if (page() && page()->mainFrame() == frame()) {
#ifndef NDEBUG
        m_didDispatchViewportPropertiesChanged = true;
#endif
        page()->chrome().dispatchViewportPropertiesDidChange(m_viewportDescription);
    }
}
