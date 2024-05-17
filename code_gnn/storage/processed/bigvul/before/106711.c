WKViewFindIndicatorCallback WebView::getFindIndicatorCallback(void** context)
{
    if (context)
        *context = m_findIndicatorCallbackContext;
    
    return m_findIndicatorCallback;
}
