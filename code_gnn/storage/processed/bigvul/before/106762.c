void WebView::setOverrideCursor(HCURSOR overrideCursor)
{
    m_overrideCursor = overrideCursor;
    updateNativeCursor();
}
