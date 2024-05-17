void webkit_web_view_execute_script(WebKitWebView* webView, const gchar* script)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));
    g_return_if_fail(script);

    core(webView)->mainFrame()->script()->executeScript(String::fromUTF8(script), true);
}
