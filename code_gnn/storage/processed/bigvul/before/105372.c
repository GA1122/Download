static GtkAdjustment* getHorizontalAdjustment(WebKitWebView* webView)
{
    Page* page = core(webView);
    if (page)
        return static_cast<WebKit::ChromeClient*>(page->chrome()->client())->adjustmentWatcher()->horizontalAdjustment();
    return 0;
}
