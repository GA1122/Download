void webkit_web_view_load_uri(WebKitWebView* webView, const gchar* uri)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));
    g_return_if_fail(uri);

    WebKitWebFrame* frame = webView->priv->mainFrame;
    webkit_web_frame_load_uri(frame, uri);
}
