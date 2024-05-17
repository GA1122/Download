static void setHorizontalAdjustment(WebKitWebView* webView, GtkAdjustment* adjustment)
{
    Page* page = core(webView);
    if (page)
        static_cast<WebKit::ChromeClient*>(page->chrome()->client())->adjustmentWatcher()->setHorizontalAdjustment(adjustment);
}
