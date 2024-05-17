static gboolean webkitWebViewBaseDragMotion(GtkWidget* widget, GdkDragContext* context, gint x, gint y, guint time)
{
    WebKitWebViewBase* webViewBase = WEBKIT_WEB_VIEW_BASE(widget);
    OwnPtr<DragData> dragData(webViewBase->priv->dragAndDropHelper.handleDragMotion(context, IntPoint(x, y), time));
    if (!dragData)
        return TRUE;

    webViewBase->priv->pageProxy->dragUpdated(dragData.get());
    DragOperation operation = webViewBase->priv->pageProxy->dragSession().operation;
    gdk_drag_status(context, dragOperationToSingleGdkDragAction(operation), time);
    return TRUE;
}
