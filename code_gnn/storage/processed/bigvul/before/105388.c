static gdouble webViewGetDPI(WebKitWebView* webView)
{
    WebKitWebViewPrivate* priv = webView->priv;
    WebKitWebSettings* webSettings = priv->webSettings.get();
    gboolean enforce96DPI;
    g_object_get(webSettings, "enforce-96-dpi", &enforce96DPI, NULL);
    if (enforce96DPI)
        return 96.0;

    gdouble DPI = defaultDPI;
    GdkScreen* screen = gtk_widget_has_screen(GTK_WIDGET(webView)) ? gtk_widget_get_screen(GTK_WIDGET(webView)) : gdk_screen_get_default();
    if (screen) {
        DPI = gdk_screen_get_resolution(screen);
        if (DPI == -1)
            DPI = defaultDPI;
    }
    ASSERT(DPI > 0);
    return DPI;
}
