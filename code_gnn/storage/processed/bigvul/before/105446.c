WebKitWebResource* webkit_web_view_get_resource(WebKitWebView* webView, char* identifier)
{
    WebKitWebViewPrivate* priv = webView->priv;
    gpointer webResource = 0;
    gboolean resourceFound = g_hash_table_lookup_extended(priv->subResources.get(), identifier, NULL, &webResource);

    if (!resourceFound && !g_str_equal(identifier, priv->mainResourceIdentifier.data()))
        return 0;

    if (!webResource)
        return webkit_web_view_get_main_resource(webView);

    return WEBKIT_WEB_RESOURCE(webResource);
}
