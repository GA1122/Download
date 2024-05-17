static void webkit_web_view_drag_data_received(GtkWidget* widget, GdkDragContext* context, gint x, gint y, GtkSelectionData* selectionData, guint info, guint time)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
    WebKitWebViewPrivate* priv = webView->priv;

    if (!priv->droppingContexts.contains(context))
        return;

    DroppingContext* droppingContext = priv->droppingContexts.get(context);
    droppingContext->pendingDataRequests--;
    pasteboardHelperInstance()->fillDataObjectFromDropData(selectionData, info, droppingContext->dataObject.get());

    if (droppingContext->pendingDataRequests)
        return;

    const IntPoint& position = droppingContext->lastMotionPosition;

    DragData dragData(droppingContext->dataObject.get(), position, globalPointForClientPoint(gtk_widget_get_window(widget), position), gdkDragActionToDragOperation(gdk_drag_context_get_actions(context)));
    DragOperation operation = core(webView)->dragController()->dragEntered(&dragData);
    gdk_drag_status(context, dragOperationToSingleGdkDragAction(operation), time);
}
