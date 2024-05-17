static void resizeWebKitWebViewBaseFromAllocation(WebKitWebViewBase* webViewBase, GtkAllocation* allocation)
{
    gtk_container_foreach(GTK_CONTAINER(webViewBase), webkitWebViewBaseChildAllocate, webViewBase);

    IntRect viewRect(allocation->x, allocation->y, allocation->width, allocation->height);
    WebKitWebViewBasePrivate* priv = webViewBase->priv;
    if (priv->inspectorView) {
        GtkAllocation childAllocation = viewRect;
        childAllocation.y = allocation->height - priv->inspectorViewHeight;
        childAllocation.height = priv->inspectorViewHeight;
        gtk_widget_size_allocate(priv->inspectorView, &childAllocation);

        viewRect.setHeight(allocation->height - priv->inspectorViewHeight);
    }

    if (priv->pageProxy->drawingArea())
        priv->pageProxy->drawingArea()->setSize(viewRect.size(), IntSize());

    GtkWidget* toplevel = gtk_widget_get_toplevel(GTK_WIDGET(webViewBase));
    if (widgetIsOnscreenToplevelWindow(toplevel))
        webkitWebViewBaseNotifyResizerSizeForWindow(webViewBase, GTK_WINDOW(toplevel));
}
