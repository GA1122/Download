static gboolean webkitWebViewBaseQueryTooltip(GtkWidget* widget, gint x, gint y, gboolean keyboardMode, GtkTooltip* tooltip)
{
    WebKitWebViewBasePrivate* priv = WEBKIT_WEB_VIEW_BASE(widget)->priv;

    if (keyboardMode) {
        notImplemented();
        return FALSE;
    }

    if (priv->tooltipText.length() <= 0)
        return FALSE;

    gtk_tooltip_set_text(tooltip, priv->tooltipText.data());
    return TRUE;
}
