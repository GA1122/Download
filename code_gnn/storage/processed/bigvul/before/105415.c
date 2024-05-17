static gboolean webkit_web_view_drag_drop(GtkWidget* widget, GdkDragContext* context, gint x, gint y, guint time)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
    WebKitWebViewPrivate* priv = webView->priv;

    if (!priv->droppingContexts.contains(context))
        return FALSE;

    DroppingContext* droppingContext = priv->droppingContexts.get(context);
    droppingContext->dropHappened = true;

    IntPoint position(x, y);
    DragData dragData(droppingContext->dataObject.get(), position, globalPointForClientPoint(gtk_widget_get_window(widget), position), gdkDragActionToDragOperation(gdk_drag_context_get_actions(context)));
    core(webView)->dragController()->performDrag(&dragData);

    gtk_drag_finish(context, TRUE, FALSE, time);
    return TRUE;
}
