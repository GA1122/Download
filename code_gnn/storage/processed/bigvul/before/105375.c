static GtkAdjustment* getVerticalAdjustment(WebKitWebView* webView)
{
    Page* page = core(webView);
    if (page)
        return static_cast<WebKit::ChromeClient*>(page->chrome()->client())->adjustmentWatcher()->verticalAdjustment();
    return 0;
}
