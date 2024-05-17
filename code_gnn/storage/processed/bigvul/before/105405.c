void webkit_web_view_clear_resources(WebKitWebView* webView)
{
    WebKitWebViewPrivate* priv = webView->priv;

    if (priv->subResources)
        g_hash_table_remove_all(priv->subResources.get());
}
