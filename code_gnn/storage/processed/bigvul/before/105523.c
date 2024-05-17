static void webkit_web_view_size_allocate(GtkWidget* widget, GtkAllocation* allocation)
{
    GTK_WIDGET_CLASS(webkit_web_view_parent_class)->size_allocate(widget, allocation);

    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);

    Page* page = core(webView);
    Frame* frame = page->mainFrame();
    if (!frame->view())
        return;

    frame->view()->resize(allocation->width, allocation->height);
    static_cast<WebKit::ChromeClient*>(page->chrome()->client())->adjustmentWatcher()->updateAdjustmentsFromScrollbars();
}
