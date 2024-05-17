bool WebView::isViewFocused()
{
    return ::GetFocus() == m_window;
}
