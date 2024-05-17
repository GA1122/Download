void webkit_web_view_set_transparent(WebKitWebView* webView, gboolean flag)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));

    WebKitWebViewPrivate* priv = webView->priv;
    priv->transparent = flag;

    Frame* frame = core(webView)->mainFrame();
    g_return_if_fail(frame);
    frame->view()->setTransparent(flag);
    g_object_notify(G_OBJECT(webView), "transparent");
}
