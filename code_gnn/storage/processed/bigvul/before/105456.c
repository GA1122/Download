WebKitWebWindowFeatures* webkit_web_view_get_window_features(WebKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);
    return webView->priv->webWindowFeatures.get();
}
