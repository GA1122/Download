static void webkit_web_view_drag_leave(GtkWidget* widget, GdkDragContext* context, guint time)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
    WebKitWebViewPrivate* priv = webView->priv;

    if (!priv->droppingContexts.contains(context))
        return;

    g_timeout_add(0, reinterpret_cast<GSourceFunc>(doDragLeaveLater), priv->droppingContexts.get(context));
}
