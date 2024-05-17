static void dragExitedCallback(GtkWidget* widget, DragData* dragData, bool dropHappened)
{
    if (dropHappened)
        return;

    WebKitWebViewBase* webViewBase = WEBKIT_WEB_VIEW_BASE(widget);
    webViewBase->priv->pageProxy->dragExited(dragData);
    webViewBase->priv->pageProxy->resetDragOperation();
}
