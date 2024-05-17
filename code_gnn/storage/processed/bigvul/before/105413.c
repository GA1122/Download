static void webkit_web_view_drag_data_get(GtkWidget* widget, GdkDragContext* context, GtkSelectionData* selectionData, guint info, guint)
{
    WebKitWebViewPrivate* priv = WEBKIT_WEB_VIEW(widget)->priv;

    if (!priv->draggingDataObjects.contains(context))
        return;

    pasteboardHelperInstance()->fillSelectionData(selectionData, info, priv->draggingDataObjects.get(context).get());
}
