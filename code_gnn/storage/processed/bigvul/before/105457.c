gfloat webkit_web_view_get_zoom_level(WebKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 1.0f);

    Frame* frame = core(webView)->mainFrame();
    if (!frame)
        return 1.0f;

    WebKitWebViewPrivate* priv = webView->priv;
    return priv->zoomFullContent ? frame->pageZoomFactor() : frame->textZoomFactor();
}
