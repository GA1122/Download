static void webkitWebViewBaseMap(GtkWidget* widget)
{
    GTK_WIDGET_CLASS(webkit_web_view_base_parent_class)->map(widget);

    WebKitWebViewBase* webViewBase = WEBKIT_WEB_VIEW_BASE(widget);
#if USE(TEXTURE_MAPPER_GL) && defined(GDK_WINDOWING_X11)
    GdkWindow* gdkWindow = gtk_widget_get_window(widget);
    ASSERT(gdkWindow);
    if (gdk_window_has_native(gdkWindow))
        webViewBase->priv->pageProxy->widgetMapped(GDK_WINDOW_XID(gdkWindow));
#endif

    if (!webViewBase->priv->needsResizeOnMap)
        return;

    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    resizeWebKitWebViewBaseFromAllocation(webViewBase, &allocation);
    webViewBase->priv->needsResizeOnMap = false;
}
