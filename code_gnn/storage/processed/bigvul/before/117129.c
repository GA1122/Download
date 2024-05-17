static gboolean webkitWebViewBaseDragDrop(GtkWidget* widget, GdkDragContext* context, gint x, gint y, guint time)
{
    WebKitWebViewBase* webViewBase = WEBKIT_WEB_VIEW_BASE(widget);
    OwnPtr<DragData> dragData(webViewBase->priv->dragAndDropHelper.handleDragDrop(context, IntPoint(x, y)));
    if (!dragData)
        return FALSE;

    SandboxExtension::Handle handle;
    SandboxExtension::HandleArray sandboxExtensionForUpload;
    webViewBase->priv->pageProxy->performDrag(dragData.get(), String(), handle, sandboxExtensionForUpload);
    gtk_drag_finish(context, TRUE, FALSE, time);
    return TRUE;
}
