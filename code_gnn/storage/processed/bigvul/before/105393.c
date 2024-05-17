static void webkit_web_view_apply_zoom_level(WebKitWebView* webView, gfloat zoomLevel)
{
    Frame* frame = core(webView)->mainFrame();
    if (!frame)
        return;

    WebKitWebViewPrivate* priv = webView->priv;
    if (priv->zoomFullContent)
        frame->setPageZoomFactor(zoomLevel);
    else
        frame->setTextZoomFactor(zoomLevel);        
}
