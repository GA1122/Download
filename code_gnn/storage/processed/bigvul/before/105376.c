static GtkScrollablePolicy getVerticalScrollPolicy(WebKitWebView* webView)
{
    return webView->priv->verticalScrollingPolicy;
}
