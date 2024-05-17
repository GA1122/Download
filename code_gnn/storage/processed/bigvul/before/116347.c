bool PlatformWebView::sendEvent(QEvent* event)
{
    return QCoreApplication::sendEvent(m_window, event);
}
