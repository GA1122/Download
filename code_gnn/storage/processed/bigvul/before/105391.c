void webkit_web_view_add_main_resource(WebKitWebView* webView, const char* identifier, WebKitWebResource* webResource)
{
    WebKitWebViewPrivate* priv = webView->priv;

    priv->mainResource = adoptGRef(webResource);
    priv->mainResourceIdentifier = identifier;
}
