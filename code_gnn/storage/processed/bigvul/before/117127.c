static void webkitWebViewBaseDragDataGet(GtkWidget* widget, GdkDragContext* context, GtkSelectionData* selectionData, guint info, guint time)
{
    WEBKIT_WEB_VIEW_BASE(widget)->priv->dragAndDropHelper.handleGetDragData(context, selectionData, info);
}
