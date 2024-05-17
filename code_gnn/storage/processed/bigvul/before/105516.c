void webkit_web_view_set_tooltip_text(WebKitWebView* webView, const char* tooltip)
{
#if GTK_CHECK_VERSION(2, 12, 0)
    WebKitWebViewPrivate* priv = webView->priv;
    if (tooltip && *tooltip != '\0') {
        priv->tooltipText = tooltip;
        gtk_widget_set_has_tooltip(GTK_WIDGET(webView), TRUE);
    } else {
        priv->tooltipText = "";
        gtk_widget_set_has_tooltip(GTK_WIDGET(webView), FALSE);
    }

    gtk_widget_trigger_tooltip_query(GTK_WIDGET(webView));
#else
    notImplemented();
#endif
}
