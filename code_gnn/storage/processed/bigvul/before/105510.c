void webkit_web_view_set_full_content_zoom(WebKitWebView* webView, gboolean zoomFullContent)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));

    WebKitWebViewPrivate* priv = webView->priv;
    if (priv->zoomFullContent == zoomFullContent)
      return;

    Frame* frame = core(webView)->mainFrame();
    if (!frame)
      return;

    gfloat zoomLevel = priv->zoomFullContent ? frame->pageZoomFactor() : frame->textZoomFactor();

    priv->zoomFullContent = zoomFullContent;
    if (priv->zoomFullContent)
        frame->setPageAndTextZoomFactors(zoomLevel, 1);
    else
        frame->setPageAndTextZoomFactors(1, zoomLevel);

    g_object_notify(G_OBJECT(webView), "full-content-zoom");
}
