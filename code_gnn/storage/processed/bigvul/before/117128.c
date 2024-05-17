static void webkitWebViewBaseDragDataReceived(GtkWidget* widget, GdkDragContext* context, gint x, gint y, GtkSelectionData* selectionData, guint info, guint time)
{
    WebKitWebViewBase* webViewBase = WEBKIT_WEB_VIEW_BASE(widget);
    OwnPtr<DragData> dragData(webViewBase->priv->dragAndDropHelper.handleDragDataReceived(context, selectionData, info));
    if (!dragData)
        return;

    webViewBase->priv->pageProxy->resetDragOperation();
    webViewBase->priv->pageProxy->dragEntered(dragData.get());
    DragOperation operation = webViewBase->priv->pageProxy->dragSession().operation;
    gdk_drag_status(context, dragOperationToSingleGdkDragAction(operation), time);
}
