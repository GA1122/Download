void webkit_web_view_load_request(WebKitWebView* webView, WebKitNetworkRequest* request)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));
    g_return_if_fail(WEBKIT_IS_NETWORK_REQUEST(request));

    WebKitWebFrame* frame = webView->priv->mainFrame;
    webkit_web_frame_load_request(frame, request);
}
