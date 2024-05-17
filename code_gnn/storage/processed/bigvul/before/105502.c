void webkit_web_view_remove_resource(WebKitWebView* webView, const char* identifier)
{
    WebKitWebViewPrivate* priv = webView->priv;
    if (g_str_equal(identifier, priv->mainResourceIdentifier.data())) {
        priv->mainResourceIdentifier = "";
        priv->mainResource = 0;
    } else
      g_hash_table_remove(priv->subResources.get(), identifier);
}
