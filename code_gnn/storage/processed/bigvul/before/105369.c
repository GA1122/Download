static gboolean doDragLeaveLater(DroppingContext* context)
{
    WebKitWebView* webView = context->webView;
    WebKitWebViewPrivate* priv = webView->priv;

    if (!priv->droppingContexts.contains(context->gdkContext))
        return FALSE;

    if (context->pendingDataRequests)
        return FALSE;

    if (!context->dropHappened) {
        const IntPoint& position = context->lastMotionPosition;
        DragData dragData(context->dataObject.get(), position, globalPointForClientPoint(gtk_widget_get_window(GTK_WIDGET(webView)), position), DragOperationNone);
        core(webView)->dragController()->dragExited(&dragData);
    }

    core(webView)->dragController()->dragEnded();
    priv->droppingContexts.remove(context->gdkContext);
    delete context;
    return FALSE;
}
