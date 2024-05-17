static void webkitWebViewBaseDragLeave(GtkWidget* widget, GdkDragContext* context, guint time)
{
    WEBKIT_WEB_VIEW_BASE(widget)->priv->dragAndDropHelper.handleDragLeave(context, dragExitedCallback);
}
