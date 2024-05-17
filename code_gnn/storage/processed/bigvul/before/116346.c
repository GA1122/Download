void PlatformWebView::resizeTo(unsigned width, unsigned height)
{
    if (!m_window->handle()) {
        QRect newGeometry(m_window->x(), m_window->y(), width, height);
        QWindowSystemInterface::handleSynchronousGeometryChange(m_window, newGeometry);
    }

    m_window->resize(width, height);
}
