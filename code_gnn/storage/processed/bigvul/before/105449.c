GList* webkit_web_view_get_subresources(WebKitWebView* webView)
{
    WebKitWebViewPrivate* priv = webView->priv;
    GList* subResources = g_hash_table_get_values(priv->subResources.get());
    return g_list_remove(subResources, priv->mainResource.get());
}
