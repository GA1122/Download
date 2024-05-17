const gchar* webkit_web_view_get_encoding(WebKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), NULL);
    String encoding = core(webView)->mainFrame()->document()->loader()->writer()->encoding();
    if (encoding.isEmpty())
        return 0;
    webView->priv->encoding = encoding.utf8();
    return webView->priv->encoding.data();
}
