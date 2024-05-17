G_CONST_RETURN gchar* webkit_web_view_get_icon_uri(WebKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);
    String iconURL = iconDatabase().synchronousIconURLForPageURL(core(webView)->mainFrame()->document()->url().string());
    webView->priv->iconURI = iconURL.utf8();
    return webView->priv->iconURI.data();
}
