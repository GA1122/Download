PassRefPtr<WebPopupMenuProxy> WebView::createPopupMenuProxy(WebPageProxy* page)
{
    return WebPopupMenuProxyWin::create(this, page);
}
