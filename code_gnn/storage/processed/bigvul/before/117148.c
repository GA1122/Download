 static void webkitWebViewBaseNotifyResizerSizeForWindow(WebKitWebViewBase* webViewBase, GtkWindow* window)
 {
     gboolean resizerVisible;
    g_object_get(G_OBJECT(window), "resize-grip-visible", &resizerVisible, NULL);

    IntSize resizerSize;
    if (resizerVisible) {
        GdkRectangle resizerRect;
        gtk_window_get_resize_grip_area(window, &resizerRect);
        GdkRectangle allocation;
        gtk_widget_get_allocation(GTK_WIDGET(webViewBase), &allocation);
        if (gdk_rectangle_intersect(&resizerRect, &allocation, 0))
            resizerSize = IntSize(resizerRect.width, resizerRect.height);
    }

    if (resizerSize != webViewBase->priv->resizerSize) {
        webViewBase->priv->resizerSize = resizerSize;
        webViewBase->priv->pageProxy->setWindowResizerSize(resizerSize);
    }
}
