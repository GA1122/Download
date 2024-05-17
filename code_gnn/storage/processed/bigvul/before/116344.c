void PlatformWebView::postEvent(QEvent* event)
{
    QCoreApplication::postEvent(m_window, event);
}
