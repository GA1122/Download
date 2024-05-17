GdkPixbuf* webkit_web_view_get_icon_pixbuf(WebKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);

    const gchar* pageURI = webkit_web_view_get_uri(webView);
    WebKitIconDatabase* database = webkit_get_icon_database();
    return webkit_icon_database_get_icon_pixbuf(database, pageURI);
}
