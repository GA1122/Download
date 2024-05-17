void webkit_web_view_add_resource(WebKitWebView* webView, const char* identifier, WebKitWebResource* webResource)
{
    WebKitWebViewPrivate* priv = webView->priv;
    g_hash_table_insert(priv->subResources.get(), g_strdup(identifier), webResource);
}
