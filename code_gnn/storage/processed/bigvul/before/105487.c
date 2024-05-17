static WebKitNavigationResponse webkit_web_view_real_navigation_requested(WebKitWebView*, WebKitWebFrame*, WebKitNetworkRequest*)
{
    return WEBKIT_NAVIGATION_RESPONSE_ACCEPT;
}
