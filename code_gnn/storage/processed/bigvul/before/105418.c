static gboolean webkit_web_view_drag_motion(GtkWidget* widget, GdkDragContext* context, gint x, gint y, guint time)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);
    WebKitWebViewPrivate* priv = webView->priv;

    DroppingContext* droppingContext = 0;
    IntPoint position = IntPoint(x, y);
    if (!priv->droppingContexts.contains(context)) {
        droppingContext = new DroppingContext;
        droppingContext->webView = webView;
        droppingContext->gdkContext = context;
        droppingContext->dataObject = WebCore::DataObjectGtk::create();
        droppingContext->dropHappened = false;
        droppingContext->lastMotionPosition = position;
        priv->droppingContexts.set(context, droppingContext);

        Vector<GdkAtom> acceptableTargets(pasteboardHelperInstance()->dropAtomsForContext(widget, context));
        droppingContext->pendingDataRequests = acceptableTargets.size();
        for (size_t i = 0; i < acceptableTargets.size(); i++)
            gtk_drag_get_data(widget, context, acceptableTargets.at(i), time);
    } else {
        droppingContext = priv->droppingContexts.get(context);
        droppingContext->lastMotionPosition = position;
    }

    ASSERT(droppingContext);
    if (droppingContext->pendingDataRequests > 0)
        return TRUE;

    DragData dragData(droppingContext->dataObject.get(), position, globalPointForClientPoint(gtk_widget_get_window(widget), position), gdkDragActionToDragOperation(gdk_drag_context_get_actions(context)));
    DragOperation operation = core(webView)->dragController()->dragUpdated(&dragData);
    gdk_drag_status(context, dragOperationToSingleGdkDragAction(operation), time);

    return TRUE;
}
