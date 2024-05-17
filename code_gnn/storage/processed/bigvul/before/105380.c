static inline gint pixelsFromSize(WebKitWebView* webView, gint size)
{
    gdouble DPI = webViewGetDPI(webView);
    return size / 72.0 * DPI;
}
