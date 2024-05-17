void webkit_web_view_open(WebKitWebView* webView, const gchar* uri)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));
    g_return_if_fail(uri);

    if (g_path_is_absolute(uri)) {
        gchar* fileUri = g_filename_to_uri(uri, NULL, NULL);
        webkit_web_view_load_uri(webView, fileUri);
        g_free(fileUri);
    }
    else
        webkit_web_view_load_uri(webView, uri);
}
