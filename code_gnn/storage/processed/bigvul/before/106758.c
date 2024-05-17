void WebView::setFindIndicatorCallback(WKViewFindIndicatorCallback callback, void* context)
{
    m_findIndicatorCallback = callback;
    m_findIndicatorCallbackContext = context;
}
