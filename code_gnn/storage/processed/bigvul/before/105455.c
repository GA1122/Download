WebKitViewportAttributes* webkit_web_view_get_viewport_attributes(WebKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);
    return webView->priv->viewportAttributes.get();
}
