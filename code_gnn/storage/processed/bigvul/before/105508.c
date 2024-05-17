void webkit_web_view_set_custom_encoding(WebKitWebView* webView, const char* encoding)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));

    core(webView)->mainFrame()->loader()->reloadWithOverrideEncoding(String::fromUTF8(encoding));
}
