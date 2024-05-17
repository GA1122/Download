PassRefPtr<WebContextMenuProxy> WebView::createContextMenuProxy(WebPageProxy* page)
{
    return WebContextMenuProxyWin::create(m_window, page);
}
