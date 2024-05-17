static void webkit_web_view_set_scroll_adjustments(WebKitWebView* webView, GtkAdjustment* horizontalAdjustment, GtkAdjustment* verticalAdjustment)
{
    Page* page = core(webView);
    if (!page)
        return;

    WebKit::ChromeClient* client = static_cast<WebKit::ChromeClient*>(page->chrome()->client());
    client->adjustmentWatcher()->setHorizontalAdjustment(horizontalAdjustment);
    client->adjustmentWatcher()->setVerticalAdjustment(verticalAdjustment);
}
