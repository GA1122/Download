static gboolean webkit_web_view_real_console_message(WebKitWebView* webView, const gchar* message, unsigned int line, const gchar* sourceId)
{
    g_message("console message: %s @%d: %s\n", sourceId, line, message);
    return TRUE;
}
