void webkit_web_view_load_string(WebKitWebView* webView, const gchar* content, const gchar* mimeType, const gchar* encoding, const gchar* baseUri)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));
    g_return_if_fail(content);

    WebKitWebFrame* frame = webView->priv->mainFrame;
    webkit_web_frame_load_string(frame, content, mimeType, encoding, baseUri);
}
