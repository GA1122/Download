WebKitWebResource* webkit_web_view_get_main_resource(WebKitWebView* webView)
{
    return webView->priv->mainResource.get();
}
